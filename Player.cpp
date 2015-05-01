#include "Player.h"

Player::Player(
      Camera *_camera) {

   object = NULL;
   camera = _camera;
   radius = 1.0;
}

Player::~Player() {
   delete object;
}

void Player::initPlayer(Object *_object) {
   object = _object;
}

void Player::step() {
   float Yrot = camera->getYRot();

   if (object != NULL) {
      object->setPos(calculatePos());
      object->scale(glm::vec3(1.0, 2.0, 1.0));
      object->setPos(object->getPos());
      object->rotate(-Yrot, glm::vec3(0, 1, 0));
      
      if (camera->pov) 
         object->draw();
   }
}

glm::vec3 Player::calculatePos() {
   glm::vec3 pos;
   float Yrot = camera->getYRot();

   pos.x = camera->pos.x - radius * sin((Yrot + 270.0f) * M_PI / 180.0);
   pos.y = 1;
   pos.z = camera->pos.z - radius * cos((Yrot - 270.0f) * M_PI / 180.0);

   return pos;
}
