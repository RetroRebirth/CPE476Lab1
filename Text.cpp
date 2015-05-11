#include "Text.h"

using namespace std;

Text::Text(
      FT_Library _ft) {
      //int _width,
      //int _height) {
      
   ft = _ft;
   //width = _width;
   //height = _height;

   //initFace();
}

Text::~Text(){
   // delete stuff...
}

void Text::createAtlas() {
 
   if(FT_New_Face(ft, "./Fonts/ostrich-black.ttf", 0, &face)) {
     fprintf(stderr, "Could not open font\n");
     exit(-1);
   }
   
   FT_GlyphSlot g = face->glyph;
   int w = 0;
   int h = 0;
    
   for(int i = 32; i < 128; i++) {
      if(FT_Load_Char(face, i, FT_LOAD_RENDER)) {
         fprintf(stderr, "Loading character %c failed!\n", i);
         continue;
      }
    
      w += g->bitmap.width;
      //h = max(h, g->bitmap.rows);
      if (g->bitmap.rows > h) {
         h = g->bitmap.rows;
      }
    
   }
   /* you might as well save this value as it is needed later on */
   atlas_width = w;
   atlas_height = h;
   
   glActiveTexture(GL_TEXTURE0);
   glGenTextures(1, &tex);
   glBindTexture(GL_TEXTURE_2D, tex);
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
   glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, atlas_width, atlas_height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, 0);
   
   int x = 0;
 
   for(int i = 32; i < 128; i++) {
     if(FT_Load_Char(face, i, FT_LOAD_RENDER))
       continue;
    
     glTexSubImage2D(GL_TEXTURE_2D, 0, x, 0, g->bitmap.width, g->bitmap.rows, GL_ALPHA, GL_UNSIGNED_BYTE, g->bitmap.buffer);
    
     x += g->bitmap.width;
   }
}  

void Text::initLibrary() {
   if(FT_Init_FreeType(&ft)) {
     fprintf(stderr, "Could not init freetype library\n");
     exit(1);
   }
}

void Text::initObjs(Object** _objs) {
   objs = _objs;
}

float Text::pixelToWorldCoordX(int pix) {
   //printf("%d\n", pix);
   return (2.0 * atlas_width / pix - 1);
}

float Text::pixelToWorldCoordY(int pix) {
   //printf("%d\n", pix);
   return (2.0 * atlas_height / pix + 1);
}

void Text::renderText(const char* text, glm::vec3 camPos, float x, float y, float sx, float sy) {
   const char *p;
   int i = 0;
 
   for(p = text; *p; p++, i++) {
      // load each character from the face
      if(FT_Load_Char(face, *p, FT_LOAD_RENDER))
        continue;

      //glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, g->bitmap.width, g->bitmap.rows, 0, 
        //GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);

      // READ ME: try to get it working in its own program

      //printf("%lf %lf\n", x, y);
      // set up the variables for where to start drawing text
      float x2 = pixelToWorldCoordX(x + g->bitmap_left * sx);
      float y2 = pixelToWorldCoordY(-y - g->bitmap_top * sy);
      float w = pixelToWorldCoordX(g->bitmap.width * sx);
      float h = pixelToWorldCoordY(g->bitmap.rows * sy);
 
      objs[i]->setPos(glm::vec3(camPos.x, camPos.y+.5, camPos.z-.5));
      
      printf("%lf\n", w); 
      objs[i]->scale(glm::vec3(w, h, .01));
      objs[i]->draw();
      
      // above three lines to replace this
      /*GLfloat box[4][4] = {
         {x2,     -y2    , 0, 0},
         {x2 + w, -y2    , 1, 0},
         {x2,     -y2 - h, 0, 1},
         {x2 + w, -y2 - h, 1, 1},
      };

      glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_DYNAMIC_DRAW);
      glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);*/

      // move where text is being drawn by scale factor
      x += (g->advance.x >> 6) * sx;
      y += (g->advance.y >> 6) * sy;
   }
}   

void Text::displayText(const char* text, glm::vec3 camPos, int x, int y) {
   /*float sx = 2.0 / width;
   float sy = 2.0 / height;

   renderText(text, camPos, x, y, sx, sy);*/
}
