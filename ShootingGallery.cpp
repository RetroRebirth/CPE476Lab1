#include "ShootingGallery.h"

ShootingGallery::ShootingGallery(GLuint _ShadeProg, Clicks* _clicks) {
   ShadeProg = _ShadeProg;
   clicks = _clicks;

   newTarget();

   // Create a wall in the back
   wall = new Object(shapes, materials, ShadeProg);
   wall->load("cube.obj");
   wall->setPos(glm::vec3(0.0, 0.0, 15.0));
   wall->scale(glm::vec3(100.0f, 100.0f, 1.0f));
   wall->setTexture(TEX_WOOD_WALL);
   wall->setShadows(false);

   score = 0;
   time = 0.0;
   clicks->setObjects(&targets);
}

void ShootingGallery::newTarget(){
   // Initialize a sphere to test clicking on TODO remove
   Object* object = new Object(shapes, materials, ShadeProg);
   object->load("sphere.obj");
   object->setShadows(false);
   // Create random coordinates for the new target
   float x = 2.0 * (int)(Util::randF() * COLS) - 3.0;
   float y = 3.0 * (int)(Util::randF() * ROWS) + 1.0;
   float z = DEPTH;
//   printf("pos: %lf %lf %lf\n", x, y, z);
   object->setPos(glm::vec3(x, y, z));
   object->setTexture(TEX_WOOD_RED);
   targets.push_back(object);
}

ShootingGallery::~ShootingGallery() {
   for (int i = 0; i < targets.size(); ++i) {
      delete targets[i];
   }
   for(int i = 0; i < bullets.size(); ++i){
      delete bullets[i];
   }
}

void ShootingGallery::step(Window* window) {
   vector<Object*> clickedObjects = clicks->getClickedObjects();

/*
   // Adds a new target every second
   if (window->time - time >= 1.0) {
   newTarget();
   time = window->time;
   }
*/
   // Record score for hit targets
   for (int i = 0; i < clickedObjects.size(); ++i) {
/*
      delete clickedObjects[i];
      clickedObjects.erase(clickedObjects.begin() + i);
*/
      // Remove the old target
      clickedObjects[i]->setPos(glm::vec3(0.0, 0.0, -1.0));

      // Add a new target
      newTarget();

      printf("Hit a target! Score: %d\n", ++score);
   }

   // Draw the targets
   for (int i = 0; i < targets.size(); ++i) {
      if (targets[i] == NULL) {
         targets.erase(targets.begin() + i);
         --i;
         continue;
      }
      targets[i]->draw();
   }
   // Draw the bullets
   for(int i = 0; i< bullets.size(); ++i){
      if (bullets[i]->getPos().z <= DEPTH){
         if (bullets[i] != NULL) {
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

