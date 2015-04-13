#ifndef __Camera__
#define __Camera__

#include "includes.h"

class Camera {
   public:
      bool bounded;

      Camera(
         GLint _h_uP,
         GLint _h_uV,
         GLint _h_uView);
      virtual ~Camera();

      void toggleBounded();
      glm::vec3 lookAtPt();
      void setProjectionMatrix(int g_width, int g_height);
      void setView();
      void key_check(GLFWwindow* window, const float elapsedTime);
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
};

#endif
