#include "Booth.h"

using namespace std;

Booth::Booth(string data,
             vector<tinyobj::shape_t> shapes,
             vector<tinyobj::material_t> materials,
             GLuint ShadeProg) {
    
    // load the booth
    booth[0] = new Object(shapes, materials, ShadeProg);
    booth[0]->load((char *)"objs/stall_top.obj", (char *)"objs/stall_top.mtl");
    booth[0]->setTexture(TEX_WOOD_RED);
    booth[1] = new Object(shapes, materials, ShadeProg);
    booth[1]->load((char *)"objs/stall_mid.obj", (char *)"objs/stall_mid.mtl");
    booth[1]->setTexture(TEX_WOOD_DARK);
    booth[2] = new Object(shapes, materials, ShadeProg);
    booth[2]->load((char *)"objs/stall_bottom.obj", (char *)"objs/stall_bottom.mtl");
    booth[2]->setTexture(TEX_WOOD_LIGHT);
    initBooth(data);
    
    // initiate bounding box
    booth[1]->getBounds(&bounds);
    
    bumpRadius = 1.0f;
    interactRadius = 4.0f;
    active = false;
    interactMessage = NULL;
}

void Booth::initBooth(string data) {
    char type[10];
    minigame = (char *)calloc(20, sizeof(char));
    float x0, y0, z0, x1, y1, z1, x2, y2, z2;
    float s0, s1, s2, angle;
    
    // load the booth data
    sscanf(data.c_str(), "%s (%f,%f,%f) (%f,%f,%f) (%f,%f,%f) %f %f %f %f %s\n", type, &x0, &y0, &z0, &x1, &y1, &z1, &x2, &y2, &z2, &s0, &s1, &s2, &angle, minigame);
    
    // place the booths
    booth[0]->translate(glm::vec3(x0, y0, z0));
    booth[0]->rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f));
    booth[0]->scale(glm::vec3(s0, s0, s0));
    booth[1]->translate(glm::vec3(x1, y1, z1));
    booth[1]->rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f));
    booth[1]->scale(glm::vec3(s1, s1, s1));
    booth[2]->translate(glm::vec3(x2, y2, z2));
    booth[2]->rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f));
    booth[2]->scale(glm::vec3(s2, s2, s2));
}

Booth::~Booth(){
    delete booth[0];
    delete booth[1];
    delete booth[2];
}

void Booth::draw() {
    booth[0]->draw();
    booth[1]->draw();
    booth[2]->draw();
}

//Shows the message to the screen through whatever magic we're going to use
void Booth::showMessage() {
  //TODO magic to show messages on the screen without glut.
}

//Checks if anything is colliding with the booth so it stops them.
bool Booth::checkCollision(Object* _otherObject){
   glm::vec3 max = glm::vec3(_otherObject->getPos().x + _otherObject->getRadius(), _otherObject->getPos().y + _otherObject->getRadius(), _otherObject->getPos().z + _otherObject->getRadius());
   glm::vec3 min = glm::vec3(_otherObject->getPos().x - _otherObject->getRadius(), _otherObject->getPos().y - _otherObject->getRadius(), _otherObject->getPos().z - _otherObject->getRadius());
   
   if ((max.x < bounds.x_min) || (min.x > bounds.x_max)) {
      return false;
   }
   if ((max.y < bounds.y_min) || (min.y > bounds.y_max)) {
      return false;
   }
   if ((max.z < bounds.z_min) || (min.z > bounds.z_max)) {
      return false;
   }
   
   return true;
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
//Only returns true if type is BOOTH_TYPE
bool Booth::checkInteract(glm::vec3 player_pos){
   if ((player_pos.x < influence_bounds.x_min) || (player_pos.x > influence_bounds.x_max)) {
      active = false;
      return false;
   }
   if ((player_pos.y < influence_bounds.y_min) || (player_pos.y > influence_bounds.y_max)) {
      active = false;
      return false;
   }
   if ((player_pos.z < influence_bounds.z_min) || (player_pos.z > influence_bounds.z_max)) {
      active = false;
      return false;
   }
   
   //if it gets here, player exists in bounds
   if (!active) { // anything you only want done once, put here.
      active = true;
      // debug print
      printf("Within Influece Bounds\n");
   }
   return false;
  //while(true /*The actual interact check would go here*/ ){
    //showMessage();
    //TODO check for whatever interaction
    //if(interaction is used)
    //startMinigame();
  //}
}

void Booth::calculateBoundingBox() {
   booth[1]->getBounds(&bounds);
   influence_bounds.x_min = bounds.x_min - INFLUENCE_WIDTH;
   influence_bounds.x_max = bounds.x_max + INFLUENCE_WIDTH;
   influence_bounds.y_min = bounds.y_min;
   influence_bounds.y_max = bounds.y_max;
   influence_bounds.z_min = bounds.z_min - INFLUENCE_WIDTH;
   influence_bounds.z_max = bounds.z_max + INFLUENCE_WIDTH;
}


//Sets the position for the booth
void Booth::setPosition(glm::vec3 position){
    for (int i = 0; i < 3; i++) {
        booth[i]->setPos(position);
    }
}

// Return the minigame associated with this booth
char* Booth::getMinigame() {
   return minigame;
}