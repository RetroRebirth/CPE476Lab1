#ifndef __TEXTURE__
#define __TEXTURE__

#include "includes.h"

class Texture {
public:
    // constructors & destructors
    Texture() { mipmapsGenerated = false; }
    virtual ~Texture() {}
    
    // methods
    void loadTexture(char *filename, int texture_id, bool genMipMaps);
    void bindTexture();
    void reloadTexture();
    
    // getters
    int getHeight()      { return height;    }
    int getWidth()       { return width;     }
    int getBPP()         { return bpp;       }
    int getID()          { return texture;   }
    string getFilename() { return filename;  }
    
private:
    // variables
    int texture;
    char* data;
    int minification, magnification;
    
    // image descriptors
    int width, height, bpp;
    char* filename;
    
    // mipmaps
    bool mipmapsGenerated;
    
    // helper functions
    unsigned int getint(FILE *fp) {
        return ((unsigned int) getc(fp)) + (((unsigned int) getc(fp)) << 8) + (((unsigned int) getc(fp)) << 16) + (((unsigned int) getc(fp)) << 24);
    }
    unsigned int getshort(FILE *fp) {
        return ((unsigned int) getc(fp)) + (((unsigned int) getc(fp)) << 8);
    }
};

#define TEX_MISC 0
#define TEX_SKY_DAY 1
#define TEX_GROUND_SAKURA 2
#define TEX_GROUND_GRASS 3
#define TEX_WOOD_LIGHT 4
#define TEX_WOOD_DARK 5
#define TEX_WOOD_RED 6
#define TEX_WOOD_WALL 7
#define TEX_LANTERN 8
#define TEX_MELON_OUT 9
#define TEX_MELON_IN 10
#define TEX_TARGET 11
#define TEX_PARTICLE 12
#define TEX_HAMMER 13

#define NUM_TEXTURES 14
extern Texture textures[NUM_TEXTURES];
void loadAllTextures();
#endif
