#ifndef __TEXTURE__
#define __TEXTURE__

#include "includes.h"

class Texture {
public:
    // constructors & destructors
    Texture() { mipmapsGenerated = false; }
    virtual ~Texture() { deleteTexture(); }
    
    // methods
    void loadTexture(char *filename, int texture_id, bool genMipMaps);
    unsigned int getint(FILE *fp) {
        return ((unsigned int) getc(fp)) + (((unsigned int) getc(fp)) << 8) + (((unsigned int) getc(fp)) << 16) + (((unsigned int) getc(fp)) << 24);
    }
    unsigned int getshort(FILE *fp) {
        return ((unsigned int) getc(fp)) + (((unsigned int) getc(fp)) << 8);
    }
    
    bool loadTextureX(char* filename, bool genMipMaps);
    void bindTexture(int texture_id);
    void reloadTexture();
    void deleteTexture();
    
    // getters
    int getHeight()      { return height;    }
    int getWidth()       { return width;     }
    int getBPP()         { return bpp;       }
    int getID()          { return texture;   }
    string getFilename() { return filename;  }
    
private:
    // variables
    GLuint texture, sampler;
    char* data;
    int minification, magnification;
    
    // image descriptors
    int width, height, bpp;
    char* filename;
    
    // mipmaps
    bool mipmapsGenerated;
};

#define NUM_TEXTURES 5
extern Texture textures[NUM_TEXTURES];
void loadAllTextures();
#endif