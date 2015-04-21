#include "Booth.h"

using namespace std;

Booth::Booth(
    Object *_object,
    const string *_message) { 

  object = _object;
  interactMessage = _message;
  //can't go past this
  bumpRadius = 1.0f;
  //can only interact within this
  interactRadius = 4.0f;
  //What should be shown to screen when inside the interact radius
  //this should go away when the player leaves the interact radius
  
  // initiate bounding box
  object->getBounds(&bounds);
  
}

Booth::~Booth(){}

//Shows the message to the screen through whatever magic we're going
//to use
void Booth::showMessage(){
  //TODO magic to show messages on the screen without glut.
}

//Checks if anything is colliding with the booth so it stops them.
bool Booth::checkCollision(Object* _otherObject){
   //TODO magic collision work
   
}

// check for camera collision set colPlane equal to the bound that has been passed through
bool Booth::checkCameraCollision(glm::vec3 cam_pos, glm::vec3 *colPlane) {
   if ((cam_pos.x < bounds.x_min) || (cam_pos.x > bounds.x_max)) {
      return false;
   }
   if ((cam_pos.y < bounds.y_min) || (cam_pos.y > bounds.y_max)) {
      return false;
   }
   if ((cam_pos.z < bounds.z_min) || (cam_pos.z > bounds.z_max)) {
      return false;
   }
   
   float x_diff, y_diff, z_diff;
   x_diff = bounds.x_min - cam_pos.x;
   if (abs(bounds.x_max - cam_pos.x) < abs(x_diff)) {
      x_diff = bounds.x_max - cam_pos.x;
   }
   y_diff = bounds.y_min - cam_pos.y;
   if (abs(bounds.y_max - cam_pos.y) < abs(y_diff)) {
      y_diff = bounds.y_max - cam_pos.y;
   }
   z_diff = bounds.z_min - cam_pos.z;
   if (abs(bounds.z_max - cam_pos.z) < abs(z_diff)) {
      z_diff = bounds.z_max - cam_pos.z;
   }
   
   float diffs[3] = {x_diff, y_diff, z_diff};
   float min = abs(diffs[0]);
   glm::vec3 plane = glm::vec3(1.0f, 0.0f, 0.0f);
   for (int i=0; i<3; ++i) {
      if (abs(diffs[i]) < min) {
         min = abs(diffs[i]);
         plane = glm::vec3(0.0f);
         plane[i] = 1.0f;
      }
   }
   glm::vec3 temp_vec = glm::vec3(cam_pos.x + x_diff, cam_pos.y + y_diff, cam_pos.z + z_diff);
   plane *= temp_vec;
   colPlane->x = plane.x;
   colPlane->y = plane.y;
   colPlane->z = plane.z;
   
   return true;
}

//Checks for if the player is within the interact radius
bool Booth::checkInteract(Player *_player){
//TODO magic collision work again
  while(true /*The actual interact check would go here*/ ){
    showMessage();
    //TODO check for whatever interaction
    //if(interaction is used)
    //startMinigame();
  }
}

void Booth::calculateBoundingBox() {
   object->getBounds(&bounds);
}

//Starts the minigame
void Booth::startMinigame(){
  //Need minigame things
}

//Sets the position for the booth
void Booth::setPosition(glm::vec3 position){
  object->pos = position;

}
