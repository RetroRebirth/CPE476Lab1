#ifndef __includes__
#define __includes__

#define _USE_MATH_DEFINES
#define _LIB
#define FREEGLUT_LIB_PRAGMAS 0
#include <stdio.h>
#include <stdlib.h>
#include "glew.h"
#include "glfw3.h"
#include <iostream>
#include <cassert>
#include <cmath>
#include <ctime>
#include <stdio.h>
#include "freeglut.h"
#include "GLSL.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" //perspective, trans etc
#include "glm/gtc/type_ptr.hpp" //value_ptr
#include "tiny_obj_loader.h"

#define MAX_OBJS 40
#define SECS_PER_OBJ 2.0
#define MOVEMENT_SPEED 0.1 // TODO base off time, not CPU speed
#define ROTATE_SPEED 0.25
#define SIZE 20.0
#define NUM_OBJ_TYPES 1
   #define SPHERE 0

using namespace std;

struct bufID_t {
   GLuint pos;
   GLuint ind;
   GLuint nor;
};

#endif
