#ifndef __Sound__
#define __Sound__

#include "includes.h"
using namespace irrklang;

#define NUM_SONGS 15

class Song {
public:
    char song_name[50];
    char song_file[50];
    float bpm;
    
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