#include "Clicks.h"

using namespace std;

Clicks::Clicks()
{
  objects = NULL;
  direction = glm::vec3(0, 0, 0);
}

Clicks::~Clicks(){
}

//This gets the clicked objects and clears the current set of clicked objects
vector<Object*> Clicks::getClickedObjects(){
  vector<Object*> forReturning = clickedObjects;
  clickedObjects.clear();
  return forReturning;
}

//Sets the objects for clicks to check against
void Clicks::setObjects(vector<Object*>* bob){
  objects = bob;
}

//This is what main calls for click call back this is also where the objects get inserted into clickedObjects
void Clicks::mouse_click (int mouse_x, int mouse_y, int height, int width, glm::mat4 position, glm::mat4 view, glm::vec3 o){
  float x = (2.0f * mouse_x) / width - 1.0f;
  float y = 1.0f - (2.0f * mouse_y) / height;
  //The z component is negative because we usually want it moving into the screen from where we are instead of back.
  glm::vec4 ray_clip = glm::vec4 (x, y, -1.0f, 1.0);
  glm::vec4 ray_eye = glm::inverse(position) * ray_clip;
  ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);
  glm::vec4 ray_hol = (glm::inverse(view) * ray_eye);
  direction = glm::vec3(ray_hol.x, ray_hol.y, ray_hol.z);
  direction = glm::normalize (direction);

  if(objects != NULL){
    //Information to start adding to the clickedObjects array.
    for(vector<Object *>::iterator count = objects->begin(); count != objects->end(); ++count){
      if(checkAgainstSphere(*count, o)){
      clickedObjects.push_back(*count);
      }
    }
  }
}

//Used to check against spheres. Give in the object and the o that it is provided into mouse_click
bool Clicks::checkAgainstSphere(Object *object, glm::vec3 o){
  glm::vec3 objectCenter = object->getPos();
  glm::vec3 infoHold = glm::vec3(o.x - objectCenter.x, o.y - objectCenter.y, o.z - objectCenter.z);
  float b = glm::dot(direction, infoHold);
  float c = glm::dot(infoHold, infoHold) - (object->getRadius() * object->getRadius());
    float t1 = -b - sqrt(b * b - c);
    float t2 = -b + sqrt(b * b - c);
    if (fmin(t1, t2) > 0){
    return true;
  }else{
    return false;
  }
}
//Gets the direction of the last click so it can be drawn in minigames that want that kind of thing. It also clears out direction afterwards so the same line won't be drawn twice.
glm::vec3 Clicks::getDirection(){
  glm::vec3 holder = direction;
  direction = glm::vec3(NULL, NULL, NULL);
  return holder;
}

