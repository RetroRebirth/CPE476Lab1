#ifndef __Camera__
#define __Camera__

#include "includes.h"
#include "Window.h"

class Camera {
   public:
      bool bounded;
      float speed; // TODO

      Camera(
         GLint _h_uP,
         GLint _h_uV,
         GLint _h_uView);
      virtual ~Camera();

      void toggleBounded();
      glm::vec3 lookAtPt();
      void setProjectionMatrix(int g_width, int g_height);
      void setView();
      void step(Window* window);
      void mouse_callback(GLFWwindow* window, double xpos, double ypos, int g_width, int g_height);
      void enter_callback(GLFWwindow* window, int entered, int g_width, int g_height);
   private:
      glm::vec3 view;
      float theta;
      float phi;
      GLint h_uP;
      GLint h_uV;
      GLint h_uView;

      inline void safe_glUniformMatrix4fv(const GLint handle, const GLfloat data[]);
      void updatePosition(Window* window);
};

#endif
