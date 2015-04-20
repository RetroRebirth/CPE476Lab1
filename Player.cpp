#include "Player.h"

Player::Player(
      Camera &_camera) {

   object = NULL;
   camera = &_camera;
   radius = 1.0;
}

Player::~Player() {}

void Player::initPlayer(Object *_object) {
   object = _object;
}

void Player::draw() {
   if (object != NULL) {
      object->pos = calculatePos();

      object->draw();
   }
}

glm::vec3 Player::calculatePos() {
   pos = camera->pos;
   pos.z -= radius;   
}
   
