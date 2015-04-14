#include "includes.h"
#include "Camera.h"
#include "Object.h"
#include "Window.h"
#include "World.h"

static string objectFiles[] = {"sphere.obj"};

// Window
Window* window;
// Camera
Camera* camera;
// World
World* world;
// Objects
vector<tinyobj::shape_t> shapes; // TODO map of mesh data
vector<tinyobj::material_t> materials;
vector<Object> objects;
// Shader handlers
GLuint ShadeProg;
GLint h_aPos;
GLint h_aNor;
GLint h_uP;
GLint h_uV;
GLint h_uM;
GLint h_uView;
GLint h_uAClr, h_uDClr, h_uSClr, h_uS;
/*
// Ground
bufID_t groundBufIDs;
*/

/** UTILITY **/
float randF() {
   return (float)rand() / RAND_MAX;
}

inline void safe_glUniformMatrix4fv(const GLint handle, const GLfloat data[]) {
   if (handle >= 0)
      glUniformMatrix4fv(handle, 1, GL_FALSE, data);
}

void resize_obj(std::vector<tinyobj::shape_t> &shapes) {
   float minX, minY, minZ;
   float maxX, maxY, maxZ;
   float scaleX, scaleY, scaleZ;
   float shiftX, shiftY, shiftZ;
   float epsilon = 0.001;

   minX = minY = minZ = 1.1754E+38F;
   maxX = maxY = maxZ = -1.1754E+38F;

   //Go through all vertices to determine min and max of each dimension
   for (size_t i = 0; i < shapes.size(); i++) {
      for (size_t v = 0; v < shapes[i].mesh.positions.size() / 3; v++) {
         if(shapes[i].mesh.positions[3*v+0] < minX)
            minX = shapes[i].mesh.positions[3*v+0];
         if(shapes[i].mesh.positions[3*v+0] > maxX)
            maxX = shapes[i].mesh.positions[3*v+0];

         if(shapes[i].mesh.positions[3*v+1] < minY)
            minY = shapes[i].mesh.positions[3*v+1];
         if(shapes[i].mesh.positions[3*v+1] > maxY)
            maxY = shapes[i].mesh.positions[3*v+1];

         if(shapes[i].mesh.positions[3*v+2] < minZ)
            minZ = shapes[i].mesh.positions[3*v+2];
         if(shapes[i].mesh.positions[3*v+2] > maxZ)
            maxZ = shapes[i].mesh.positions[3*v+2];
      }
   }
   //From min and max compute necessary scale and shift for each dimension
   float maxExtent, xExtent, yExtent, zExtent;
   xExtent = maxX-minX;
   yExtent = maxY-minY;
   zExtent = maxZ-minZ;
   if (xExtent >= yExtent && xExtent >= zExtent) {
      maxExtent = xExtent;
   }
   if (yExtent >= xExtent && yExtent >= zExtent) {
      maxExtent = yExtent;
   }
   if (zExtent >= xExtent && zExtent >= yExtent) {
      maxExtent = zExtent;
   }
   scaleX = 2.0 /maxExtent;
   shiftX = minX + (xExtent/ 2.0);
   scaleY = 2.0 / maxExtent;
   shiftY = minY + (yExtent / 2.0);
   scaleZ = 2.0/ maxExtent;
   shiftZ = minZ + (zExtent)/2.0;

   // Go through all verticies shift and scale them
   for (size_t i = 0; i < shapes.size(); i++) {
      for (size_t v = 0; v < shapes[i].mesh.positions.size() / 3; v++) {
         shapes[i].mesh.positions[3*v+0] = (shapes[i].mesh.positions[3*v+0] - shiftX) * scaleX;
         assert(shapes[i].mesh.positions[3*v+0] >= -1.0 - epsilon);
         assert(shapes[i].mesh.positions[3*v+0] <= 1.0 + epsilon);
         shapes[i].mesh.positions[3*v+1] = (shapes[i].mesh.positions[3*v+1] - shiftY) * scaleY;
         assert(shapes[i].mesh.positions[3*v+1] >= -1.0 - epsilon);
         assert(shapes[i].mesh.positions[3*v+1] <= 1.0 + epsilon);
         shapes[i].mesh.positions[3*v+2] = (shapes[i].mesh.positions[3*v+2] - shiftZ) * scaleZ;
         assert(shapes[i].mesh.positions[3*v+2] >= -1.0 - epsilon);
         assert(shapes[i].mesh.positions[3*v+2] <= 1.0 + epsilon);
      }
   }
}

void loadShapes(const string &objFile) {
   string err = tinyobj::LoadObj(shapes, materials, objFile.c_str());
   if(!err.empty()) {
      cerr << err << endl;
   }
   resize_obj(shapes);
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
void createObject() {
   Object object(shapes, materials, h_uAClr, h_uDClr, h_uSClr, h_uS, h_uM, h_aPos, h_aNor);
   objects.push_back(object);
}

/*
void initGround() {
   // Position array of ground
   GLfloat vertices[] = {
      -1.0f, 0.0, -1.0f, //0
      -1.0f, 0.0, +1.0f, //1
      +1.0f, 0.0, -1.0f, //2
      +1.0f, 0.0, +1.0f, //3
   };
   // Amplify ground size
   for (int i = 0, last = sizeof(vertices)/sizeof(GLfloat); i < last; i++) {
      // Don't amplify the floor height
      if (i % 3 != 1)
         vertices[i] *= SIZE;
   }
   glGenBuffers(1, &(groundBufIDs.pos));
   glBindBuffer(GL_ARRAY_BUFFER, groundBufIDs.pos);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

   // Index array of ground
   GLuint indices[] = {
      0, 1, 2,
      1, 3, 2,
   };
   glGenBuffers(1, &(groundBufIDs.ind));
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, groundBufIDs.ind);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

   // Normal array of ground
   vector<float> posBuf(vertices, vertices+sizeof(vertices)/sizeof(vertices[0]));
   vector<unsigned int> indBuf(indices, indices+sizeof(indices)/sizeof(indices[0]));

   glGenBuffers(1, &(groundBufIDs.nor));
   glBindBuffer(GL_ARRAY_BUFFER, groundBufIDs.nor);
}
*/

void initGL() {
/*
   loadShapes(objectFiles[0]);

   // Initialize GLEW
   if (glewInit() != GLEW_OK) {
      fprintf(stderr, "Failed to initialize GLEW\n");
      exit(-1);
   }

   installShaders("vert.glsl", "frag.glsl");
*/

   // Enable alpha drawing
   glEnable (GL_BLEND);
   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glEnable (GL_DEPTH_TEST);

   // Set the background color
   glClearColor(0.6f, 0.6f, 0.8f, 1.0f);

//   initGround();
   world->initGround();

   // Unbind the arrays
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   GLSL::checkVersion();
   assert(glGetError() == GL_NO_ERROR);
}

/** DRAWING **/
/*
void drawGround() {
   // Bind position buffer
   glBindBuffer(GL_ARRAY_BUFFER, groundBufIDs.pos);
   glVertexAttribPointer(h_aPos, 3, GL_FLOAT, GL_FALSE, 0, 0);
   // Bind index buffer
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, groundBufIDs.ind);
   // Bind normal buffer
   glVertexAttribPointer(h_aNor, 3, GL_FLOAT, GL_FALSE, 0, 0);
   // Set color
   glUniform3f(h_uAClr, 0.13, 0.13, 0.14);
   glUniform3f(h_uDClr, 0.0, 0.0, 0.0);
   glUniform3f(h_uSClr, 0.0, 0.0, 0.0);
   glUniform1f(h_uS, 1.0);
   // Apply translation
   glm::vec3 trans = glm::vec3(0.0, 0.0, 0.0);
   glm::mat4 T = glm::translate(glm::mat4(1.0f), trans);
   safe_glUniformMatrix4fv(h_uM, glm::value_ptr(T));

   glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
*/

void drawGL() {
   // Clear the screen
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   // Use "frag.glsl" and "vert.glsl"
   glUseProgram(ShadeProg);
   // Send position info to the attribute "aPos"
   GLSL::enableVertexAttribArray(h_aPos);
   // Send normal info to the attribute "aNor"
   GLSL::enableVertexAttribArray(h_aNor);

   camera->setProjectionMatrix(window->width, window->height);
   camera->setView();

//   drawGround();
   world->drawGround();
/*
   for (vector<Object>::iterator it = objects.begin(); it != objects.end(); ++it) {
      (*it).draw();
      (*it).step(window->dt);
   }
*/

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
   double objStartTime = 0.0;
   //char *txt = (char *)malloc(sizeof(char)*10);

   Window _window(1024, 768, "CPE 476 Lab 1");
   window = &_window;

   // Set input callbacks
   glfwSetWindowSizeCallback(window->glfw_window, window_size_callback);
   glfwSetCursorPosCallback(window->glfw_window, mouse_callback);
   glfwSetCursorEnterCallback(window->glfw_window, enter_callback);
   glfwSetKeyCallback(window->glfw_window, key_callback);


   loadShapes(objectFiles[0]);

   // Initialize GLEW
   if (glewInit() != GLEW_OK) {
      fprintf(stderr, "Failed to initialize GLEW\n");
      exit(-1);
   }

   installShaders("vert.glsl", "frag.glsl");


   // Initialize everything else (mesh data, shaders, OpenGL states, etc.)
   World _world(shapes, materials, h_uAClr, h_uDClr, h_uSClr, h_uS, h_uM, h_aPos, h_aNor);
   world = &_world;

   initGL();

   Camera _camera(h_uP, h_uV, h_uView);
   camera = &_camera;

   vector<Object>::iterator it1;
   vector<Object>::iterator it2;
   int objCount = 0;

   do {
      window->step();
      camera->step(window);

      drawGL();

      // Create a new object every SECS_PER_OBJ
      if (objCount < MAX_OBJS && window->time - objStartTime >= SECS_PER_OBJ) {
         createObject();
         objStartTime = window->time;
         objCount++;
         printf("%d\n", objCount);
      }
    
      for (it1 = objects.begin(); it1 != objects.end(); ++it1) { 
         for (it2 = objects.begin(); it2 != objects.end(); ++it2) {
            if (it1 != it2) {
               (*it1).collisionDetection(*it2);
            }
         }
      }
   
      //TODO - Needs to move to window D:
      char str[20];
      sprintf(str, "hello world");
      window->drawText(str, 0.75, 0.9);

   } while(window->isActive());

   return 0;
}
