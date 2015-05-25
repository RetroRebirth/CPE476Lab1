#ifndef __Sound__
#define __Sound__

#include "includes.h"
using namespace irrklang;

#define NUM_SONGS 15

class Sound {
   public:
    
    Sound();
    virtual ~Sound();
    
    ISoundEngine* engine;
    
    void initSound();
    void playBackgroundMusic();
    float playKaraokeMusic(int song);
    int getSongDuration();

    void playContactSound();
    void playBuzzerSound();
    void playThwackSound();
    void playSplatSound();
    void playJumpSound();
    
private:
    vector<char *> karaoke_songs;
    vector<float> karaoke_bpm;
    ISound* curSound;
};

#endif