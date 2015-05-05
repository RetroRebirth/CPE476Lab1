#ifndef __Minigame__
#define __Minigame__

#include "includes.h"

class Minigame {
   public:
       explicit Minigame();
       virtual ~Minigame();

       virtual void step();
   private:
};

#endif
