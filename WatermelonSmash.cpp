#include "WatermelonSmash.h"

WatermelonSmash::WatermelonSmash(GLuint _ShadeProg, Clicks* _clicks, Sound* _sound) {
    
    // Inititalize the game
    ShadeProg = _ShadeProg;
    clicks = _clicks;
    sound = _sound;
    score = 0;
    timeStart = timer = timeLeft = timeRight = 0.0;
    spawnLeft = spawnRight = false;
    gameOver = gameStart = false;

    // Set up the booth
    setUp();
    
    // Display the game description and rules
    printf("\t\t----- Welcome to the WATERMELON SMASH -----\n");
    printf("\t\tSmash the watermelons as they appear by clicking them.\n");
    printf("\t\tDestroy bigger watermelons to earn more points!.\n\n");
    printf("\t\tHitting a watermelon = 1 point\n");
    printf("\t\tSmall watermelon = 10 points, 2 hits\n");
    printf("\t\tMedium watermelon = 20 points, 8 hits\n");
    printf("\t\tLarge watermelon = 30 points, 18 hits\n");
    printf("\t\tYou have 30 seconds to smash as many watermelons as you can!\n");
    printf("\t\tPress ENTER to start the game!.\n");
    printf("\t\tOnce you are done, press SPACE to exit.\n\n");
}

WatermelonSmash::~WatermelonSmash() {
    for (int i = 0; i < bullets.size(); ++i) {
        delete bullets[i];
    }
    for (int i = 0; i < melons.size(); i++) {
        delete melons[i];
    }
}

void WatermelonSmash::setUp() {
    // Create a wall in the back
    wall = new Object(shapes, materials, ShadeProg);
    wall->load("cube.obj");
    wall->setPos(glm::vec3(0.0, 0.0, 30.0));
    wall->scale(glm::vec3(100.0f, 100.0f, 1.0f));
    wall->setTexture(TEX_WOOD_WALL);
    wall->setShadows(false);
    
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
    
    // Decide where to put the watermelon
    newObj->setPos(glm::vec3(xPos, 2.0, MELON_DEPTH));
    
    // Add the watermelons to the game
    Watermelon *newMelon = new Watermelon(newObj);
    newMelon->xPos = xPos;
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
        // Check whether the game has ended (50 seconds)
        if (window->time - timeStart >= MELON_TIME) {
            printf("Time's up! Your score is: %d\n", score);
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
    }
}

void WatermelonSmash::step(Window* window) {
    // Draw the booth
    wall->draw();
    
    // Check how much time has passed and whether game is playing
    if (gameOver || !gameStart)
        return;
    checkTime(window);
    
    // Draw the watermelons
    for (int i = 0; i < melons.size(); i++) {
        if (melons[i] == NULL) {
            melons.erase(melons.begin() + i--);
            continue;
        }
        melons[i]->object->draw();
    }
    // Fire the bullets
    for (int i = 0; i < bullets.size(); i++){
        if (bullets[i]->getPos().z <= MELON_DEPTH) {
            if (bullets[i] != NULL) {
                bullets[i]->setPos(bullets[i]->calculateNewPos(window->dt));
                //bullets[i]->draw();
                
                // Check collision of bullet against watermelons
                for (int j = 0; j < melons.size(); ++j) {
                    if (bullets[i]->collidedWithObj(*melons[j]->object, window->dt)) {
                        // Hit the melon
                        sound->playContactSound();
                        int pointsEarned = melons[j]->hit();
                        printf("Hit a melon! Points earned: %d\n", pointsEarned);
                        score += pointsEarned;
                        printf("Score: %d\n", score);
                        
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
                            melons.erase(melons.begin() + j--);
                        }
                    }
                }
            }
        }
        // Remove the bullet if it has gone past the target
        else
            bullets.erase(bullets.begin() + i--);
    }
}

void WatermelonSmash::mouseClick(glm::vec3 direction, glm::vec4 point) {
    //printf("(x, y) = (%lf, %lf)\n", point.x, point.y);
    
    // Shoot a "bullet"
    Object* bullet = new Object(shapes, materials, ShadeProg);
    bullet->load("sphere.obj");
    bullet->setPos(glm::vec3(point.x, 2.25, 0));
    bullet->setDir(direction);
    bullet->setSpeed(1.0f);
    bullet->setTexture(TEX_WOOD_WALL);
    bullet->setShadows(false);
    bullet->setSpeed(BULLET_SPD);
    bullet->scale(glm::vec3(0.2, 0.2, 0.2));
    bullets.push_back(bullet);
}