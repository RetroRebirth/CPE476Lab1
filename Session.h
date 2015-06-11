#ifndef __Session__
#define __Session__

#include "includes.h"
#include "Window.h"
#include "Camera.h"
#include "World.h"
#include "Clicks.h"
#include "Sound.h"
#include "Minigame.h"
#include "Program.h"
#include "ParticleTexture.h"
#include "FontEngine.h"
#include "Object.h"

#define GAME 2
#define MAX_PUNS 9

class Session {
   public:
      Session();
      virtual ~Session();

      void run();
      Camera* getCamera();
      Window* getWindow();
      Clicks* getClicks();
      Minigame* getMinigame() { return minigame; }
      void enterMinigame();
      void leaveMinigame();
      void mouseClick(glm::vec3 direction, glm::vec4 point);
      void mouseMove(double xpos, double ypos, int width, int height);
      int getGameState() { return game_state; };
      void toggleDrawWorld();
      bool gameStarted() { return game_start; }
      bool gameEnded() { return minigame->getGameOver(); }
      void startMinigame();
    
   private:
      Window* window;
      Camera* camera;
      
      Clicks* clicks;
      Sound* sound;
      World* world;
    
      Minigame* minigame;
      bool game_start;
      
      float startTime;
      float xInc;
      int punLine;

      //GLuint ShadeProg;
      GLint h_aPos;
      GLint h_aNor;
      GLint h_uP;
      GLint h_uV;
      GLint h_uM;
      GLint h_uView;
      GLint h_uAClr, h_uDClr, h_uSClr, h_uS;
      GLint h_aTexCoord, h_uTexUnit;

      // OpenGL handle to texture data
      ParticleTexture texture;
      GLint h_texture0;

      /** INITIALIZING **/
      bool installShaders(Program* prog);
      void initGL();
      void initFBO();

      // Text bluh bluh I'm tired
      string getLine();

      /** MAIN GAME LOOP **/
      void step();

      /** MISC **/
      void createMinigame(char* type);
};

// contains all shaders mapped to key defined in includes.h
extern map<string, Program*> shaders;

#endif
