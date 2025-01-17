#ifndef __includes__
#define __includes__

#define _USE_MATH_DEFINES
#define _LIB
#define FREEGLUT_LIB_PRAGMAS 0

#define GLM_FORCE_RADIANS

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cassert>
#include <cmath>
#include <ctime>
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "glew.h"
//#include "glfw3.h"
#include "glfw3_1.h"
#include "GL/freeglut.h"
#include "GLSL.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" //perspective, trans etc
#include "glm/gtc/type_ptr.hpp" //value_ptr
#include "glm/glm.h"
#include "glm/glmint.h"
#include "tiny_obj_loader.h"
#include "Util.h"
#include "MatrixStack.h"
#include <irrKlang.h>

// defines the keys for corresponding shader programs
#define SHADER_DEFAULT "default"
#define SHADER_BILLBOARD "billboard"
#define SHADER_TEXT "text"
// defines the file names for shaders
#define DEFAULT_VERT_SHADER "shaders/vert.glsl"
#define DEFAULT_FRAG_SHADER "shaders/frag.glsl"
#define BILLBOARD_VERT_SHADER "shaders/billboard_vert.glsl"
#define BILLBOARD_FRAG_SHADER "shaders/billboard_frag.glsl"
#define TEXT_VERT_SHADER "shaders/text.v.glsl"
#define TEXT_FRAG_SHADER "shaders/text.f.glsl"

// defines file names for various meshes
#define EXTRA_FILE_NAME "objs/kaito.obj"
#define WALL_FILE_NAME "cube.obj"
#define STALL_FILE_NAME "objs/base_lantern_stall.obj"
#define PLAYER_FILE_NAME "objs/miku.obj"
#define LANTERN_FILE_NAME "objs/standing_lantern.obj"
#define FOUNTAIN_FILE_NAME "objs/fountain.obj"

// define map file
#define MAP_FILE_NAME "map.txt"

// defines the type of structure related to the Booth class
#define WALL_TYPE 0
#define BOOTH_TYPE 1
#define MISC_TYPE 2

// misc defines for globally accessed values
#define NUM_FOUNTAIN_PARTICLES 400
#define NUM_FIREFLY_PARTICLES 200
#define MAX_OBJS 20
#define SECS_PER_OBJ 2.0
#define SIZE 50.0
#define SKY_SIZE 300.0
#define NUM_OBJ_TYPES 1
#define SPHERE 0
#define MOUSE_SPEED 0.25
#define INITIAL_SPEED 5.0
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
#define PLAYER_ROT_DEG 4
#define NUM_PLANES_VIEW_FRUSTUM 6

// Game States
#define WORLD_STATE 0
#define TRANSITION_STATE 1
#define MINIGAME_STATE 2

// Minigames
#define NO_GAME "null"
#define NO_LIFE "Kayla and Patrick D="
#define SHOOTING_GALLERY "shooting_gallery"
#define WATERMELON_SMASH "watermelon_smash"
#define KARAOKE "karaoke"

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

struct plane {
   float a, b, c, d;
};

#endif
