#include "Texture.h"

Texture textures[NUM_TEXTURES];

/* Initially loads all textures used in this program */
void loadAllTextures()
{
    char *textureNames[] = {
        (char *)"textures/shadow.bmp",
        (char *)"textures/skydome.bmp",
        (char *)"textures/ground_sakura.bmp",
        (char *)"textures/ground_grass.bmp",
        (char *)"textures/wood_light.bmp",
        (char *)"textures/wood_dark.bmp",
        (char *)"textures/wood_red.bmp",
        (char *)"textures/wood_wall.bmp",
        (char *)"textures/greyConcrete.bmp",
        (char *)"textures/watermelon_outside.bmp",
        (char *)"textures/watermelon_inside.bmp",
        (char *)"textures/target.bmp",
        (char *)"textures/alpha.bmp"
    };
    // load the textures
    for (int i = 0; i < NUM_TEXTURES; i++)
    {
        textures[i].loadTexture(textureNames[i], i, true);
    }
}

/* Loads a .bmp texture from the given file and loads it into the texture_id */
void Texture::loadTexture(char *_filename, int texture_id, bool genMipMaps)
{
    FILE *file;
    filename = _filename;
    texture = texture_id;
    
    // make sure the file is there
    if ((file = fopen(filename, "rb")) == NULL) {
        printf("File Not Found: %s\n", filename);
        return;
    }
    // get the size of the image
    fseek(file, 18, SEEK_CUR);
    width = getint (file);  // read the width
    height = getint (file); // read the height
    unsigned long size = width * height * 3;
    
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
    data = (char *) malloc(size);
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
    glBindTexture(GL_TEXTURE_2D, texture_id);
    
    // Generate a 2D image for the image's format
    glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
    
    // Generate mip maps
    if (genMipMaps) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    mipmapsGenerated = genMipMaps;
}

/* Binds a texture to the texture unit id */
void Texture::bindTexture()
{
    glBindTexture(GL_TEXTURE_2D, texture);
}

void Texture::reloadTexture()
{
    FILE *file;
    // Load the texture from the image
    if ((file = fopen(filename, "rb")) == NULL) {
        printf("File not found: %s\n", filename);
    }
    fseek(file, 18, SEEK_CUR);
    width = getint (file);
    height = getint (file);
    unsigned long size = width * height * 3;
    bpp = getshort(file);
    fseek(file, 24, SEEK_CUR);
    
    // read the data and reverse all of the colors. (bgr -> rgb)
    char temp;
    data = (char *) malloc(size);
    for (int i = 0; i < size; i += 3) {
        temp = data[i];
        data[i] = data[i+2];
        data[i+2] = temp;
    }
    fclose(file);
    
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_BGR, GL_UNSIGNED_BYTE, data);
    if (mipmapsGenerated) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }
}
