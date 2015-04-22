#ifndef __Player__
#define __Player__

#include "includes.h"
#include "Camera.h"
#include "Object.h"

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

   private:
      glm::vec3 calculatePos();
};

#endif
