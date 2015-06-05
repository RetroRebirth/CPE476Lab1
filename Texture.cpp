#include "Texture.h"

map<int, GLuint> textures;
GLuint video_texture;

/* Initially loads all textures used in this program */
void loadAllTextures()
{
    ifstream textureFile;
    textureFile.open("textures.txt");
    string line;
    if (textureFile.is_open()) {
        while (getline(textureFile, line)) {
            if (line[0] == '#')
                continue;
            
            int texID;
            char texName[100];
            sscanf(line.c_str(), "%d %s\n", &texID, texName);
            
            Texture newTex;
            newTex.loadTexture(texName, texID, true);
        }
    }
    else {
        printf("file 'textures.txt' was not available or could not be opened\n");
    }
    
    glGenTextures(1, &video_texture);
    glBindTexture(1, video_texture);
    textures[100] = video_texture;
}

/* Loads a .bmp texture from the given file and loads it into the texture_id */
void Texture::loadTexture(char *_filename, int texture_id, bool genMipMaps)
{
    FILE *file;
    filename = _filename;
    
    // make sure the file is there
    if ((file = fopen(filename, "rb")) == NULL) {
        printf("File Not Found: %s\n", filename);
        return;
    }
    // get the size of the image
    fseek(file, 18, SEEK_CUR);
    width = getint (file);  // read the width
    height = getint (file); // read the height
    unsigned long size = (width * height * 3);
    
    // read the planes
    unsigned short int planes = getshort(file);
    if (planes != 1) {
        printf("Planes from %s is not 1: %u\n", filename, planes);
        return;
    }
    // read the bpp
    bpp = getshort(file);
    if (bpp != 24) {
        printf("Bpp from %s is not 24: %u\n", filename, bpp);
        return;
    }
    fseek(file, 24, SEEK_CUR);
    
    // read the data
    char* data = (char *) malloc(size);
    if (data == NULL) {
        printf("Error allocating memory for color-corrected image data");
        return;
    }
    if ((fread(data, size, 1, file)) != 1) {
        printf("Error reading image data from %s.\n", filename);
        return;
    }
    fclose(file);
    
    // Generate an OpenGL texture id
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    // Generate a 2D image for the image's format
    glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
    
    // Generate mip maps
    if (genMipMaps) {
        glEnable(GL_TEXTURE_2D);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    }
    mipmapsGenerated = genMipMaps;
    
    textures[texture_id] = texture;
    free(data);
}

/* Loads a texture from a video frame */
void Texture::loadTexture(Mat frame, int texture_id)
{
    // Generate the image
    IplImage* img = new IplImage(frame);
    
    // Generate an OpenGL texture id
    glBindTexture(GL_TEXTURE_2D, video_texture);
    
    // Generate a 2D image for the image's format
    glTexImage2D(GL_TEXTURE_2D, 0, 3, img->width, img->height, 0, GL_BGR, GL_UNSIGNED_BYTE, img->imageData);
    
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    
    textures[texture_id] = video_texture;
}