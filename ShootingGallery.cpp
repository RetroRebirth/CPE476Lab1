#include "ShootingGallery.h"

ShootingGallery::ShootingGallery(GLuint _ShadeProg, Clicks* _clicks) {
    ShadeProg = _ShadeProg;
    clicks = _clicks;

    // Create a wall in the back
    wall = new Object(shapes, materials, ShadeProg);
    wall->load("cube.obj");
    wall->setPos(glm::vec3(0.0, 0.0, 15.0));
    wall->scale(glm::vec3(100.0f, 100.0f, 1.0f));
    wall->setTexture(TEX_WOOD_WALL);
    wall->setShadows(false);
    
    score = 0;
    clicks->setObjects(&objects);
}

void ShootingGallery::newTarget(){
    // Initialize a sphere to test clicking on TODO remove
    Object* object = new Object(shapes, materials, ShadeProg);
    object->load("sphere.obj");
    // Create random coordinates for the new target
    float randX = Util::randF() * WIDTH - (WIDTH/2);
    float randY = Util::randF() * HEIGHT - (HEIGHT/2);
    float randZ = DEPTH;
    object->setPos(glm::vec3(randX, Util::randF() * 10, 10.0));
    object->setTexture(TEX_WOOD_RED);
    objects.push_back(object);
}

ShootingGallery::~ShootingGallery() {
   for (int i = 0; i < objects.size(); ++i) {
      delete objects[i];
   }
    for(int i = 0; i < bullets.size(); ++i){
        delete bullets[i];
    }
}

void ShootingGallery::makeBullets(){
    Object* bullet = new Object(shapes, materials, ShadeProg);
    bullet->load("sphere.obj");
    bullet->setPos(glm::vec3(0, 0, 0));
    bullet->setDir(clicks->getDirection());
    bullet->setSpeed(1.0f);
    bullet->setTexture(TEX_WOOD_WALL);
    bullets.push_back(bullet);
}

void ShootingGallery::step(Window* window) {
   vector<Object*> clickedObjects = clicks->getClickedObjects();
   for (int i = 0; i < clickedObjects.size(); ++i) {
      float randX = Util::randF() * WIDTH - (WIDTH/2);
      float randY = Util::randF() * HEIGHT - (HEIGHT/2);
      float randZ = DEPTH;
      clickedObjects[i]->setPos(glm::vec3(randX, randY, randZ)); // move the target to a random spot
      printf("Hit a target! Score: %d\n", ++score);
   }
   for (int i = 0; i < objects.size(); ++i) {
      objects[i]->draw();
   }
   for(int i = 0; i< bullets.size(); ++i){
      if (bullets[i]->getPos().z <= DEPTH){
         if(bullets[i] != NULL){
            bullets[i]->setPos(bullets[i]->calculateNewPos(BULLET_SPD*window->dt));
            bullets[i]->draw();
         }
      } else {
         // Remove the bullet if it has gone past the target
         bullets.erase(bullets.begin() + i);
         --i;
      }
   }
   wall->draw();
}

void ShootingGallery::mouseClick(glm::vec3 direction) {
   // Shoot a bullet
   Object* bullet = new Object(shapes, materials, ShadeProg);
   bullet->load("sphere.obj");
   bullet->setPos(glm::vec3(0, 2.5, 0));
   bullet->setDir(direction);
   bullet->setSpeed(1.0f);
   bullet->setTexture(TEX_WOOD_WALL);
   bullet->setShadows(false);
   bullets.push_back(bullet);
}