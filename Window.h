#ifndef __Window__
#define __Window__

#include "includes.h"

class Window {
   public:
      GLFWwindow* glfw_window;
      int width;
      int height;
      int frames;

      Window(
         int _width,
         int _height,
         const char* title);
      virtual ~Window();

      void drawText(char* string, int x, int y);
      bool isActive();
      void update();
};

#endif
