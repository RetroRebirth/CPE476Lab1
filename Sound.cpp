#include "Sound.h"

Sound::Sound() {
}

Sound::~Sound() {
   engine->drop();
}

void Sound::initSound() {
   engine = createIrrKlangDevice();

   if (engine == NULL) {
      exit(-1);
   }
}

void Sound::playBackgroundMusic() {
   // play some sound stream, looped
   engine->play2D("41Moonsetter.wav", true);
}

void Sound::playContactSound() {
   engine->play2D("bell.wav", false);
}

