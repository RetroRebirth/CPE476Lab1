#include "Sound.h"

Sound::Sound() {
   curSound = NULL;
   
   // Add the list of karaoke songs
   songs.push_back((char *)"sounds/Aldnoah_Zero_OP1.wav");
   songs.push_back((char *)"sounds/Aldnoah_Zero_OP2.wav");
   songs.push_back((char *)"sounds/Aldnoah_Zero_ED2.wav");
   songs.push_back((char *)"sounds/Angel_Beats_OP.wav");
   songs.push_back((char *)"sounds/Pokemon_OP1.wav");
   songs.push_back((char *)"sounds/Shinsekai_Yori_ED1.wav");
   songs.push_back((char *)"sounds/Shinsekai_Yori_ED2.wav");
   songs.push_back((char *)"sounds/Terror_in_Resonance_ED.wav");
   songs.push_back((char *)"sounds/Tokyo_Ghoul_OP.wav");
   songs.push_back((char *)"sounds/Tokyo_Ghoul_ED.wav");
   //songs.push_back((char *)"sounds/");
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
   if (curSound != NULL) {
      curSound->drop();
      curSound = NULL;
   }
   engine->stopAllSounds();
   
   // play some sound stream, looped
   engine->play2D("sounds/41Moonsetter.wav", true);
}

void Sound::playContactSound() {
   engine->play2D("sounds/bell.wav", false);
}

int Sound::playKaraokeMusic(int song) {
   if (curSound != NULL) {
      curSound->drop();
      curSound = NULL;
   }
   engine->stopAllSounds();
   
   curSound = engine->play2D(songs[song], false, false, true);
   return (int)(curSound->getPlayLength());
}