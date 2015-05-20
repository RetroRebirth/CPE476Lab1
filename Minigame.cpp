#include "Minigame.h"

Minigame::Minigame() {
   shootingGallery = NULL;
   watermelonSmash = NULL;
}

Minigame::~Minigame() {
   if (shootingGallery != NULL)
      delete shootingGallery;
   if (watermelonSmash != NULL)
      delete watermelonSmash;
}

void Minigame::step(Window* window) {
   if (shootingGallery != NULL)
      shootingGallery->step(window);
   if (watermelonSmash != NULL)
      watermelonSmash->step(window);
}

void Minigame::mouseClick(glm::vec3 direction, glm::vec4 point) {
   if (shootingGallery != NULL)
      shootingGallery->mouseClick(direction, point);
   if (watermelonSmash != NULL)
      watermelonSmash->mouseClick(direction, point);
}

void Minigame::setGameStart(bool _gameStart) {
   if (shootingGallery != NULL)
      shootingGallery->gameStart = _gameStart;
   if (watermelonSmash != NULL)
      watermelonSmash->gameStart = _gameStart;
}

bool Minigame::getGameOver() {
   if (shootingGallery != NULL)
      return shootingGallery->gameOver;
   if (watermelonSmash != NULL)
      return watermelonSmash->gameOver;

   return false;
}

void Minigame::clearMinigames() {
   if (shootingGallery != NULL) {
      delete shootingGallery;
      shootingGallery = NULL;
   }
   if (watermelonSmash != NULL) {
      delete watermelonSmash;
      watermelonSmash = NULL;
   }
}

