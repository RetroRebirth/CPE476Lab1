#ifndef __Camera__
#define __Camera__

#include "includes.h"

class Camera {
   public:
      Camera();
      virtual ~Camera();

      inline void safe_glUniformMatrix4fv(const GLint handle, const GLfloat data[]);
      glm::vec3 lookAtPt();
      void setProjectionMatrix(int g_width, int g_height, GLint h_uP);
      void setView(GLint h_uV, GLint h_uView);
      void key_check(GLFWwindow* window);
      void mouse_callback(GLFWwindow* window, double xpos, double ypos, int g_width, int g_height);
      void enter_callback(GLFWwindow* window, int entered, int g_width, int g_height);

      glm::vec3 g_view;
      float theta;
      float phi;
};

#endif
