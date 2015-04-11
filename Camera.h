#ifndef __Camera__
#define __Camera__

#define _USE_MATH_DEFINES
#include <stdio.h>
#include <stdlib.h>
#include "glew.h"
#include "glfw3.h"
#include <iostream>
#include <cassert>
#include <cmath>
#include <ctime>
#include <stdio.h>
#include "GLSL.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" //perspective, trans etc
#include "glm/gtc/type_ptr.hpp" //value_ptr
#include "tiny_obj_loader.h"

#define MOVEMENT_SPEED 0.1 // TODO base off time, not CPU speed
#define ROTATE_SPEED 0.25
#define SIZE 20.0
#define NUM_OBJ_TYPES 1
   #define SPHERE 0

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
