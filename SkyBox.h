#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cassert>
#include <cmath>
#include <stdio.h>

#include "includes.h"
#include "Util.h"
#include "GLSL.h"
#include "tiny_obj_loader.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" //perspective, trans etc
#include "glm/gtc/type_ptr.hpp" //value_ptr

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
      SkyBox();
      virtual ~SkyBox();

      void init();
      void draw();

      // Load the textures for the skybox
      void loadTexture(char* image_file, int texID);
      int loadImage(char *filename, Image *image);

   private:
      Image* TextureImage;
      RGB myimage[64][64];
      RGB* g_pixel;

      GLuint texture_id;
      GLuint boxBuffObj, boxIndBuffObj, texBuffObj; 
      GLint h_aPos, h_aNor, h_uM, h_uAClr, h_uDClr, h_uSClr, h_uS;
      bufID_t boxBufIDs;

      vector<tinyobj::shape_t> shapes;
      vector<tinyobj::material_t> materials;
      GLint h_aPos, h_aNor;
      GLint h_uM;
      GLint h_uAClr, h_uDClr, h_uSClr, h_uS;
      bufID_t groundBufIDs;
      vector<Object*> objects;
      vector<Object*> activeObjects;

      // Helper functions
      unsigned int getint(FILE *fp) {
         return ((unsigned int) getc(fp)) + (((unsigned int) getc(fp)) << 8) + (((unsigned int) getc(fp)) << 16) + (((unsigned int) getc(fp)) << 24);
      }
      unsigned int getshort(FILE *fp) {
         return ((unsigned int) getc(fp)) + (((unsigned int) getc(fp)) << 8);
      }
      
};


