#include "WatermelonSmash.h"

bool removeExplosion(vector<Particle*> p) {
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
    
    t = 0.0f;
    t0_disp = 0.0f;
    t_disp = 0.0f;
    h = 1.0f;
    g = glm::vec3(0.0f, -0.005f, 0.0f);
    
    fireworks.clear();

    // Set up the booth
    setUp();
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
    delete hammer;
}

void WatermelonSmash::setUp() {
    // Create a wall in the back
    Object *wall = new Object(shapes, materials, ShadeProg);
    wall->load("cube.obj");
    wall->setPos(glm::vec3(0.0, 0.0, 30.0));
    wall->scale(glm::vec3(100.0f, 100.0f, 1.0f));
    wall->setTexture(TEX_WOOD_LIGHT);
    wall->setShadows(false, 0.0, 0.0);
    misc_objects.push_back(wall);
    
    // Create a table for the watermelons
    Object *table = new Object(shapes, materials, ShadeProg);
    table->load("cube.obj");
    table->setPos(glm::vec3(0.0, 0.0, MELON_DEPTH));
    table->scale(glm::vec3(10.0f, 1.0f, 1.0f));
    table->setTexture(TEX_WOOD_DARK);
    table->setShadows(false, 0.0, 0.0);
    misc_objects.push_back(table);
    
    // Create the hammer
    hammer = new Object(shapes, materials, ShadeProg);
    hammer->load("objs/hammer.obj");
    hammer->setPos(glm::vec3(0.0, 3.5, MELON_DEPTH + .2));
    hammer->scale(glm::vec3(3.0, 3.0, 3.0));
    hammer->updateRadius();
    hammer->setTexture(TEX_HAMMER);
    hammer->setShadows(false, 0.0, 0.0);
    
    // Add watermelons
    newMelon(MELON_LEFT);
    newMelon(MELON_RIGHT);
}

void WatermelonSmash::newMelon(float xPos) {
    // Create a watermelon object
    Object *newObj = new Object(shapes, materials, ShadeProg);
    newObj->load((char *)"objs/watermelon.obj");
    newObj->setTexture(TEX_MELON_OUT);
    newObj->setShadows(false, 0.0, 0.0);
    
    // Add the watermelons to the game
    Watermelon *newMelon = new Watermelon(particleProg, camera, newObj, xPos);
    melons.push_back(newMelon);
}

void WatermelonSmash::printInstructions() {
   ifstream instrFile;
   instrFile.open("wminstr.txt");
   string line;
   float yPos = .8;
   float yInc;
   
   fontEngine->useFont("caviar", 30);
   yInc = fontEngine->getTextHeight("blank") * 1.3;
   
   if (instrFile.is_open()) {
      while (getline(instrFile, line)) {
         if (line[0] == '\n') {
            yPos -= yInc;
         }
         
         yPos -= yInc;
         fontEngine->display(glm::vec4(0.98, 0.5, 0.48, 1.0), line.c_str(), 0-fontEngine->getTextWidth(line.c_str())/2.0, yPos);
      }
   }
   else {
      printf("file 'wminstr.txt' was not available or could not be opened\n");
   }
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
            timer = window->time;
        }
        // Check whether the game has ended
        if (window->time - timeStart >= MELON_TIME) {
            global_points += score;
            gameOver = true;
        }
        // Spawn a watermelon
        if (window->time - timeLeft >= MELON_SPAWN && spawnLeft) {
            newMelon(MELON_LEFT);
            ageLeft = window->time;
            spawnLeft = false;
        }
        else if (window->time - timeRight >= MELON_SPAWN && spawnRight) {
            newMelon(MELON_RIGHT);
            ageRight = window->time;
            spawnRight = false;
        }
        // Swing the hammer
        if (timeSwing - window->time > 0) {
            float angle = -360 * (timeSwing - window->time);
            hammer->rotate(angle, glm::vec3(1.0, 0.0, 0.0));
        }
    }
}

void WatermelonSmash::addNewFirework(glm::vec3 pos, float melonSize) {
   
   vector<Particle*> firework;
   firework.clear();
   for (int i=0; i<NUM_PARTICLES; ++i) {
      Particle* particle = new Particle(); // !C++11: Particle *particle = new Particle();
		particle->load();
		particle->setTexture(TEX_PARTICLE);
		particle->setStartPos(pos);
		glm::vec3 vel = glm::vec3(randFloat(-1.0f, 1.0f), randFloat(-1.0f, 3.0f), randFloat(-1.0f, 1.0f));      
		
		if (melonSize > 0.0f) {
		   particle->setStartVel(glm::normalize(vel)/10.0f);
		   particle->setStartScale(0.5f);
		   if (i%2 == 0) {
		      particle->setStartCol(glm::vec3(0.8f, 0.1f, 0.1f));
		   }
	      else {
		      particle->setStartCol(glm::vec3(0.1f, 0.8f, 0.1f));
		   }
		   particle->setStartTTL(EXPLOSION_TIME/1.5f);
		}
		else {
		   particle->setStartVel(glm::normalize(vel)/5.0f);
		   particle->setStartScale(0.9f);
		   if (i%2 == 0) {
		      particle->setStartCol(glm::vec3(0.8f, 0.1f, 0.1f));
		   }
	      else {
		      particle->setStartCol(glm::vec3(0.1f, 0.8f, 0.1f));
		      particle->setStartCol(glm::vec3(0.1f, 0.1f, 0.1f));
		   }
		   particle->setStartTTL(EXPLOSION_TIME);
		}
      particle->init(particleProg);
      
      firework.push_back(particle);
   }
   
   fireworks.push_back(firework);
}

void WatermelonSmash::particleStep() {
       // Display every 60 Hz
	   t += h;
      
      // Create matrix stacks
	   MatrixStack P, MV;
	   // Apply camera transforms
	   P.pushMatrix();
	   camera->applyProjectionMatrix(&P);
	   MV.pushMatrix();
	   camera->applyViewMatrix(&MV);
	
	   // Bind the program
	   particleProg->bind();
	   ParticleSorter sorter;
	
	   glUniformMatrix4fv(particleProg->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P.topMatrix()));
	   
	   for(int i=0; i<fireworks.size(); ++i) {
	      //printf("in particle draw\n");
	      
	      // sort the explosions' Particles from back to front
         MatrixStack temp;
         camera->applyViewMatrix(&temp);
         glm::mat4 V = temp.topMatrix();
   
         sorter.C = glm::transpose(glm::inverse(V)); // glm is transposed!
         std::sort(fireworks[i].begin(), fireworks[i].end(), sorter);
               
	      for (int j=0; j<fireworks[i].size(); ++j) {
	         fireworks[i][j]->update(t, h, g);
		      fireworks[i][j]->draw(&MV);
		   }
	   }
	   
	   fireworks.erase(std::remove_if(fireworks.begin(),
                                           fireworks.end(),
                                           &removeExplosion),
                                           fireworks.end());
	  
	   
	   // Unbind the program
	   particleProg->unbind();
}

void WatermelonSmash::step(Window* window) {
    // Draw the booth
    for (int i = 0; i < misc_objects.size(); i++) 
        misc_objects[i]->draw();
    
    // Check whether game is playing
    if (!gameStart) {
        printInstructions();
        ageRight = ageLeft = window->time;
        return;
    }
    if (gameOver) {
        float yPos = .5;
        float yInc;
 
        char ln1[40];
        sprintf(ln1, "Time's up! Your score is: %d\n", score);
        fontEngine->useFont("caviar", 30);
        fontEngine->display(glm::vec4(0.98, 0.5, 0.48, 1.0), ln1, 0-fontEngine->getTextWidth(ln1)/2.0, yPos);
        yInc = fontEngine->getTextHeight(ln1) * 1.3;
        yPos -= yInc;

        char ln2[40];
        sprintf(ln2, "You smashed %d watermelons!\n", numMelons);
        fontEngine->display(glm::vec4(0.98, 0.5, 0.48, 1.0), ln2, 0-fontEngine->getTextWidth(ln2)/2.0, yPos);
        
        yPos -= (2 * yInc);        
        
        char ln3[40];
        sprintf(ln3, "Press ENTER to exit.");
        fontEngine->display(glm::vec4(0.98, 0.5, 0.48, 1.0), ln3, 0-fontEngine->getTextWidth(ln3)/2.0, yPos);

        return;
    }
    checkTime(window);
   
    // Draw the watermelons and hammer
    for (int i = 0; i < melons.size(); i++)
        melons[i]->object->draw();
    hammer->draw();
    // Check if the watermelons wilted
    for (int i = 0; i < melons.size(); i++) {
        if (melons[i]->xPos == MELON_LEFT) {
            timeLMelon = (int)(melons[i]->lifeSpan + (ageLeft - window->time) + 1);
            if (window->time - ageLeft >= melons[i]->lifeSpan) {
                spawnLeft = true;
                timeLeft = window->time;
                sound->playBuzzerSound();
                melons.erase(melons.begin() + i--);
            }
        }
        else if (melons[i]->xPos == MELON_RIGHT) {
            timeRMelon = (int)(melons[i]->lifeSpan + (ageRight - window->time) + 1);
            if (window->time - ageRight >= melons[i]->lifeSpan) {
                spawnRight = true;
                timeRight = window->time;
                sound->playBuzzerSound();
                melons.erase(melons.begin() + i--);
            }
        }
    }
    // Fire the bullets
    for (int i = 0; i < bullets.size(); i++){
        if (bullets[i]->getPos().z <= MELON_DEPTH && bullets[i]->getPos().z >= -MELON_DEPTH) {
            if (bullets[i] != NULL) {
                bullets[i]->setPos(bullets[i]->calculateNewPos(window->dt));
                for (int j = 0; j < melons.size(); ++j) {
                    // Player hit a watermelon
                    if (bullets[i]->collidedWithObj(*melons[j]->object, window->dt)) {
                        // Hit the melon
                        timeSwing = window->time + MELON_SWING;
                        float height = melons[j]->yPos + melons[j]->size;
                        if (melons[j]->size <= 2.0)
                            height += 0.2;
                        hammer->setPos(glm::vec3(melons[j]->xPos, height, MELON_DEPTH + .2));
                        int pointsEarned = melons[j]->hit();
                        score += pointsEarned;
                        // add an explosion!
                        addNewFirework(glm::vec3(melons[j]->xPos, melons[j]->yPos, MELON_DEPTH), (float)melons[j]->hits);
                        // Remove the melon if it was destroyed
                        if (pointsEarned >= 5) {
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
    
    particleStep();
    // Draw the watermelons exploding
    /*for (int i = 0; i < melons.size(); i++) {
        melons[i]->explosionsStarted.erase(std::remove_if(melons[i]->explosionsStarted.begin(),
                                           melons[i]->explosionsStarted.end(),
                                           &removeExplosions),
                                           melons[i]->explosionsStarted.end());
        melons[i]->particleStep();
    }*/

    textStep(window); 
}

void WatermelonSmash::textStep(Window* window) {
    float yPos = .9;
    float yInc;

    char time[30];
    sprintf(time, "Time remaining: 0:%d / 0:30", (int)(window->time - timeStart));
    fontEngine->useFont("caviar", 30);
    fontEngine->display(glm::vec4(0.98, 0.5, 0.48, 1.0), time, 0-fontEngine->getTextWidth(time)/2.0, yPos);
    yInc = fontEngine->getTextHeight(time) * 1.3;

    char scrStr[15];
    sprintf(scrStr, "Score: %d", score);
    fontEngine->display(glm::vec4(0.98, 0.5, 0.48, 1.0), scrStr, 1-fontEngine->getTextWidth(scrStr)-.07, yPos);

    char melons[30];
    sprintf(melons, "Melons Smashed: %d", numMelons);
    fontEngine->display(glm::vec4(0.98, 0.5, 0.48, 1.0), melons, 0-fontEngine->getTextWidth(melons)/2.0, yPos-yInc);
    
    char left[10];
    sprintf(left, "0:%d", timeLMelon);
    if (!spawnLeft)
       fontEngine->display(glm::vec4(0.98, 0.5, 0.48, 1.0), left, .5-fontEngine->getTextWidth(left)/2.0, -0.4);    
    
    char right[10];
    sprintf(right, "0:%d", timeRMelon);
    if (!spawnRight)
       fontEngine->display(glm::vec4(0.98, 0.5, 0.48, 1.0), right, -.5-fontEngine->getTextWidth(right)/2.0, -0.4);
}

void WatermelonSmash::mouseClick(glm::vec3 direction, glm::vec4 point) {
    // Shoot a "bullet"
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
}
