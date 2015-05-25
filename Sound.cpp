#include "Sound.h"

Sound::Sound() {
   curSound = NULL;
   
   // Add the list of karaoke songs
   karaoke_songs.push_back((char *)"sounds/Aldnoah_Zero_OP1.wav");
   karaoke_bpm.push_back(157.14);
   karaoke_songs.push_back((char *)"sounds/Aldnoah_Zero_OP2.wav");
   karaoke_bpm.push_back(159.16);
   karaoke_songs.push_back((char *)"sounds/Aldnoah_Zero_ED2.wav");
   karaoke_bpm.push_back(108.94);
   karaoke_songs.push_back((char *)"sounds/Angel_Beats_OP.wav");
   karaoke_bpm.push_back(145.02);
   karaoke_songs.push_back((char *)"sounds/Pokemon_OP1.wav");
   karaoke_bpm.push_back(145.20);
   karaoke_songs.push_back((char *)"sounds/Shinsekai_Yori_ED1.wav");
   karaoke_bpm.push_back(120.01);
   karaoke_songs.push_back((char *)"sounds/Shinsekai_Yori_ED2.wav");
   karaoke_bpm.push_back(139.95);
   karaoke_songs.push_back((char *)"sounds/Terror_in_Resonance_ED.wav");
   karaoke_bpm.push_back(131.46);
   karaoke_songs.push_back((char *)"sounds/Tokyo_Ghoul_OP.wav");
   karaoke_bpm.push_back(121.59);
   karaoke_songs.push_back((char *)"sounds/Tokyo_Ghoul_ED.wav");
   karaoke_bpm.push_back(100.11);
   karaoke_songs.push_back((char *)"sounds/Howls_Moving_Castle.wav");
   karaoke_bpm.push_back(114.80);
   karaoke_songs.push_back((char *)"sounds/Princess_Mononoke.wav");
   karaoke_bpm.push_back(103.31);
   karaoke_songs.push_back((char *)"sounds/Spirited_Away.wav");
   karaoke_bpm.push_back(118.84);
   karaoke_songs.push_back((char *)"sounds/Code_Geass_ED1.wav");
   karaoke_bpm.push_back(126.06);
   karaoke_songs.push_back((char *)"sounds/Death_Note_OP2.wav");
   karaoke_bpm.push_back(135.16);
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

// Sound when a target is hit in Shooting Gallery
void Sound::playContactSound() {
   engine->play2D("sounds/bell.wav", false);
}
// Sound when an arrow is missed in Karaoke
void Sound::playBuzzerSound() {
   engine->play2D("sounds/buzz.wav", false);
}
// Sound when an arrow is hit in Karaoke
void Sound::playJumpSound() {
   engine->play2D("sounds/jump.wav", false);   
}
// Sound when a melon is hit in Watermelon Smash
void Sound::playThwackSound() {
   engine->play2D("sounds/thud.wav", false);
}
// Sound when a melon is broken in Watermelon Smash
void Sound::playSplatSound() {
   engine->play2D("sounds/splat.wav", false);
}

/* Plays the given karaoke track and return its beats per minute */
float Sound::playKaraokeMusic(int song) {
   if (curSound != NULL) {
      curSound->drop();
      curSound = NULL;
   }
   engine->stopAllSounds();
   
   curSound = engine->play2D(karaoke_songs[song], false, false, true);
   return karaoke_bpm[song];
}

int Sound::getSongDuration() {
   return (int)(curSound->getPlayLength());
}