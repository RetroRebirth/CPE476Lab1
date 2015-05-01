#ifndef __Player__
#define __Player__

#include "includes.h"
#include "Camera.h"
#include "Object.h"

// player class (used in 3rd person camera)
// hit detection will be sphere based for simplification
class Player {
   public:
      Object *object;
      Camera *camera;
      float radius;

      Player(
         Camera *_camera);
      virtual ~Player();

      void initPlayer(Object *_object);
      void step();
      
      // calculate the bounding radius for the player... should only be called once.
      void calulateRadius();

   private:
      glm::vec3 calculatePos();
};

#endif
