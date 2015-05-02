#include "Session.h"

Session::Session() {
   window = new Window(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);

   initGL();

   camera = new Camera(h_uP, h_uV, h_uView);
   world = new World(ShadeProg, camera);
   
   game_state = WORLD_STATE;
}

Session::~Session() {
   delete window;
   delete camera;
   delete world;
}

/**
 * Starts the main game loop.
 */
void Session::run() {
   do {
      step();
   } while(window->isActive());
}

bool Session::installShaders(const string &vShaderName, const string &fShaderName) {
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
   h_aTexCoord = GLSL::getAttribLocation(ShadeProg,  "aTexCoord");
   h_uTexUnit = GLSL::getUniformLocation(ShadeProg, "uTexUnit");

   assert(glGetError() == GL_NO_ERROR);
   return true;
}

void Session::initGL() {
/*
   FT_Library ft;

   // Initialize Freetype 
   if(FT_Init_FreeType(&ft)) {
      fprintf(stderr, "Failed to intialize Freetype\n");
      exit(-1);
   }
*/
   
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

   // Enable texture drawing
   glEnable(GL_TEXTURE_2D);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

   // Set the background color
   glClearColor(0.6f, 0.6f, 0.8f, 1.0f);

   // Unbind the arrays
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   GLSL::checkVersion();
   assert(glGetError() == GL_NO_ERROR);
}

/**
 * This is called on every game loop.
 */
void Session::step() {
   glfwSwapBuffers(window->glfw_window);
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
   world->step(window);

   // Disable and unbind
   GLSL::disableVertexAttribArray(h_aPos);
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
   glUseProgram(0);
   //assert(glGetError() == GL_NO_ERROR);
}

Camera* Session::getCamera() {
   return camera;
}

Window* Session::getWindow() {
   return window;
}

/* handles mouse clicks thrown in main! Takes in a direction */
void Session::mouseClick(glm::vec3 direction) {
   //if (game_state == MINIGAME_STATE) {
      // bullet time!
      world->mouseClick(direction);
   //}
}

void Session::enterMinigame() {
   Booth* booth = world->currentActiveBooth();
   if (booth->getMinigame() != NULL) {
      game_state = MINIGAME_STATE;
      world->inMiniGame();
      camera->moveToMinigame();
      // TODO set up controls for playing minigame
      // TODO draw a sphere that we can click
   }
}

void Session::leaveMinigame() {
   game_state = WORLD_STATE;
   world->leftMiniGame();
   camera->moveToOverworld();
   // TODO set up controls for moving around overworld
}
