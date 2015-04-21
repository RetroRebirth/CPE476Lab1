#ifndef __Session__
#define __Session__

#include "includes.h"
#include "Window.h"
#include "Camera.h"
#include "World.h"

class Session {
   public:
      Session();
      virtual ~Session();

      void run();
   private:
      Window* window;
      Camera* camera;
      World* world;

      GLuint ShadeProg;
      GLint h_aPos;
      GLint h_aNor;
      GLint h_uP;
      GLint h_uV;
      GLint h_uM;
      GLint h_uView;
      GLint h_uAClr, h_uDClr, h_uSClr, h_uS;
      GLint h_aTexCoord, h_uTexUnit;

      /** INITIALIZING **/
      bool installShaders(const string &vShaderName, const string &fShaderName);
      void initGL();

      void setInputCallbacks();

      void window_size_callback(GLFWwindow* window, int w, int h);
      void mouse_callback(GLFWwindow* win, double xpos, double ypos);
      void enter_callback(GLFWwindow* win, int entered);
      void key_callback(GLFWwindow* win, int key, int scancode, int action, int mods);

      /** MAIN GAME LOOP **/
      void step();
};

#endif
