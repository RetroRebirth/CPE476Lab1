#ifndef __Sound__
#define __Sound__

#include "includes.h"
using namespace irrklang;

class Sound {
   public:
    
    Sound();
    virtual ~Sound();
    
    ISoundEngine* engine;
    
    void initSound();
    void playBackgroundMusic();
};
#endif
