#include "Session.h"

FontEngine* fontEngine;
map<string, Program*> shaders;
int global_points;
int game_state;

voxel spatialGrid[UNIFORM_GRID_SIZE];

Session::Session() {
   window = new Window(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
   initGL();
   loadAllTextures();
   initFBO();
   camera = new Camera(h_uP, h_uV, h_uView);
   world = new World(shaders[SHADER_DEFAULT]->getPID(), camera);
   clicks = new Clicks();
   fontEngine = new FontEngine(WINDOW_WIDTH, WINDOW_HEIGHT, shaders[SHADER_TEXT], shaders[SHADER_DEFAULT]);
   sound = new Sound();
   sound->initSound();
   camera->booths = world->booths;
   camera->structures = world->structures;
   minigame = new Minigame();
   game_state = TITLE_STATE;
   game_start = false;
   global_points = 0;
   
   world->initParticles(shaders[SHADER_BILLBOARD]);
   fontEngine->init(shaders[SHADER_TEXT]->getPID());

   camera->pov = false;
}

Session::~Session() {
   delete window;
   delete camera;
   delete world;
   delete sound;
   delete minigame;
}

/**
 * Starts the main game loop.
 */
void Session::run() {
   sound->playBackgroundMusic();
   do {
      step();
   } while(window->isActive());
}

bool Session::installShaders(Program* prog) {
   prog->init();
   return true;
}

GLuint FBO_Basic, FBO_CBasic, FBO_DBasic, FBO_TBasic;
void Session::initFBO() {
    /* Init basic FBO */
    glGenFramebuffers(1, &FBO_Basic);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO_Basic);
    // rendered texture
    glGenTextures(1, &FBO_TBasic);
    glBindTexture(GL_TEXTURE_2D, FBO_TBasic);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 768, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // depth buffer
    glGenRenderbuffers(1, &FBO_DBasic);
    glBindRenderbuffer(GL_RENDERBUFFER, FBO_DBasic);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 768);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, FBO_DBasic);
    // color buffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FBO_TBasic, 0);
    GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, DrawBuffers);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        printf("Error making basic frame buffer object\n");
        return;
    }
}

void Session::initGL() {
    //printf("openGL version: %s\n", (const char*)glGetString(GL_VERSION​​));
    std::cout << "openGL version " << (char*)glGetString(GL_VERSION) << endl;
    
    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
      fprintf(stderr, "Failed to initialize GLEW\n");
      exit(-1);
    }
    
    // initialize shaders
    shaders[SHADER_DEFAULT] = new Program();
    shaders[SHADER_DEFAULT]->setShaderNames(DEFAULT_VERT_SHADER, DEFAULT_FRAG_SHADER);
    
    //shaders[SHADER_COOK] = new Program();
    //shaders[SHADER_COOK]->setShaderNames(COOK_VERT_SHADER, COOK_FRAG_SHADER);
    
    shaders[SHADER_BILLBOARD] = new Program();
    shaders[SHADER_BILLBOARD]->setShaderNames(BILLBOARD_VERT_SHADER, BILLBOARD_FRAG_SHADER);
    
    shaders[SHADER_TEXT] = new Program();
    shaders[SHADER_TEXT]->setShaderNames(TEXT_VERT_SHADER, TEXT_FRAG_SHADER);

    // Enable alpha drawing
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    TODO multiple point source lights?
//    reference: http://gamedev.stackexchange.com/questions/19575/is-there-a-way-to-use-an-arbitrary-number-of-lights-in-a-fragment-shader
//    glBlendFunc(GL_ONE, GL_ONE);
    glEnable (GL_DEPTH_TEST);
    
    printf("done loading shaders\n");

    printf("textrue setFilename, unit, setName, init, shaders addtexture\n");
	texture.setFilename("textures/alpha.bmp");
	texture.setUnit(0);
	texture.setName("alphaTexture");
	texture.init();
	
    printf("initing shader\n");
    shaders[SHADER_DEFAULT]->init();
    //shaders[SHADER_COOK]->init();
    shaders[SHADER_BILLBOARD]->init();
    shaders[SHADER_TEXT]->init();
    
    h_aPos = shaders[SHADER_DEFAULT]->addAttribute("aPos");
    h_aNor = shaders[SHADER_DEFAULT]->addAttribute("aNor");
    h_uP = shaders[SHADER_DEFAULT]->addUniform("uP");
    h_uV = shaders[SHADER_DEFAULT]->addUniform("uV");
    h_uM = shaders[SHADER_DEFAULT]->addUniform("uM");
    h_uView = shaders[SHADER_DEFAULT]->addUniform("uView");
    h_aTexCoord = shaders[SHADER_DEFAULT]->addAttribute("aTexCoord");
    h_uTexUnit = shaders[SHADER_DEFAULT]->addUniform("uTexUnit");
    shaders[SHADER_DEFAULT]->addUniform("BlendMode");
    
    /*shaders[SHADER_COOK]->addAttribute("aPos");
    shaders[SHADER_COOK]->addAttribute("aNor");
    shaders[SHADER_COOK]->addUniform("uP");
    shaders[SHADER_COOK]->addUniform("uV");
    shaders[SHADER_COOK]->addUniform("uM");
    shaders[SHADER_COOK]->addUniform("uView");
    shaders[SHADER_COOK]->addAttribute("aTexCoord");
    shaders[SHADER_COOK]->addUniform("uTexUnit");
    shaders[SHADER_COOK]->addUniform("BlendMode");*/
    
    shaders[SHADER_BILLBOARD]->addAttribute("vertPosition");
    shaders[SHADER_BILLBOARD]->addAttribute("vertTexCoords");
    shaders[SHADER_BILLBOARD]->addUniform("P");
    shaders[SHADER_BILLBOARD]->addUniform("MV");
    shaders[SHADER_BILLBOARD]->addUniform("scale");
    shaders[SHADER_BILLBOARD]->addUniform("color");
	 shaders[SHADER_BILLBOARD]->addTexture(&texture);
	
    shaders[SHADER_TEXT]->addAttribute("aCoord");
    shaders[SHADER_TEXT]->addUniform("uTex");
    shaders[SHADER_TEXT]->addUniform("uCol");
    
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
    //assert(glGetError() == GL_NO_ERROR);
}
/**
 * This is called on every game loop.
 */
void Session::step() {
   //printf("in session step\n");
   glfwSwapBuffers(window->glfw_window);
   glfwPollEvents();
   // Clear the screen
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
   // Use "frag.glsl" and "vert.glsl"
   //shaders[SHADER_COOK]->bind();
   shaders[SHADER_DEFAULT]->bind();
   // Send position info to the attribute "aPos"
   GLSL::enableVertexAttribArray(h_aPos);
   // Send normal info to the attribute "aNor"
   GLSL::enableVertexAttribArray(h_aNor);

   // Step other components
   window->step();
   world->step(window);
   //camera->step(window, game_state);
   
   // Disable and unbind
   GLSL::disableVertexAttribArray(h_aPos);
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
   //shaders[SHADER_COOK]->unbind();
   
   //shaders[SHADER_DEFAULT]->bind();
   if (game_state == MINIGAME_STATE) {
      minigame->step(window);
   }

   if (minigame->getGameOver()) {
      //leaveMinigame();
   }
   shaders[SHADER_DEFAULT]->unbind();
   
   if (game_state == WORLD_STATE) {
      world->particleStep(shaders[SHADER_BILLBOARD], window);
   }
   
   char txt[30];
   sprintf(txt, "Matsuriyo!");
   if (game_state == WORLD_STATE || game_state == MINIGAME_STATE) {
      fontEngine->useFont("goodDog", 48);
      fontEngine->display(glm::vec4(0.99, 0.56, 0.55, 1.0), txt, -1.0, 0.9);
   
      char pts[15];
      sprintf(pts, "Points: %d", global_points);
      fontEngine->useFont("goodDog", 36);   
      fontEngine->display(glm::vec4(0.99, 0.56, 0.55, 1.0), pts, -.98, 0.9-(fontEngine->getTextHeight(txt)*1.1));
   } else if (game_state == TITLE_STATE) {
      fontEngine->useFont("goodDog", 72);
      fontEngine->display(glm::vec4(1.0, 0.22, 0.22, 1.0), txt, -0.75, -0.1);
   
      char subtext[30];
      sprintf(subtext, "Press ENTER to play");
      fontEngine->useFont("goodDog", 48);
      fontEngine->display(glm::vec4(1.0, 0.22, 0.22, 1.0), subtext, -0.75, -0.3);
   }
   /*
   char txt[30];
   sprintf(txt, "Matsuriyo!");
   fontEngine->useFont("goodDog", 48);
   fontEngine->display(glm::vec4(0.99, 0.56, 0.55, 1.0), txt, -1.0, 0.9);
*/

}

Camera* Session::getCamera() {
   return camera;
}

Window* Session::getWindow() {
   return window;
}

Clicks* Session::getClicks() {
  return clicks;
}

/* handles mouse clicks thrown in main! Takes in a direction */
void Session::mouseClick(glm::vec3 direction, glm::vec4 point) {
   // TODO pass click along to clicks
   if (game_state == MINIGAME_STATE) {
      minigame->mouseClick(direction, point);
   }
}

void Session::toggleDrawWorld() {
   world->drawWorld = !world->drawWorld;
}

void Session::startMinigame() {
   if (game_state == MINIGAME_STATE) {
      minigame->setGameStart(true);
   }
}

void Session::createMinigame(char* type) {
   // Which type of minigame is this?
   if (strcmp(type, SHOOTING_GALLERY) == 0) {
      window->showMouse();
      minigame->shootingGallery = new ShootingGallery(shaders[SHADER_DEFAULT]->getPID(), sound);
   } else if (strcmp(type, WATERMELON_SMASH) == 0) {
      window->showMouse();
      minigame->watermelonSmash = new WatermelonSmash(shaders[SHADER_DEFAULT]->getPID(), shaders[SHADER_BILLBOARD], camera, sound);
   } else if (strcmp(type, KARAOKE) == 0) {
      window->showMouse();
      minigame->karaoke = new Karaoke(shaders[SHADER_DEFAULT]->getPID(), sound, camera, shaders[SHADER_BILLBOARD]);
   } else if (strcmp(type, SHOP) == 0) {
      window->showMouse();
      minigame->shop = new Shop(shaders[SHADER_DEFAULT]->getPID(), sound, camera);
   }
}

void Session::enterMinigame() {
   Booth* booth = world->currentActiveBooth();
   if (booth != NULL && booth->getMinigame() != NULL && strcmp(booth->getMinigame(), NO_GAME) != 0) {
      game_state = MINIGAME_STATE;
      world->inMiniGame();
      camera->moveToMinigame();
      minigame->clearMinigames();
      createMinigame(booth->getMinigame());
   }
}

void Session::leaveMinigame() {
   game_state = WORLD_STATE;
   world->leftMiniGame();
   camera->moveToOverworld();
   window->hideMouse();
   minigame->clearMinigames();
}

void Session::mouseMove(double xpos, double ypos, int width, int height) {
   if (game_state == MINIGAME_STATE) {
      minigame->mouseMove(xpos, ypos, width, height);
   }
}
