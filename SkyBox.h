#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cassert>
#include <cmath>
#include <stdio.h>

#include "includes.h"
#include "Util.h"
#include "Camera.h"
#include "GLSL.h"
#include "tiny_obj_loader.h"

typedef struct Image {
   unsigned long sizeX;
   unsigned long sizeY;
   char* data;
} Image;

typedef struct RGB {
  GLubyte r;
  GLubyte g;
  GLubyte b;
} RGB;

class SkyBox
{
   public:
      SkyBox(GLint _h_aPos, GLint _h_aNor, GLint _h_uM, GLint _h_uTexUnit, GLint h_aTexCoord);
      virtual ~SkyBox();

      void init();
      void draw(Camera* camera);

      // Load the textures for the skybox
      void loadTexture(char* image_file, int texID);
      int loadImage(char *filename, Image *image);

   private:
      Image* TextureImage;
      RGB myimage[64][64];
      RGB* g_pixel;

      GLint h_aPos, h_aNor, h_uM;
      bufID_t boxBufIDs; // pos = pos, ind = ind, nor = textures
      GLint h_uTexUnit, h_aTexCoord;
      
      // Helper functions
      unsigned int getint(FILE *fp) {
         return ((unsigned int) getc(fp)) + (((unsigned int) getc(fp)) << 8) + (((unsigned int) getc(fp)) << 16) + (((unsigned int) getc(fp)) << 24);
      }
      unsigned int getshort(FILE *fp) {
         return ((unsigned int) getc(fp)) + (((unsigned int) getc(fp)) << 8);
      }
      
};


