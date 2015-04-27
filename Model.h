#include "includes.h"
#include "Texture.h"

class Material {
public:
    int texture;
};

class Model {
public:
    // constructors & destructors
    Model();
    virtual ~Model();
    
    // methods
    void loadModel(char* filename, GLint h_tex);
    void drawModel();
    void finalizeModel(GLuint ShadeProg);
    void bindModel();
    
    // setters
    
    // getters
    
    // modifiers
    
private:
    // descriptive variables
    glm::vec3 position, dimension, direction, color;
    bool loaded;
    
    // vertex buffer variables
    GLuint vBufferID;
    GLuint buffer;
    GLuint VAO;
    vector<char> data;
    int size, curSize;
    
    // texture variables
    vector<Texture> textures;
    vector<int> meshIndices;
    vector<int> meshSizes;
    vector<int> materialIndices;
    int numMaterials;
    
    // animation variables
    float velocity;
};