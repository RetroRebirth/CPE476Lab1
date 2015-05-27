#include "WatermelonSmash.h"


bool removeExplosions(vector<Particle*> p) {
   if (p[0]->cycles > 0) {
      return true;
   }
   return false;
}

WatermelonSmash::WatermelonSmash(GLuint _ShadeProg, Program* _particleProg, Camera* _camera, Sound* _sound) {
    
    // Inititalize the game
    ShadeProg = _ShadeProg;
    particleProg = _particleProg;
    camera = _camera;
    
    sound = _sound;
    score = numMelons = 0;
    timeStart = timer = timeLeft = timeRight = timeSwing = 0.0;
    spawnLeft = spawnRight = false;
    gameStart = gameOver = false;

    // Set up the booth
    setUp();
    
    // Display the game description and rules
    /*printf("\t\t----- Welcome to the WATERMELON SMASH -----\n");
    printf("\t\tSmash the watermelons as they appear by clicking them.\n");
    printf("\t\tDestroy bigger watermelons to earn more points!.\n\n");
    printf("\t\tHitting a watermelon = 1 point\n");
    printf("\t\tSmall watermelon = 10 points, 2 hits\n");
    printf("\t\tMedium watermelon = 20 points, 8 hits\n");
    printf("\t\tLarge watermelon = 30 points, 15 hits\n\n");
    printf("\t\tYou have 30 seconds to smash as many watermelons as you can!\n");
    printf("\t\tPress ENTER to start the game!.\n");
    printf("\t\tOnce you are done, press SPACE to exit.\n\n");*/   
}

WatermelonSmash::~WatermelonSmash() {
    for (int i = 0; i < bullets.size(); ++i) {
        delete bullets[i];
    }
    for (int i = 0; i < melons.size(); i++) {
        delete melons[i];
    }
    for (int i = 0; i < misc_objects.size(); i++) {
        delete misc_objects[i];
    }
}

void WatermelonSmash::setUp() {
    // Create a wall in the back
    Object *wall = new Object(shapes, materials, ShadeProg);
    wall->load("cube.obj");
    wall->setPos(glm::vec3(0.0, 0.0, 30.0));
    wall->scale(glm::vec3(100.0f, 100.0f, 1.0f));
    wall->setTexture(TEX_WOOD_LIGHT);
    wall->setShadows(false);
    misc_objects.push_back(wall);
    
    // Create a table for the watermelons
    Object *table = new Object(shapes, materials, ShadeProg);
    table->load("cube.obj");
    table->setPos(glm::vec3(0.0, 0.0, MELON_DEPTH));
    table->scale(glm::vec3(10.0f, 1.0f, 1.0f));
    table->setTexture(TEX_WOOD_DARK);
    table->setShadows(false);
    misc_objects.push_back(table);
    
    // Create booth player
    Object *npc = new Object(shapes, materials, ShadeProg);
    npc->load("bunny.obj");
    npc->setPos(glm::vec3(0.0, 0.0, MELON_DEPTH + 2));
    npc->scale(glm::vec3(2.0, 3.0, 2.0));
    npc->setTexture(TEX_LANTERN);
    npc->setShadows(false);
    misc_objects.push_back(npc);
    
    // Create the hammer
    hammer = new Object(shapes, materials, ShadeProg);
    hammer->load("objs/hammer.obj");
    hammer->setPos(glm::vec3(0.0, 3.5, MELON_DEPTH + .2));
    hammer->scale(glm::vec3(3.0, 3.0, 3.0));
    hammer->updateRadius();
    hammer->setTexture(TEX_HAMMER);
    hammer->setShadows(false);
    
    // Add watermelons
    newMelon(MELON_LEFT);
    newMelon(MELON_RIGHT);
}

void WatermelonSmash::newMelon(float xPos) {
    // Create a watermelon object
    Object *newObj = new Object(shapes, materials, ShadeProg);
    newObj->load((char *)"objs/watermelon.obj");
    newObj->setTexture(TEX_MELON_OUT);
    newObj->setShadows(false);
    
    // Add the watermelons to the game
    Watermelon *newMelon = new Watermelon(particleProg, camera, newObj, xPos);
    melons.push_back(newMelon);
}

void WatermelonSmash::checkTime(Window *window) {
    // Initialize the time if not done so already
    if (timeStart == 0.0) {
        timeStart = window->time;
        timer = window->time;
        srand(timeStart);
    }
    else {
        // Increment the timer every second
        if (window->time - timer >= 1.0) {
            printf("Time remaining: %d\n", (int)(window->time - timeStart));
            timer = window->time;
        }
        // Check whether the game has ended
        if (window->time - timeStart >= MELON_TIME) {
            printf("Time's up! Your score is: %d\n", score);
            printf("You smashed %d watermelons!\n", numMelons);
            gameOver = true;
        }
        // Spawn a watermelon
        if (window->time - timeLeft >= MELON_SPAWN && spawnLeft) {
            newMelon(MELON_LEFT);
            spawnLeft = false;
        }
        if (window->time - timeRight >= MELON_SPAWN && spawnRight) {
            newMelon(MELON_RIGHT);
            spawnRight = false;
        }
        // Swing the hammer
        if (timeSwing - window->time > 0) {
            float angle = -360 * (timeSwing - window->time);
            hammer->rotate(angle, glm::vec3(1.0, 0.0, 0.0));
        }
    }
}

void WatermelonSmash::step(Window* window) {
    // Draw the booth
    for (int i = 0; i < misc_objects.size(); i++) 
        misc_objects[i]->draw();
    // Check how much time has passed and whether game is playing
    if (gameOver || !gameStart)
        return;
    checkTime(window);
    
    // Draw the watermelons and hammer
    for (int i = 0; i < melons.size(); i++) {
        melons[i]->object->draw();
    }
    hammer->draw();
    
    // Fire the bullets
    for (int i = 0; i < bullets.size(); i++){
        if (bullets[i]->getPos().z <= MELON_DEPTH && bullets[i]->getPos().z >= -MELON_DEPTH) {
            if (bullets[i] != NULL) {
                bullets[i]->setPos(bullets[i]->calculateNewPos(window->dt));
                
                // Check collision against watermelons
                for (int j = 0; j < melons.size(); ++j) {
                    if (bullets[i]->collidedWithObj(*melons[j]->object, window->dt)) {
                        // Hit the melon
                        timeSwing = window->time + MELON_SWING;
                        hammer->setPos(glm::vec3(melons[j]->xPos, melons[j]->yPos + melons[j]->size * 1.0, MELON_DEPTH + .2));
                        int pointsEarned = melons[j]->hit();
                        score += pointsEarned;
                        
                        // Remove the melon if it was destroyed
                        if (pointsEarned >= 10) {
                            if (melons[j]->xPos == MELON_LEFT) {
                                spawnLeft = true;
                                timeLeft = window->time;
                            }
                            if (melons[j]->xPos == MELON_RIGHT) {
                                spawnRight = true;
                                timeRight = window->time;
                            }
                            numMelons++;
                            melons.erase(melons.begin() + j--);
                            sound->playSplatSound();
                        }
                        else
                            sound->playThwackSound();
                    }
                }
            }
        }
        // Remove the bullet if it has gone past the target
        else
            bullets.erase(bullets.begin() + i--);
    }
    
            
    // Draw the HUD
    char scrStr[15];
    sprintf(scrStr, "Score: %d", score);
    
    
		      
    for (int i = 0; i < melons.size(); i++) {
        melons[i]->explosionsStarted.erase(std::remove_if(melons[i]->explosionsStarted.begin(), 
                     melons[i]->explosionsStarted.end(),
                     &removeExplosions),
                     melons[i]->explosionsStarted.end());
        melons[i]->particleStep();
    }
    fontEngine->display(glm::vec4(0.98, 0.5, 0.48, 1.0), 2, 30, scrStr, 0.55, 0.85);

}

void WatermelonSmash::mouseClick(glm::vec3 direction, glm::vec4 point) {
    // Shoot a "bullet"
    Object* bullet = new Object(shapes, materials, ShadeProg);
    bullet->load("sphere.obj");
    bullet->setPos(glm::vec3(point.x, point.y - 7.5, 0));
    bullet->setDir(direction);
    bullet->setSpeed(1.0f);
    bullet->setTexture(TEX_WOOD_WALL);
    bullet->setShadows(false);
    bullet->setSpeed(BULLET_SPD);
    bullet->scale(glm::vec3(0.2, 0.2, 0.2));
    bullets.push_back(bullet);
}
