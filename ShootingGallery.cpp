#include "ShootingGallery.h"

ShootingGallery::ShootingGallery(GLuint _ShadeProg, Sound* _sound) {
   ShadeProg = _ShadeProg;

   // Create a wall in the back
   gallery = new Object(shapes, materials, ShadeProg);
   gallery->load(GALLERY_FILE_NAME);
   gallery->setPos(glm::vec3(0.0f, 2.5f, 5.0f));
   gallery->scale(glm::vec3(20.0f, 20.0f, 20.0f));
   gallery->rotate(180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
   gallery->setTexture(textures[TEX_WOOD_RED]);
   gallery->setShadows(false, 0.0, 0.0);
   
   backdrop = new Object(shapes, materials, ShadeProg);
   backdrop->load(BACKDROP_FILE_NAME);
   backdrop->setPos(glm::vec3(0.0f, 5.0f, 10.0f));
   backdrop->scale(glm::vec3(50.0f, 75.0f, 1.0f));
   backdrop->setTexture(textures[TEX_SKY_DAY]);

   // Create a gun to shoot from
   gun = new Object(shapes, materials, ShadeProg);
   gun->load(RIFLE_FILE_NAME);
   gun->setPos(glm::vec3(0.0, 1.0, 1.0));
   gun->setShadows(false, 0.0, 0.0);
   gun->scale(glm::vec3(4.0f, 4.0f, 1.0f));
   gun->setTexture(textures[TEX_STEEL]);
   
   tempFix = new Object(shapes, materials, ShadeProg);
   tempFix->load(TARGET_FILE_NAME);
   tempFix->setShadows(false, 0.0, 0.0);
   tempFix->setPos(glm::vec3(0.0, 0.0, -1.0));
   xRotation = yRotation = 0.0f;

   sound = _sound;
   gameStart = gameOver = false;

   score = 0;
   elapsedTime = 0.0;
   gameOver = false;
   doneTimer = -1;
   difficulty = 0;
   changeDifficulty(0);
}

void ShootingGallery::newTarget(){
   // Initialize a target
   Object* object = new Object(shapes, materials, ShadeProg);
   object->load(TARGET_FILE_NAME);
   object->setShadows(false, 0.0, 0.0);
   // Pick a random spot from 8 places
   float x = 2.0 * (int)(Util::randF() * COLS) - 3.0;
   float y = 3.0 * (int)(Util::randF() * ROWS) + 1.0 - 10.0;
   float z = DEPTH;
   object->setPos(glm::vec3(x, y, z));
   object->rotate(-90, glm::vec3(0.0, 1.0, 0.0));
   object->setTexture(textures[TEX_TARGET]);
   // Have target pop in from bottom of screen
   object->setDir(glm::vec3(0.0, 1.0, 0.0));
   object->setSpeed(newTargetSpeed);
   object->setAccel(newTargetAccel);
   object->setChangeDir(true);
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
   delete gallery;
   delete backdrop;
   delete gun;
   delete tempFix;
}

void ShootingGallery::printInstructions() {
   ifstream instrFile;
   instrFile.open("sginstr.txt");
   string line;
   float yPos = .55;
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
    gallery->draw();
    backdrop->draw();
    gun->draw();
    
    if (!gameStart) {
        printInstructions();
        
        char diff1[40];
        sprintf(diff1, "Difficulty Mode:");
        fontEngine->useFont("chunk5", 35);
        fontEngine->display(glm::vec4(1.0, 1.0, 1.0, 1.0), diff1, 0-fontEngine->getTextWidth(diff1)/2.0, .85);
        
        char diff2[20];
        if (difficulty == 0) {
           sprintf(diff2, "Easy");
        }
        else if (difficulty == 1) {
           sprintf(diff2, "Intermediate");
        }
        else if (difficulty == 2) {
           sprintf(diff2, "Hard");
        }
        else {
           sprintf(diff2, "Extreme");
        }   

        fontEngine->display(glm::vec4(1.0, 1.0, 1.0, 1.0), diff2, 0-fontEngine->getTextWidth(diff2)/2.0, .85-fontEngine->getTextHeight(diff2)*1.3);
        
        return;
    }
    if (gameOver) {
        float yPos = .3;
        float yInc;

        char ln1[30];
        
        if (ammo == 0) {
           sprintf(ln1, "You're out of ammo!");
        }
        else {
           sprintf(ln1, "You're out of time!");
        }
           
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
   
   if (ammo <= 0) {
      global_points += score * targetPoints;
      gameOver = true;
   }
   // Decrement the time limit if not set to unlimited (< 0)
   else if (timeLimit > 0) {
      timeLimit -= window->dt;
      // If the time limit ran out, begin leaving the minigame
      if (timeLimit <= 0) {
         gameOver = true;
         global_points += score * targetPoints;
      }
   }

   // Adds a new target every amount of time
   if (window->time - elapsedTime >= newTargetTime) {
      newTarget();
      elapsedTime = window->time;
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
      // If the bullet hasn't gone past the targets OR hasn't fallen off screen, do stuff with it
      if (bullets[i]->getPos().z <= 10.0 && bullets[i]->getPos().y >= -10){
         bullets[i]->setPos(bullets[i]->calculateNewPos(window->dt));
         bullets[i]->draw();
         // Check collision of bullet against targets
         // TODO use spatial data structure
         for (int j = 0; j < targets.size(); ++j) {
            if (bullets[i]->collidedWithObj(*targets[j], window->dt)) {
               sound->playThwackSound();
               // Increment score
               score++;
               // Remove the target
               targets.erase(targets.begin() + j);
               --j;
               // Remove the bullet
               bullets.erase(bullets.begin() + i);
               --i;
               break;
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
   int timeLim = (int)timeLimit;

   char time[40];
   sprintf(time, "Time remaining: 0:%d", timeLim);
   fontEngine->useFont("chunk5", 25);
   fontEngine->display(glm::vec4(1.0, 1.0, 1.0, 1.0), time, 0-fontEngine->getTextWidth(time)/2.0, yPos);
   yInc = fontEngine->getTextHeight(time) * 1.3;

   char diff[25];
   
   if (difficulty == 0) {
      sprintf(diff, "Easy Mode");
   }
   else if (difficulty == 1) {
      sprintf(diff, "Intermediate Mode");
   }
   else if (difficulty == 2) {
      sprintf(diff, "Hard Mode");
   }
   else {
      sprintf(diff, "Extreme Mode");
   }   
   fontEngine->display(glm::vec4(1.0, 1.0, 1.0, 1.0), diff, 0-fontEngine->getTextWidth(diff)/2.0, yPos - yInc);
   
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
   glm::vec3 gunPos = gun->getPos();
   glm::vec3 clickPt = glm::vec3(point.x, point.y - 7.5, DEPTH);
   glm::vec3 gunDir = clickPt - gunPos;
   
   // Shoot a bullet
   Object* bullet = new Object(shapes, materials, ShadeProg);
   bullet->load("sphere.obj");
//   bullet->setPos(glm::vec3(gunPos.x + xRotation/57.0, gunPos.y - yRotation/33.0, gunPos.z + 1.0));
   bullet->setPos(glm::vec3(point.x, point.y-7.5, 0.0));
   bullet->setDir(direction);
   bullet->setTexture(textures[TEX_WOOD_WALL]);
   bullet->setShadows(false, 0.0, 0.0);
   bullet->setSpeed(30.0f);
   bullet->setAccel(0.0f);
   bullet->scale(glm::vec3(0.25, 0.25, 0.25));
   bullets.push_back(bullet);
   
   // Decrement ammo
   ammo--;
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
   xRotation = (-45.0 * x);
   gun->iterativeRotate(xRotation, glm::vec3(0.0, 1.0, 0.0));
   yRotation = -30.0 * y;
   gun->iterativeRotate(yRotation, glm::vec3(1.0, 0.0, 0.0));
}

void ShootingGallery::changeDifficulty(int delta) {
   int newDiff = difficulty + delta;

   if (gameStart || newDiff < 0 || newDiff > 3)
      return;

   difficulty = newDiff;

printf("difficulty is now %d\n", difficulty);

   switch (difficulty) {
      case 0: // EASY
         ammo = 20;
         timeLimit = 30.0f;
         newTargetTime = 3.0f;
         newTargetSpeed = 10.0f;
         newTargetAccel = -4.5f;
         targetPoints = 10;
         break;
      case 1: // MEDIUM
         ammo = 30;
         timeLimit = 45.0f;
         newTargetTime = 2.0f;
         newTargetSpeed = 15.0f;
         newTargetAccel = -10.5f;
         targetPoints = 20;
         break;
      case 2: // HARD
         ammo = 60;
         timeLimit = 60.0f;
         newTargetTime = 1.0f;
         newTargetSpeed = 20.0f;
         newTargetAccel = -18.0f;
         targetPoints = 50;
         break;
      case 3: // EXTREME
         ammo = 60;
         timeLimit = 30.0f;
         newTargetTime = 0.5f;
         newTargetSpeed = 20.0f;
         newTargetAccel = -18.0f;
         targetPoints = 50;
         break;
   }
}
