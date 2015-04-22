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
#include "GL/freeglut.h"
#include "GLSL.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" //perspective, trans etc
#include "glm/gtc/type_ptr.hpp" //value_ptr
#include "tiny_obj_loader.h"
#include "Util.h"
#include "MatrixStack.h"

// defines file names for various meshes
#define EXTRA_FILE_NAME "bunny.obj"
#define WALL_FILE_NAME "cube.obj"
#define PLAYER_FILE_NAME "sphere.obj"

// defines the type of structure related to the Booth class
#define WALL_TYPE 0
#define BOOTH_TYPE 1
#define MISC_TYPE 2
// Texture IDs
#define TEXTURE_SKY 3
#define TEXTURE_GROUND 4
#define TEXTURE_WOOD 5
// TODO add defines for specific minigame types

// misc defines for globally accessed values
#define MAX_OBJS 40
#define SECS_PER_OBJ 2.0
#define SIZE 25.0
#define SKY_SIZE 100.0
#define NUM_OBJ_TYPES 1
#define SPHERE 0
#define MOUSE_SPEED 0.25
#define INITIAL_SPEED 10.0
#define DELTA_SPEED 0.1
#define MIN_SPEED 0.1
#define MAX_SPEED 20.0
#define OBJ_SPEED 20.0
#define OBJ_SIZE 1.0
#define OBJ_SHRINK_RATE 0.5
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768
#define WINDOW_TITLE "Japanese Festival"
#define SCALE_CONST1 0.025f
#define SCALE_CONST2 1.3f
#define INFLUENCE_WIDTH 2.0f

// Game States
#define WORLD_STATE 0
#define TRANSITION_STATE 1
#define MINIGAME_STATE 2

using namespace std;

struct bufID_t {
   GLuint pos;
   GLuint ind;
   GLuint nor;
};

struct bound_box {
   float x_min, x_max;
   float y_min, y_max;
   float z_min, z_max;
};

#endif
