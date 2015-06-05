#include "ShootingGallery.h"

ShootingGallery::ShootingGallery(GLuint _ShadeProg, Sound* _sound) {
   ShadeProg = _ShadeProg;

   // Create a wall in the back
   wall = new Object(shapes, materials, ShadeProg);
   wall->load("cube.obj");
   wall->setPos(glm::vec3(0.0, 0.0, 30.0));
   wall->scale(glm::vec3(100.0f, 100.0f, 1.0f));
   wall->setTexture(TEX_WOOD_WALL);
   wall->setShadows(false, 0.0, 0.0);

   // Create a gun to shoot from
   gun = new Object(shapes, materials, ShadeProg);
   gun->load("objs/rifle.obj");
   gun->setPos(glm::vec3(0.0, 1.0, 1.0));
   gun->setShadows(false, 0.0, 0.0);
   gun->scale(glm::vec3(4.0f, 4.0f, 1.0f));
   gun->setTexture(TEX_STEEL);

   tempFix = new Object(shapes, materials, ShadeProg);
   tempFix->load("objs/target.obj");
   tempFix->setShadows(false, 0.0, 0.0);
   tempFix->setPos(glm::vec3(0.0, 0.0, -1.0));
   
   xRotation = yRotation = 0.0f;

   sound = _sound;
   gameStart = gameOver = false;

   score = 0;
   elapsedTime = 0.0;
   gameOver = false;
   doneTimer = -1;
   ammo = 20;
   timeLimit = -1; // Unlimited

}

void ShootingGallery::newTarget(){
   // Initialize a target
   Object* object = new Object(shapes, materials, ShadeProg);
   object->load("objs/target.obj");
   object->setShadows(false, 0.0, 0.0);
   // Pick a random spot from 8 places
   float x = 2.0 * (int)(Util::randF() * COLS) - 3.0;
   float y = 3.0 * (int)(Util::randF() * ROWS) + 1.0 - 10.0;
   float z = DEPTH;
//   printf("pos: %lf %lf %lf\n", x, y, z);
   object->setPos(glm::vec3(x, y, z));
   object->rotate(-90, glm::vec3(0.0, 1.0, 0.0));
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
   delete wall;
   delete gun;
   delete tempFix;
}

void ShootingGallery::printInstructions() {
   ifstream instrFile;
   instrFile.open("sginstr.txt");
   string line;
   float yPos = .45;
   float yInc;
   
   fontEngine->useFont("chunk5", 35);
   yInc = fontEngine->getTextHeight("blank") * 1.3;
   
   if (instrFile.is_open()) {
      while (getline(instrFile, line)) {
         if (line[0] == '\n') {
            yPos -= yInc;
         }
         
         yPos -= yInc;
         fontEngine->display(glm::vec4(1.0, 1.0, 1.0, 1.0), line.c_str(), 0-fontEngine->getTextWidth(line.c_str())/2.0, yPos);
      }
   }
   else {
      printf("file 'sginstr.txt' was not available or could not be opened\n");
   }
}

void ShootingGallery::step(Window* window) {
    tempFix->draw();
    wall->draw();
    gun->draw();
    
    if (!gameStart) {
        // DISPLAY INSTRUCTIONS
        printInstructions();
        
        return;
    }
    if (gameOver) {
        float yPos = .3;
        float yInc;

        char ln1[20];
        sprintf(ln1, "You're out of ammo!");
        fontEngine->useFont("chunk5", 40);
        fontEngine->display(glm::vec4(1.0, 1.0, 1.0, 1.0), ln1, 0-fontEngine->getTextWidth(ln1)/2.0, yPos);
        yInc = fontEngine->getTextHeight(ln1);
        yPos -= (yInc * 2);        

        char ln2[15];
        sprintf(ln2, "Score: %d", score);
        fontEngine->display(glm::vec4(1.0, 1.0, 1.0, 1.0), ln2, 0-fontEngine->getTextWidth(ln2)/2.0, yPos);

        yPos -= (2 * yInc);        
        
        char ln3[20];
        sprintf(ln3, "Press ENTER to exit.");
        fontEngine->display(glm::vec4(1.0, 1.0, 1.0, 1.0), ln3, 0-fontEngine->getTextWidth(ln3)/2.0, yPos);

        return;
    }
    
   // Decrement the done timer if it has been set
   if (doneTimer > 0) {
      doneTimer -= window->dt;
      // If our finished game timer is done, exit the minigame
      if (doneTimer <= 0) {
          if (!gameOver)
              global_points += score;
          gameOver = true;
      }
   }

   // Decrement the time limit if not set to unlimited (< 0)
   if (timeLimit > 0) {
      timeLimit -= window->dt;
      // If the time limit ran out, begin leaving the minigame
      if (timeLimit <= 0) {
         finished();
      }
   }

   // TODO seed with system time
//   srand(window->time);

   // Adds a new target every amount of time
   if (window->time - elapsedTime >= 2.0) {
      newTarget();
      elapsedTime = window->time;
   }
/*
   // Record score for hit targets
   vector<Object*> clickedObjects = clicks->getClickedObjects();
   for (int i = 0; i < clickedObjects.size(); ++i) {
      delete clickedObjects[i];
      clickedObjects.erase(clickedObjects.begin() + i);
      // Remove the old target
      clickedObjects[i]->setPos(glm::vec3(0.0, 0.0, -1.0));

      // Add a new target
      newTarget();

      printf("Hit a target! Score: %d\n", ++score);
   }
*/
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
      // If the bullet hasn't gone past the targets OR hasn't fallen off screen, do stuff with it
      if (bullets[i]->getPos().z <= DEPTH && bullets[i]->getPos().y >= -10){
         if (bullets[i] != NULL) {
            bullets[i]->setPos(bullets[i]->calculateNewPos(window->dt));
            bullets[i]->draw();
            // Check collision of bullet against targets
            // TODO use spatial data structure
            for (int j = 0; j < targets.size(); ++j) {
               if (bullets[i]->collidedWithObj(*targets[j], window->dt)) {
                  sound->playThwackSound();
                  // Remove the target
                  targets.erase(targets.begin() + j);
                  --j;
                  // Pop bullet up then drop
                  bullets[i]->setDir(glm::vec3(0.0, 1.0, 0.0));
                  bullets[i]->setSpeed(1.0);
                  bullets[i]->setAccel(-10.0);
                  score++;
               }
            }
         }
      } else {
         // Remove the bullet if it has gone past the targets OR fallen off screen
         bullets.erase(bullets.begin() + i);
         --i;
      }
   }
   
   textStep(window);
}

void ShootingGallery::textStep(Window* window) {
   float yPos = .9;
   float yInc;

   char time[40];
   sprintf(time, "Time remaining: 0:%d / 0:30", 10000);
   fontEngine->useFont("chunk5", 25);
   fontEngine->display(glm::vec4(1.0, 1.0, 1.0, 1.0), time, 0-fontEngine->getTextWidth(time)/2.0, yPos);
   yInc = fontEngine->getTextHeight(time) * 1.3;

   char scrStr[15];
   sprintf(scrStr, "Score: %d", score);
   fontEngine->display(glm::vec4(1.0, 1.0, 1.0, 1.0), scrStr, 1-fontEngine->getTextWidth(scrStr)-.07, yPos);

   char ammoStr[15];
   sprintf(ammoStr, "Ammo: %d", ammo);
   fontEngine->display(glm::vec4(1.0, 1.0, 1.0, 1.0), ammoStr, 1-fontEngine->getTextWidth(ammoStr)-.07, yPos - yInc);
}

void ShootingGallery::mouseClick(glm::vec3 direction, glm::vec4 point) {
   // Only shoot a bullet if we have the ammo
   if (ammo <= 0 || !gameStart) {
      return;
   }
   // Shoot a bullet
   Object* bullet = new Object(shapes, materials, ShadeProg);
   bullet->load("sphere.obj");
   bullet->setPos(glm::vec3(point.x, point.y - 7.5, 0));
   bullet->setDir(direction);
   bullet->setSpeed(1.0f);
   bullet->setTexture(TEX_WOOD_WALL);
   bullet->setShadows(false, 0.0, 0.0);
   bullet->setSpeed(BULLET_SPD);
   bullet->scale(glm::vec3(0.2, 0.2, 0.2));
   bullets.push_back(bullet);
   // Decrement and print ammo
   ammo--;
   // If run out of ammo, set a timer for the game to finish
   if (ammo <= 0) {
      finished();
   }
}

/**
 * The game is finished. Set a delay timer for kicking the player out of the minigame.
 */
void ShootingGallery::finished() {
   doneTimer = 2.0f; // 2 seconds
   // Tell the user the game is done and we will be exiting
}

void ShootingGallery::exit() {
   gameOver = true;
   // TODO record score for buying prizes later?
}

void ShootingGallery::mouseMove(double xpos, double ypos, int width, int height) {
   // TODO move gun
   // Measure mouse x position from center -1.0 to 1.0
   double x  = (xpos - (width / 2.0)) / (width / 2.0);
   // Measure mouse y position from near bottom 0.0 to 1.0
   double y  = 1.0 - (ypos / (0.8 * height));

   // Rotate gun to follow mouse (binded to 45 degrees)
   gun->iterativeRotate(0.0f, glm::vec3(0.0, 0.0, 0.0));
   double rotateY = (-45.0 * x);
   gun->iterativeRotate(rotateY, glm::vec3(0.0, 1.0, 0.0));
   double rotateX = -30.0 * y;
   gun->iterativeRotate(rotateX, glm::vec3(1.0, 0.0, 0.0));
}

