#ifndef __ShootingGallery__
#define __ShootingGallery__

#include "includes.h"
#include "Clicks.h"
#include "Texture.h"
#include "Window.h"
#include "Sound.h"
#include "FontEngine.h"

#define DEPTH 3.0
#define GUN_BULLET_SPD 30.0
#define ROWS 2
#define COLS 4

class ShootingGallery {
   public:
      ShootingGallery(GLuint _ShadeProg, Sound* _sound);
      virtual ~ShootingGallery();
      void mouseClick(glm::vec3 direction, glm::vec4 point);
      void mouseMove(double xpos, double ypos, int width, int height);
      void step(Window* window);

      bool gameOver, gameStart;

      void changeDifficulty(int delta);

   private:
      vector<Object*> targets;
      vector<tinyobj::shape_t> shapes;
      vector<tinyobj::material_t> materials;
      
      float xRotation, yRotation;
      
      GLuint ShadeProg;
      Sound* sound;
      Object* crosshair;
      Object* gun;
      Object* gallery;
      Object* backdrop;
      int score;
      double elapsedTime;
      void newTarget();
      vector<Object*> bullets;
      int ammo;
      float doneTimer;
      float timeLimit;
      void finished();
      void exit();
      void printInstructions();
      void textStep(Window* window);
      Object* tempFix; // Permament target that is off screen so background doesn't flicker
      int difficulty;
      float newTargetTime;
      float newTargetSpeed;
      float newTargetAccel;
};

#endif
