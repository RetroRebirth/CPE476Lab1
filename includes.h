#ifndef __includes__
#define __includes__

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

using namespace std;

struct transform_t {
   glm::vec3 trans;
   glm::vec3 scale;
   float rotX;
   float rotY;
   float rotZ;
};
struct bufID_t {
   GLuint pos;
   GLuint ind;
   GLuint nor;
};

#endif
