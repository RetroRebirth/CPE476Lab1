#ifndef __Text__
#define __Text__

#include "includes.h"
#include "FontEngine.h"
#include "Program.h"
using namespace std;

class Text {
   public:
      Text(Program* prog, int width, int height);
      virtual ~Text();
      
      int init();
      void display(Program* prog, glm::vec4 col, int size, const char* text);
      
   private:
      FontEngine* fontEngine;
      std::string ostrichHandle;
};

#endif
