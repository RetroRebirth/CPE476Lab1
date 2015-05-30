#ifndef __Minigame__
#define __Minigame__

#include "includes.h"
#include "Window.h"

#include "ShootingGallery.h"
#include "WatermelonSmash.h"
#include "Karaoke.h"
#include "Shop.h"

class Minigame {
   public:
      Minigame();
      virtual ~Minigame();

      // Minigame instance objects
      ShootingGallery* shootingGallery;
      WatermelonSmash* watermelonSmash;
      Karaoke* karaoke;
      Shop* shop;

      // Public methods
      void step(Window* window);
      void mouseClick(glm::vec3 direction, glm::vec4 point);
      void mouseMove(double xpos, double ypos, int width, int height);
      void arrowPress(int key);
      void setGameStart(bool _gameStart);
      bool getGameOver();
      void clearMinigames();
   private:
};

#endif
