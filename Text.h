#ifndef __Text__
#define __Text__

#include "includes.h"
#include "Object.h"

class Text {
   public:
      Text(FT_Library _ft, int _width, int _height);
      virtual ~Text();
      
      FT_Library ft;
      Object** objs;
            
      void initObjs(Object** _objs);      
      void displayText(const char* text, glm::vec3 camPos, int x, int y);
      
   private:
      FT_GlyphSlot g;
      FT_Face face;
      int width;
      int height;

      void initFace();
      float pixelToWorldCoordX(int pix);
      float pixelToWorldCoordY(int pix);      
      void renderText(const char *text, glm::vec3 camPos, float x, float y, float sx, float sy);
};

#endif
