#ifndef __TEXTURE__
#define __TEXTURE__

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "includes.h"

using namespace cv;

class Texture {
public:
    // constructors & destructors
    Texture() {
        mipmapsGenerated = false;
        data = NULL;
    }
    virtual ~Texture() {
        //if (data != NULL)
          //  delete data;
    }
    
    // methods
    void loadTexture(char *filename, int texture_id, bool genMipMaps);
    void loadTexture(Mat frame, int texture_id);
    
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
#define TEX_MIKU 14
#define TEX_RIN 15
#define TEX_LEN 16
#define TEX_KAITO 17
#define TEX_SQUISH_BLUE 18
#define TEX_SQUISH_RED 19
#define TEX_SQUISH_PURPLE 20
#define TEX_STEEL 21
#define TEX_SONGS 50

#define TEX_SCENE 500
#define TEX_GLOW 501
#define TEX_BLUR 502

void loadAllTextures();
#endif
