#include "includes.h"
#include "Camera.h"
#include "Object.h"
#include "Window.h"
#include "World.h"

// Window
Window* window;
// Camera
Camera* camera;
// World
World* world;
// Shader handlers
GLuint ShadeProg;
GLint h_aPos;
GLint h_aNor;
GLint h_uP;
GLint h_uV;
GLint h_uM;
GLint h_uView;
GLint h_uAClr, h_uDClr, h_uSClr, h_uS;

/** UTILITY **/
float randF() {
   return (float)rand() / RAND_MAX;
}

inline void safe_glUniformMatrix4fv(const GLint handle, const GLfloat data[]) {
   if (handle >= 0)
      glUniformMatrix4fv(handle, 1, GL_FALSE, data);
}

bool installShaders(const string &vShaderName, const string &fShaderName) {
   GLint rc;

   // Create shader handles
   GLuint VS = glCreateShader(GL_VERTEX_SHADER);
   GLuint FS = glCreateShader(GL_FRAGMENT_SHADER);

   // Read shader sources
   const char *vshader = GLSL::textFileRead(vShaderName.c_str());
   const char *fshader = GLSL::textFileRead(fShaderName.c_str());
   glShaderSource(VS, 1, &vshader, NULL);
   glShaderSource(FS, 1, &fshader, NULL);

   // Compile vertex shader
   glCompileShader(VS);
   GLSL::printError();
   glGetShaderiv(VS, GL_COMPILE_STATUS, &rc);
   GLSL::printShaderInfoLog(VS);
   if(!rc) {
      printf("Error compiling vertex shader %s\n", vShaderName.c_str());
      return false;
   }

   // Compile fragment shader
   glCompileShader(FS);
   GLSL::printError();
   glGetShaderiv(FS, GL_COMPILE_STATUS, &rc);
   GLSL::printShaderInfoLog(FS);
   if(!rc) {
      printf("Error compiling fragment shader %s\n", fShaderName.c_str());
      return false;
   }

   // Create the program and link
   ShadeProg = glCreateProgram();
   glAttachShader(ShadeProg, VS);
   glAttachShader(ShadeProg, FS);
   glLinkProgram(ShadeProg);

   GLSL::printError();
   glGetProgramiv(ShadeProg, GL_LINK_STATUS, &rc);
   GLSL::printProgramInfoLog(ShadeProg);
   if(!rc) {
      printf("Error linking shaders %s and %s\n", vShaderName.c_str(), fShaderName.c_str());
      return false;
   }

   // Get handles to attribute data
   h_aPos = GLSL::getAttribLocation(ShadeProg, "aPos");
   h_aNor = GLSL::getAttribLocation(ShadeProg, "aNor");
   h_uP = GLSL::getUniformLocation(ShadeProg, "uP");
   h_uV = GLSL::getUniformLocation(ShadeProg, "uV");
   h_uM = GLSL::getUniformLocation(ShadeProg, "uM");
   h_uView = GLSL::getUniformLocation(ShadeProg, "uView");
   h_uAClr = GLSL::getUniformLocation(ShadeProg, "uAClr");
   h_uDClr = GLSL::getUniformLocation(ShadeProg, "uDClr");
   h_uSClr = GLSL::getUniformLocation(ShadeProg, "uSClr");
   h_uS = GLSL::getUniformLocation(ShadeProg, "uS");

   assert(glGetError() == GL_NO_ERROR);
   return true;
}

/** INITIALIZING FOR DRAW **/
void initGL() {
//   loadShapes(objectFiles[0]);

   // Initialize GLEW
   if (glewInit() != GLEW_OK) {
      fprintf(stderr, "Failed to initialize GLEW\n");
      exit(-1);
   }

   installShaders("vert.glsl", "frag.glsl");

   // Enable alpha drawing
   glEnable (GL_BLEND);
   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glEnable (GL_DEPTH_TEST);

   // Set the background color
   glClearColor(0.6f, 0.6f, 0.8f, 1.0f);

   // Unbind the arrays
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   GLSL::checkVersion();
   assert(glGetError() == GL_NO_ERROR);
}

/** DRAWING **/
void step() { glfwSwapBuffers(window->glfw_window);
   glfwPollEvents();
   // Clear the screen
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   // Use "frag.glsl" and "vert.glsl"
   glUseProgram(ShadeProg);
   // Send position info to the attribute "aPos"
   GLSL::enableVertexAttribArray(h_aPos);
   // Send normal info to the attribute "aNor"
   GLSL::enableVertexAttribArray(h_aNor);

   // Step other components
   window->step();
   camera->step(window);
   world->step(window);

   // Disable and unbind
   GLSL::disableVertexAttribArray(h_aPos);
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
   glUseProgram(0);
   assert(glGetError() == GL_NO_ERROR);
}

/** WINDOW CALLBACKS **/
void window_size_callback(GLFWwindow* window, int w, int h) {
   glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void mouse_callback(GLFWwindow* win, double xpos, double ypos) {
   camera->mouse_callback(win, xpos, ypos, window->width, window->height);
}

void enter_callback(GLFWwindow* win, int entered) {
   camera->enter_callback(win, entered, window->width, window->height);
}

void key_callback(GLFWwindow* win, int key, int scancode, int action, int mods) {
   if (action != GLFW_PRESS) {
      return;
   }

   switch(key) {
   case GLFW_KEY_C:
      camera->bounded = !camera->bounded;
      break;
   case GLFW_KEY_UP:
      camera->speed += .01;
      break;
   case GLFW_KEY_DOWN:
      if (camera->speed - .01 > 0) {
         camera->speed -= .01;
      }
      break;
   }
}

/** MAIN **/
int main(int argc, char **argv) {
   Window _window(1024, 768, "CPE 476 Lab 1");
   window = &_window;

   // Set input callbacks
   glfwSetWindowSizeCallback(window->glfw_window, window_size_callback);
   glfwSetCursorPosCallback(window->glfw_window, mouse_callback);
   glfwSetCursorEnterCallback(window->glfw_window, enter_callback);
   glfwSetKeyCallback(window->glfw_window, key_callback);

   initGL();

   // Initialize everything else (mesh data, shaders, OpenGL states, etc.)
   World _world(h_uAClr, h_uDClr, h_uSClr, h_uS, h_uM, h_aPos, h_aNor);
   world = &_world;

   Camera _camera(h_uP, h_uV, h_uView);
   camera = &_camera;

   do {
      step();

      // TODO don't use glut
      char fps_text[30];
      sprintf(fps_text, "%lf fps", window->fps);
      window->drawText(fps_text, 0.75, 0.9);

      char obj_text[30];
      sprintf(obj_text, "%d out of %d (%d max)", 0, world->objCount, MAX_OBJS);
      window->drawText(obj_text, -0.95, 0.9);

      char spd_text[30];
      sprintf(spd_text, "%.2lf mph", camera->speed);
      window->drawText(spd_text, -0.1, 0.9);
   } while(window->isActive());

   return 0;
}
