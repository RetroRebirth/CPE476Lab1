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
         sscanf(line.c_str(), "%s %s %s %s %f %d\n", newSong.song_name, newSong.song_file, newSong.vid_file, newSong.img_file, &(newSong.bpm), &(newSong.price));
         newSong.unlocked = (newSong.price == 0);
         
         // Replace all '.' to ' ' in the song name
         for (int i = 0; i < strlen(newSong.song_name); i++) {
            if (newSong.song_name[i] == '.')
               newSong.song_name[i] = ' ';
         }
         karaoke_songs.push_back(newSong);
      }
      
      // Load the textures
      for (int i = 0; i < karaoke_songs.size(); i++) {
         Texture newTex;
         newTex.loadTexture(karaoke_songs[i].img_file, i + NUM_TEXTURES, true);
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

// Sound when an arrow is hit in Karaoke
void Sound::playJumpSound() {
    engine->play2D("sounds/jump.wav", false);
}
// Sound when a perfect arrow is hit in Karaoke
void Sound::playContactSound() {
    engine->play2D("sounds/bell.wav", false);
}
// Sound when an arrow is missed in Karaoke
void Sound::playBuzzerSound() {
   engine->play2D("sounds/buzz.wav", false);
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
   
   curSound = engine->play2D(karaoke_songs[song].song_file, false, false, true);
   return karaoke_songs[song].bpm;
}

int Sound::getSongDuration() {
   return (int)(curSound->getPlayLength());
}