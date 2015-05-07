#include "Model.h"

Model::Model()
{
    loaded = false;
    buffer = 0;
}

Model::~Model()
{
    
}

void Model::loadModel(char *filename, GLint h_tex)
{
    if (buffer == 0)
    {
        glGenBuffers(1, &buffer);
        data.reserve(50);
        size = 50;
        curSize = 0;
    }
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(filename, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);
    
    if (!scene)
    {
        exit(1);
    }
    const int vertexTotalSize = sizeof(aiVector3D) * 2 + sizeof(aiVector2D);
    int totalVertices = 0;
    
    for (int i = 0; i < scene->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[i];
        int meshFaces = mesh->mNumFaces;
        materialIndices.push_back(mesh->mMaterialIndex);
        int sizeBefore = curSize;
        meshIndices.push_back(sizeBefore/vertexTotalSize);
        
        for (int j = 0; j < meshFaces; j++)
        {
            const aiFace &face = mesh->mFaces[j];
            
            for (int k = 0; k < 3; k++)
            {
                aiVector3D pos = mesh->mVertices[face.mIndices[k]];
                aiVector3D uv = mesh->mTextureCoords[0][face.mIndices[k]];
                aiVector3D normal = mesh->HasNormals() ? mesh->mNormals[face.mIndices[k]] : aiVector3D(1.0f, 1.0f, 1.0f);
                
                data.insert(data.end(), (char *)&pos, (char *)&pos + sizeof(aiVector3D));
                curSize += sizeof(aiVector3D);
                
                data.insert(data.end(), (char *)&uv, (char *)&uv + sizeof(aiVector2D));
                curSize += sizeof(aiVector2D);
                
                data.insert(data.end(), (char *)&normal, (char *)&normal + sizeof(aiVector3D));
                curSize += sizeof(aiVector3D);
            }
        }
        int meshVertices = mesh->mNumVertices;
        totalVertices += meshVertices;
        meshSizes.push_back((curSize - sizeBefore) / vertexTotalSize);
    }
    numMaterials = scene->mNumMaterials;
    
    vector<int> materialRemap(numMaterials);
    
    printf("# materials is %d\n", numMaterials);
    
    for (int i = 0; i < numMaterials; i++)
    {
        printf("Looping %d\n", i);
        
        const aiMaterial *material = scene->mMaterials[i];
        int a = 5, texIndex = 0;
        aiString path;  // filename
        
        if(material->GetTexture(aiTextureType_DIFFUSE, texIndex, &path) == AI_SUCCESS)
        {
            string dir = "objs/";
            string textureName = dir + path.data;
            int texFound = -1;
            
            for (int j = 0; j < textures.size(); j++)
            {
                if (textureName == textures[j].getFilename())
                {
                    texFound = j;
                    break;
                }
            }
            if(texFound != -1)
            {
                materialRemap[i] = texFound;
            }
            else
            {
                printf("Loading %s\n", &textureName[0]);
                
                Texture newTex;
                newTex.loadTexture(&textureName[0], true, h_tex);
                
                printf("Testing 1!\n");
                
                materialRemap[i] = textures.size();
                textures.push_back(newTex);
                
                printf("Testing 2!\n");
            }
        }
        
        printf("Finished loop %d\n", i);
    }
    
    printf("Testing 3!\n");
    
    for (int i = 0; i < meshSizes.size(); i++)
    {
        int oldIndex = materialIndices[i];
        materialIndices[i] = materialRemap[oldIndex];
    }
    
    printf("Testing 4!\n");
}

void Model::finalizeModel(GLuint ShadeProg)
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    // bind the vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    // upload data to the GPU
    glBufferData(GL_ARRAY_BUFFER, data.size(), &data[0], GL_STATIC_DRAW);
    data.clear();
    
    GLint h_pos = GLSL::getAttribLocation(ShadeProg, "aPos");
    GLint h_nor = GLSL::getAttribLocation(ShadeProg, "aNor");
    GLint h_tex = GLSL::getAttribLocation(ShadeProg, "aTexCoord");
    
    // send vertex positions
    glEnableVertexAttribArray(h_pos);
    glVertexAttribPointer(h_pos, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(aiVector3D) + sizeof(aiVector2D), 0);
    // send texture coordinates
    glEnableVertexAttribArray(h_tex);
    glVertexAttribPointer(h_tex, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(aiVector3D) + sizeof(aiVector2D), (void *)(sizeof(aiVector3D) + sizeof(aiVector2D)));
    // send normal vectors
    glEnableVertexAttribArray(h_nor);
    glVertexAttribPointer(h_nor, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(aiVector3D) + sizeof(aiVector2D), (void *)(sizeof(aiVector3D) + sizeof(aiVector2D)));
}

void Model::bindModel()
{
    glBindVertexArray(VAO);
}

/* Draws the model */
void Model::drawModel()
{
    if (!loaded) {
        return;
    }
    
    int numMeshes = meshSizes.size();
    for (int i = 0; i < numMeshes; i++)
    {
        int matIndex = materialIndices[i];
        textures[matIndex].bindTexture();
        glDrawArrays(GL_TRIANGLES, meshIndices[i], meshSizes[i]);
    }
}
