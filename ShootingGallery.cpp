#include "ShootingGallery.h"

ShootingGallery::ShootingGallery(GLuint _ShadeProg, Clicks* _clicks, Sound* _sound) {
   ShadeProg = _ShadeProg;
   clicks = _clicks;

   // Create a wall in the back
   wall = new Object(shapes, materials, ShadeProg);
   wall->load("cube.obj");
   wall->setPos(glm::vec3(0.0, 0.0, 30.0));
   wall->scale(glm::vec3(100.0f, 100.0f, 1.0f));
   wall->setTexture(TEX_WOOD_WALL);
   wall->setShadows(false);

   sound = _sound;
   gameOver = gameStart = false;

   score = 0;
   time = 0.0;
   clicks->setObjects(&targets);

   printf("\t\t----- Welcome to the SHOOTING GALLERY -----\n");
   printf("\t\tTest your skills by shooting your targets as they appear!\n");
   printf("\t\tOnce you are done, press SPACE to exit.\n\n");
}

void ShootingGallery::newTarget(){
   // Initialize a sphere to test clicking on TODO remove
   Object* object = new Object(shapes, materials, ShadeProg);
   object->load("objs/target.obj");
   object->setShadows(false);
   // Create random coordinates for the new target
   float x = 2.0 * (int)(Util::randF() * COLS) - 3.0;
   float y = 3.0 * (int)(Util::randF() * ROWS) + 1.0 - 10.0;
   float z = DEPTH;
//   printf("pos: %lf %lf %lf\n", x, y, z);
   object->setPos(glm::vec3(x, y, z));
   object->setTexture(TEX_TARGET);
   // Have target pop in from bottom of screen
   object->setDir(glm::vec3(0.0, 1.0, 0.0));
   object->setSpeed(20.0);
   object->setAccel(-20);
   object->setChangeDir(false);
   // Add the target to the list
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
   
   if (!gameStart || gameOver)
      return;
   vector<Object*> clickedObjects = clicks->getClickedObjects();
   srand(window->time);

   // Adds a new target every amount of time
   if (window->time - time >= 2.0) {
      newTarget();
      time = window->time;
   }

   // Draw the targets
   for (int i = 0; i < targets.size(); ++i) {
      if (targets[i] == NULL) {
         targets.erase(targets.begin() + i);
         --i;
         continue;
      }
      targets[i]->setPos(targets[i]->calculateNewPos(window->dt));
      targets[i]->draw();
   }
   // Draw the bullets
   for(int i = 0; i< bullets.size(); ++i){
      if (bullets[i]->getPos().z <= DEPTH){
         if (bullets[i] != NULL) {
            bullets[i]->setPos(bullets[i]->calculateNewPos(window->dt));
            bullets[i]->draw();
            // Check collision of bullet against targets
            // TODO use spatial data structure
            for (int j = 0; j < targets.size(); ++j) {
               if (bullets[i]->collidedWithObj(*targets[j], window->dt)) {
//                  sound->playContactSound();
                  // Remove the target
                  targets.erase(targets.begin() + j);
                  --j;
                  // Pop bullet up then drop
                  bullets[i]->setDir(glm::vec3(0.0, 1.0, -0.1));
                  bullets[i]->setSpeed(1.0);
                  bullets[i]->setAccel(-10.0);
                  // Report the score
                  printf("Hit a target! Score: %d\n", ++score);
               }
            }
         }
      } else {
         // Remove the bullet if it has gone past the target
         bullets.erase(bullets.begin() + i);
         --i;
      }
   }
   wall->draw();
}

void ShootingGallery::mouseClick(glm::vec3 direction, glm::vec4 point) {
   // Shoot a bullet
   Object* bullet = new Object(shapes, materials, ShadeProg);
   bullet->load("sphere.obj");
   bullet->setPos(glm::vec3(point.x, 2.5, 0));
   bullet->setDir(direction);
   bullet->setSpeed(1.0f);
   bullet->setTexture(TEX_WOOD_WALL);
   bullet->setShadows(false);
   bullet->setSpeed(BULLET_SPD);
   bullet->scale(glm::vec3(0.2, 0.2, 0.2));
   bullets.push_back(bullet);
}
