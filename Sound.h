#ifndef __Sound__
#define __Sound__

#include "includes.h"
#include "Texture.h"
using namespace irrklang;

class Song {
public:
    char song_name[50];
    char song_file[50];
    char img_file[50];
    float bpm;
    int price;
    bool unlocked;
    
private:
    
};

class Sound {
public:
    Sound();
    virtual ~Sound();
    
    ISoundEngine* engine;
    
    void initSound();
    void playBackgroundMusic();
    float playKaraokeMusic(int song);
    
    int getSongDuration();
    Song getSongInfo(int song) { return karaoke_songs[song]; }
    vector<Song> getSongs()    { return karaoke_songs;       }
    void unlockSong(int song)  { karaoke_songs[song].unlocked = true; }

    void playContactSound();
    void playBuzzerSound();
    void playThwackSound();
    void playSplatSound();
    void playJumpSound();
    
private:
    vector<Song> karaoke_songs;
    ISound* curSound;
};

#endif