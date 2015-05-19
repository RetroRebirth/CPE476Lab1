#ifndef __Session__
#define __Session__

#include "includes.h"
#include "Window.h"
#include "Camera.h"
#include "World.h"
#include "Clicks.h"
#include "Sound.h"

#include "ShootingGallery.h"
#include "WatermelonSmash.h"

class Session {
   public:
      Session();
      virtual ~Session();

      void run();
      Camera* getCamera();
      Window* getWindow();
      Clicks* getClicks();
      void enterMinigame();
      void leaveMinigame();
      void mouseClick(glm::vec3 direction, glm::vec4 point);
      int getGameState() { return game_state; };
      void toggleDrawWorld();
      bool gameStarted() { return game_start; }
      void startMinigame();
   
   private:
      Window* window;
      Camera* camera;
      //Text* text;
//      FT_Library ft;

      Clicks* clicks;
      Sound* sound;
      World* world;
    
      ShootingGallery* minigame;
      //WatermelonSmash* minigame;
    
      // defines current state of game... consts defined in includes.h
      int game_state;
      bool game_start;

      GLuint ShadeProg;
      GLint h_aPos;
      GLint h_aNor;
      GLint h_uP;
      GLint h_uV;
      GLint h_uM;
      GLint h_uView;
      GLint h_uAClr, h_uDClr, h_uSClr, h_uS;
      GLint h_aTexCoord, h_uTexUnit;

      /** INITIALIZING **/
      bool installShaders(const string &vShaderName, const string &fShaderName);
      void initGL();

      /** MAIN GAME LOOP **/
      void step();

      /** MISC **/
      void startMinigame(char* type);
};

#endif