#include "Text.h"

using namespace std;

Text::Text(
      FT_Library _ft,
      int _width,
      int _height) {
      
   ft = _ft;
   width = _width;
   height = _height;

   initFace();
}

Text::~Text(){
   // delete stuff...
}

void Text::initFace() {
   if(FT_New_Face(ft, "Fonts/GoodDog.otf", 0, &face)) {
      fprintf(stderr, "Could not open font\n");
      exit(-1);
   }
   
   g = face->glyph;
}

void Text::initObjs(Object** _objs) {
   objs = _objs;
}

float Text::pixelToWorldCoordX(int pix) {
   //printf("%d\n", pix);
   return (2.0 * width / pix - 1);
}

float Text::pixelToWorldCoordY(int pix) {
   //printf("%d\n", pix);
   return (2.0 * height / pix + 1);
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
   float sx = 2.0 / width;
   float sy = 2.0 / height;

   renderText(text, camPos, x, y, sx, sy);
}
