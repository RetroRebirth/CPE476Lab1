#include "Minigame.h"

Minigame::Minigame() {
   shootingGallery = NULL;
   watermelonSmash = NULL;
   karaoke = NULL;
   shop = NULL;
}

Minigame::~Minigame() {
   if (shootingGallery != NULL)
      delete shootingGallery;
   if (watermelonSmash != NULL)
      delete watermelonSmash;
   if (karaoke != NULL)
      delete karaoke;
   if (shop != NULL)
      delete shop;
}

void Minigame::step(Window* window) {
   if (shootingGallery != NULL)
      shootingGallery->step(window);
   if (watermelonSmash != NULL)
      watermelonSmash->step(window);
   if (karaoke != NULL)
      karaoke->step(window);
   if (shop != NULL)
      shop->step(window);
}

void Minigame::mouseClick(glm::vec3 direction, glm::vec4 point) {
   if (shootingGallery != NULL)
      shootingGallery->mouseClick(direction, point);
   if (watermelonSmash != NULL)
      watermelonSmash->mouseClick(direction, point);
}

void Minigame::mouseMove(double xpos, double ypos, int width, int height) {
    if (shootingGallery != NULL)
        shootingGallery->mouseMove(xpos, ypos, width, height);
}

void Minigame::arrowPress(int key) {
    if (karaoke != NULL) {
        // still choosing a karaoke song
        if (!karaoke->songChosen) {
            if (key == GLFW_KEY_UP)
                karaoke->increaseDifficulty();
            else if (key == GLFW_KEY_DOWN)
                karaoke->decreaseDifficulty();
            else if (key == GLFW_KEY_LEFT)
                karaoke->prevSong();
            else if (key == GLFW_KEY_RIGHT)
                karaoke->nextSong();
        }
        // selecting an arrow
        else if (!karaoke->gameOver) {
            if (key == GLFW_KEY_UP)
                karaoke->selectCharacter(1);
            else if (key == GLFW_KEY_DOWN)
                karaoke->selectCharacter(2);
            else if (key == GLFW_KEY_LEFT)
                karaoke->selectCharacter(0);
            else if (key == GLFW_KEY_RIGHT)
                karaoke->selectCharacter(3);
        }
    }
    if (shop != NULL) {
         if (key == GLFW_KEY_UP)
             shop->nextCategory();
         else if (key == GLFW_KEY_DOWN)
             shop->prevCategory();
         else if (key == GLFW_KEY_LEFT)
             shop->prevItem();
         else if (key == GLFW_KEY_RIGHT)
             shop->nextItem();
    }
}

void Minigame::setGameStart(bool _gameStart) {
   if (shootingGallery != NULL)
      shootingGallery->gameStart = _gameStart;
   if (watermelonSmash != NULL)
      watermelonSmash->gameStart = _gameStart;
   if (karaoke != NULL)
      karaoke->selectSong();
   if (shop != NULL) {
      if (!shop->gameStart)
         shop->gameStart = _gameStart;
      else
         shop->selectItem();
   }
}

bool Minigame::getGameOver() {
   if (shootingGallery != NULL)
      return shootingGallery->gameOver;
   if (watermelonSmash != NULL)
      return watermelonSmash->gameOver;
   if (karaoke != NULL)
      return karaoke->gameOver;

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
   if (karaoke != NULL) {
      delete karaoke;
      karaoke = NULL;
   }
   if (shop != NULL) {
      delete shop;
      shop = NULL;
   }
}