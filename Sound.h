#ifndef __Sound__
#define __Sound__

#include "includes.h"
using namespace irrklang;

#define NUM_SONGS 13

class Sound {
   public:
    
    Sound();
    virtual ~Sound();
    
    ISoundEngine* engine;
    
    void initSound();
    void playBackgroundMusic();
    int playKaraokeMusic(int song);

    void playContactSound();
    void playMelonSound();
    void playMelonSplat();
    
private:
    vector<char *> songs;
    ISound* curSound;
};

#endif