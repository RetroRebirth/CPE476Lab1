#include "Sound.h"

Sound::Sound() {
}

Sound::~Sound() {
//   engine->drop();
}

void Sound::initSound() {
   return;

   engine = createIrrKlangDevice();

   if (engine == NULL) {
      exit(-1);
   }
}

void Sound::playBackgroundMusic() {
   return;

   // play some sound stream, looped
   engine->play2D("41Moonsetter.wav", true);
}

void Sound::playContactSound() {
   return;

   engine->play2D("bell.wav", false);
}

