#include "Sound.h"

Sound::Sound() {
   curSound = NULL;
   
   // Load the karaoke songs from the file
   ifstream songFile;
   songFile.open("songs.txt");
   string line;
   if (songFile.is_open()) {
      while (getline(songFile, line)) {
         // Skip comments
         if (line[0] == '#')
            continue;
         
         // Load the song data
         Song newSong;
         sscanf(line.c_str(), "%s %s %s %f %d\n", newSong.song_name, newSong.song_file, newSong.vid_file, &(newSong.bpm), &(newSong.price));
         newSong.unlocked = (newSong.price == 0);
         
         // Replace all '.' to ' ' in the song name
         for (int i = 0; i < strlen(newSong.song_name); i++) {
            if (newSong.song_name[i] == '.')
               newSong.song_name[i] = ' ';
         }
         karaoke_songs.push_back(newSong);
      }
      songFile.close();
   }
   else {
      printf("file 'songs.txt' was not available or could not be opened\n");
   }
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
   engine->play2D("sounds/Okami_CherryBlossomStorm.wav", true);
}

void Sound::pauseSong() {
    engine->stopAllSounds();
}

void Sound::playJumpSound() {
    engine->play2D("sounds/jump.wav", false);
}
void Sound::playContactSound() {
    engine->play2D("sounds/bell.wav", false);
}
void Sound::playBuzzerSound() {
   engine->play2D("sounds/buzz.wav", false);
}
void Sound::playThwackSound() {
   engine->play2D("sounds/thud.wav", false);
}
void Sound::playSplatSound() {
   engine->play2D("sounds/splat.wav", false);
}
void Sound::playCorrectSound() {
    engine->play2D("sounds/correct.wav", false);
}
void Sound::playIncorrectSound() {
    engine->play2D("sounds/incorrect.wav", false);    
}
void Sound::playSoftDing() {
    engine->play2D("sounds/ding_soft.wav", false);
}
void Sound::playLoudDing() {
    engine->play2D("sounds/ding_high.wav", false);
}


/* Plays the given karaoke track and return its beats per minute */
float Sound::playKaraokeMusic(int song) {
   if (curSound != NULL) {
      curSound->drop();
      curSound = NULL;
   }
   engine->stopAllSounds();
   
   curSound = engine->play2D(karaoke_songs[song].song_file, false, false, true);
   return karaoke_songs[song].bpm;
}

int Sound::getSongDuration() {
   return (int)(curSound->getPlayLength());
}