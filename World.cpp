#include "World.h"
#include "ParticleSorter.h"
#include <algorithm>

// custom particle functions
void fireflyFunc(glm::vec3* pos, glm::vec3* vel, glm::vec3* grav, glm::vec4* color, float time);


World::World(GLuint _ShadeProg, Camera* _camera) {
   // Default attribute values
   objStartTime = 0.0;
   numCollected = 0;

   // Defined attribute values
   ShadeProg = _ShadeProg;
   h_aPos = GLSL::getAttribLocation(ShadeProg, "aPos");
   h_aNor = GLSL::getAttribLocation(ShadeProg, "aNor");
   h_uM = GLSL::getUniformLocation(ShadeProg, "uM");
   h_uTexUnit = GLSL::getUniformLocation(ShadeProg, "uTexUnit");

   inGame = false;
    
   skybox = new SkyBox(ShadeProg);
   camera = _camera;
   initGround();
   setupOverWorld();
   createExtras((char *)"objs/squish_blue.obj", TEX_SQUISH_BLUE);
   
   // initialize time and gravity for particles
   t = 0.0f;
   t0_disp = 0.0f;
   t_disp = 0.0f;
	h = 1.0f;
	g = glm::vec3(0.0f, -0.01f, 0.0f);
   
   drawWorld = true;
}

World::~World() {
   for (int i=0; i<objects.size(); ++i) { 
      delete objects[i];
   }
   for (int i=0; i<extras.size(); ++i) { 
      delete extras[i]->object;
      delete extras[i];
   }
   for (int i=0; i<structures.size(); ++i) {
      delete structures[i];
   }
   for (int i=0; i<booths.size(); ++i) {
      delete booths[i];
   }
   for (int i=0; i<lanterns.size(); ++i) {
      delete lanterns[i];
   }
   for (int i=0; i<fountainParticles.size(); ++i) {
      delete fountainParticles[i];
   }
}

void fireflyFunc(glm::vec3* pos, glm::vec3* vel, glm::vec3* grav, glm::vec4* color, float time) {
   pos->y += (1.0f/50.0f)*cos(time/50.0f);
}

void World::initParticles(Program* prog) {
   // load fountainParticles
   fountainParticles.clear();
	for(int i = 0; i < NUM_FOUNTAIN_PARTICLES; ++i) {
		Particle* particle = new Particle(); // !C++11: Particle *particle = new Particle();
		particle->load();
		particle->setTexture(TEX_PARTICLE);
		if (i%2 == 0) {
		   particle->setStartPos(glm::vec3(0.0f, 3.0f, 30.0f));
		}
		else {
		   particle->setStartPos(glm::vec3(0.0f, 3.0f, -30.0f));
		}
		particle->setStartCol(glm::vec3(0.1f, 0.1f, 0.8f));
		particle->setStartTTL(FOUNTAIN_TTL);
		particle->startTime = randFloat(0.0f, FOUNTAIN_TTL);
		particle->setStartOpacity(0.6f);
		particle->setOpacityTaper(false);
		fountainParticles.push_back(particle);
	
      fountainParticles[i]->init(prog);
   }
   // load fireflies
   fireflyParticles.clear();
   for (int i = 0; i < NUM_FIREFLY_PARTICLES; ++i) {
      Particle* particle = new Particle();
      particle->load();
      particle->setTexture(TEX_PARTICLE);
      particle->setStartPos(glm::vec3(randFloat(-SIZE,SIZE), randFloat(0.2f, 5.0f), randFloat(-SIZE, SIZE)));
      particle->setStartVel(glm::vec3(0.0f, 0.0f, 0.0f));
      particle->setStartCol(glm::vec3(0.99f, 0.99f, 0.68f));
      particle->setStartTTL(1000.0f);
      particle->startTime = randFloat(0.0f, 20.0f);
      particle->setStartOpacity(0.8f);
      particle->setOpacityTaper(false);
      particle->setUpdateFunc(&fireflyFunc);
      fireflyParticles.push_back(particle);
      
      fireflyParticles[i]->init(prog);
   }
   // load fireworks
   fireworkParticles.clear();
   // put all possible firework positions into a list
   vector<glm::vec3> fireworkPositions;
   fireworkPositions.clear();
   for (float k = -SIZE; k < SIZE; ++k) {
      fireworkPositions.push_back(glm::vec3(SIZE+10.0f, 30.0f, k));
      fireworkPositions.push_back(glm::vec3(-SIZE-10.0f, 30.0f, k));
      fireworkPositions.push_back(glm::vec3(k, 30.0f, SIZE+10.0f));
      fireworkPositions.push_back(glm::vec3(k, 30.0f, -SIZE-10.0f));
   }
   for (int i = 0; i < NUM_FIREWORK_PARTICLES; ++i) {
      Particle* particle = new Particle();
      particle->load();
      particle->setTexture(TEX_PARTICLE);
      particle->setStartPos(fireworkPositions[(int)(randFloat(0.0f,(float)(fireworkPositions.size()-1))+0.5f)]);
      //particle->setStartVel(glm::vec3(0.0f, 0.0f, 0.0f));
      //particle->setStartCol(glm::vec3(0.99f, 0.99f, 0.68f));
      particle->setStartTTL(50.0f);
      particle->startTime = randFloat(0.0f, 50.0f);
      //particle->setStartOpacity(0.8f);
      //particle->setOpacityTaper(false);
      //particle->setUpdateFunc(&fireflyFunc);
      fireworkParticles.push_back(particle);
   }
}

void World::particleStep(Program* prog, Window* window) {
   // Display every 60 Hz
   
	t += h;
	
	t_disp = window->getElapsedTime();
	
	// sort the fountainParticles from back to front
   MatrixStack temp;
   camera->applyViewMatrix(&temp);
   glm::mat4 V = temp.topMatrix();
   ParticleSorter sorter;
   sorter.C = glm::transpose(glm::inverse(V)); // glm is transposed!
   std::sort(fountainParticles.begin(), fountainParticles.end(), sorter);
   
   // Create matrix stacks
	MatrixStack P, MV;
	// Apply camera transforms
	P.pushMatrix();
	camera->applyProjectionMatrix(&P);
	MV.pushMatrix();
	camera->applyViewMatrix(&MV);
	
	// Bind the program
	prog->bind();
	
	glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P.topMatrix()));
	for (int i=0; i<fountainParticles.size(); ++i) {
	   fountainParticles[i]->update(t, h, g);
		fountainParticles[i]->draw(&MV);
	}
	
	for (int i=0; i<fireflyParticles.size(); ++i) {
	   fireflyParticles[i]->update(t, h, glm::vec3(0.0f, 0.0f, 0.0f));
	   fireflyParticles[i]->draw(&MV);
	}
	
	for (int i=0; i<fireworkParticles.size(); ++i) {
	   fireworkParticles[i]->update(t, h, g);
	   //fireworkParticles[i]->draw(&MV);
	}
	// Unbind the program
	prog->unbind();
}

void World::step(Window* window) {
   
	float alpha = std::fmod(window->dt, 1.0f);
	//printf("alpha: %f\n",alpha);
   
   bool playerHit = false;
   
   if (!inGame) {
      for (int i=0; i<extras.size(); ++i) {
         struct Extra* extra = extras[i];
         //extra->step();
         //drawObject(extra);
         if (cmpVec3(extra->currPos,extra->targetPos)) {
            if (extra->rest == 0) {
               //extra->rest = 100;
            }
         }
         // use alpha to linearly interpolate between startPos and targetPos
         //extra->currPos.x += alpha;
         //extra->object->pos = extra->currPos;
         if (extra->rest > 0) {
            extra->rest--;
         }
         if (extra->rest == 0) {
            //extra->object->pos.x += alpha;
            if (!passedTarget(extra)) {
               extra->object->pos += extra->object->getDir() * alpha;
               //extra->object->setDir(extra->object->getDir());
               extra->currPos = extra->object->pos;
            }
            else {
               extra->rest = 100;
               extra->startPos = extra->targetPos;
               extra->currPos = extra->object->pos;
               findNewExtraTarget(extra);
            }
         }
         extra->object->pos.y = 0.6f;
         extra->object->draw();
      }
      drawGround();
      drawOverWorld();
      
      
     /*float s = SIZE - (playerXZRad*2.0f);
    
     if (player->pos.x < -s) {
        player->pos.x = -s;
        playerHit = true;
     }
     if (player->pos.x > s) {
        player->pos.x = s;
        playerHit = true;
     }
     if (player->pos.z < -s) {
        player->pos.z = -s;
        playerHit = true;
     }
     if (player->pos.z > s) {
        player->pos.z = s;
        playerHit = true;
     }
      
      // code to stop camera from going through things... will probably need to be moved somewhere else
      for (int i=0; i<structures.size(); ++i) {
         // if camera.pos violates bounds, then set pos to be equal to bound... 
         glm::vec3 colPlane;
         // TODO influence check should use player position in the future
         //structures[i]->checkInteract(camera->pos);
         
         // check for player hit detection
         if (structures[i]->checkPlayerCollision(player, &colPlane)) {
            playerHit = true;
            if (abs(colPlane.x) > 0.0f) {
               // hit an x boundary
               if ((colPlane.x - structures[i]->pos.x) > 0.0f) {
                  player->pos.x = colPlane.x + playerXZRad;
               }
               if ((colPlane.x - structures[i]->pos.x) < 0.0f) {
                  player->pos.x = colPlane.x - playerXZRad;
               }
            }
            else if (abs(colPlane.y) > 0.0f) {
               // hit a y boundary
               if ((colPlane.y - structures[i]->pos.y) > 0.0f) {
                  player->pos.y = colPlane.y + playerXZRad;
               }
               if ((colPlane.y - structures[i]->pos.y) < 0.0f) {
                  player->pos.y = colPlane.y - playerXZRad;
               }
            }
            else if (abs(colPlane.z) > 0.0f) {
               // hit a z boundary
               if ((colPlane.z - structures[i]->pos.z) > 0.0f) {
                  player->pos.z = colPlane.z + playerXZRad;
               }
               if ((colPlane.z - structures[i]->pos.z) < 0.0f) {
                  player->pos.z = colPlane.z - playerXZRad;
               }
            }
         }
         // collision detection
         if (structures[i]->checkCameraCollision(camera->pos, &colPlane)) {
            if (abs(colPlane.x) > 0.0f) {
               // hit an x boundary
               camera->pos.x = colPlane.x;
            }
            else if (abs(colPlane.y) > 0.0f) {
               // hit a y boundary
               camera->pos.y = colPlane.y;
            }
            else if (abs(colPlane.z) > 0.0f) {
               // hit a z boundary
               camera->pos.z = colPlane.z;
            }
         }
      }
      for (int i=0; i<booths.size(); ++i) {
         
         // if camera.pos violates bounds, then set pos to be equal to bound... 
         glm::vec3 colPlane;
         // TODO influence check should use player position in the future
         //booths[i]->checkInteract(camera->pos);
         booths[i]->checkInteract(player->pos);
         // check for player hit detection
         if (booths[i]->checkPlayerCollision(player, &colPlane)) {
            playerHit = true;
            if (abs(colPlane.x) > 0.0f) {
               // hit an x boundary
               if ((colPlane.x - booths[i]->booth[1]->pos.x) > 0.0f) {
                  player->pos.x = colPlane.x + playerXZRad;
               }
               if ((colPlane.x - booths[i]->booth[1]->pos.x) < 0.0f) {
                  player->pos.x = colPlane.x - playerXZRad;
               }
            }
            else if (abs(colPlane.y) > 0.0f) {
               // hit a y boundary
               if ((colPlane.y - booths[i]->booth[1]->pos.y) > 0.0f) {
                  player->pos.y = colPlane.y + playerXZRad;
               }
               if ((colPlane.y - booths[i]->booth[1]->pos.y) < 0.0f) {
                  player->pos.y = colPlane.y - playerXZRad;
               }
            }
            else if (abs(colPlane.z) > 0.0f) {
               // hit a z boundary
               if ((colPlane.z - booths[i]->booth[1]->pos.z) > 0.0f) {
                  player->pos.z = colPlane.z + playerXZRad;
               }
               if ((colPlane.z - booths[i]->booth[1]->pos.z) < 0.0f) {
                  player->pos.z = colPlane.z - playerXZRad;
               }
            }
         }
         
         // collision detection
         if (booths[i]->checkCameraCollision(camera->pos, &colPlane)) {
            if (abs(colPlane.x) > 0.0f) {
               // hit an x boundary
               camera->pos.x = colPlane.x;
            }
            else if (abs(colPlane.y) > 0.0f) {
               // hit a y boundary
               camera->pos.y = colPlane.y;
            }
            else if (abs(colPlane.z) > 0.0f) {
               // hit a z boundary
               camera->pos.z = colPlane.z;
            }
         }
      }*/
   }

   
   camera->step(window);
   if (!inGame) {
      skybox->draw(camera, window);
   }
}

bool World::passedTarget(struct Extra* extra) {
   if (glm::distance(extra->currPos, extra->startPos) > glm::distance(extra->targetPos, extra->startPos)) {
      return true;
   }
   return false;
}

bool World::checkStructureCollisions(Object* object) {
   for (int i=0; i<structures.size(); ++i) {
      if (structures[i]->checkCollision(object)) {
         return true;
      }
   }
   return false;
}

bool World::checkBoothCollisions(Object* object) {
   for (int i=0; i<booths.size(); ++i) {
      if (booths[i]->booth[1]->checkCollision(object)) {
         return true;
      }
   }
   return false;
}

void World::findNewExtraTarget(struct Extra* extra) {
   // populate a buffer containing the available spaces not interrupted by an object.
   vector<glm::vec3> availablePositions;
   vector<int> Is;
   vector<int> Js;
   Object* tempObj = new Object(shapes, materials, ShadeProg);
   for (int i = extra->i; i < (int)SIZE * 2 - 2; i++) {
      tempObj->pos = mapGrid[i][extra->j];
      if (checkStructureCollisions(tempObj)) {
         break;
      }
      if (checkBoothCollisions(tempObj)) {
         break;
      }
      availablePositions.push_back(mapGrid[i][extra->j]);
      Is.push_back(i);
      Js.push_back(extra->j);
   }
   for (int i = extra->i; i > 0; i--) {
      tempObj->pos = mapGrid[i][extra->j];
      if (checkStructureCollisions(tempObj)) {
         break;
      }
      if (checkBoothCollisions(tempObj)) {
         break;
      }
      availablePositions.push_back(mapGrid[i][extra->j]);
      Is.push_back(i);
      Js.push_back(extra->j);
   }  
   
   for (int j = extra->j; j < (int)SIZE * 2 - 2; j++) {
      tempObj->pos = mapGrid[extra->i][j];
      if (checkStructureCollisions(tempObj)) {
         break;
      }
      if (checkBoothCollisions(tempObj)) {
         break;
      }
      availablePositions.push_back(mapGrid[extra->i][j]);
      Is.push_back(extra->i);
      Js.push_back(j);
   }
   for (int j = extra->j; j > 0; j--) {
      tempObj->pos = mapGrid[extra->i][j];
      if (checkStructureCollisions(tempObj)) {
         break;
      }
      if (checkBoothCollisions(tempObj)) {
         break;
      }
      availablePositions.push_back(mapGrid[extra->i][j]);
      Is.push_back(extra->i);
      Js.push_back(j);
   }  
   
   // next, we find a random position 
   if ((int)availablePositions.size() > 0) {
      
      int randomIndex = (int)(randFloat(0.0f,(float)availablePositions.size()-1.0f) + 0.5f);
         
      extra->targetPos = availablePositions[randomIndex];
      extra->i = Is[randomIndex];
      extra->j = Js[randomIndex];
      extra->object->setDir(glm::normalize(extra->targetPos - extra->startPos));
   }
   delete tempObj;
   availablePositions.clear();
   Is.clear();
   Js.clear();
}

Booth* World::currentActiveBooth() {
   for (int i=0; i<booths.size(); ++i) {
      if (booths[i]->isActive()) {
         return booths[i];
      }
   }
   return NULL;
}

inline void World::safe_glUniformMatrix4fv(const GLint handle, const GLfloat data[]) {
   if (handle >= 0)
      glUniformMatrix4fv(handle, 1, GL_FALSE, data);
}

void World::initGround() {
    ground = new Object(shapes, materials, ShadeProg);
    ground->load("objs/ground_sakura.obj", "objs/ground_sakura.mtl");
    ground->scale(glm::vec3(SIZE * 2, SIZE * 2, SIZE * 2));
    ground->setTexture(TEX_GROUND_SAKURA);
    ground->setShadows(false);
    
    int i = 0;
    for (float z = -SIZE + 1.0f; z < SIZE - 1.0f; ++z) {
      int j = 0;
      for (float x = -SIZE + 1.0f; x < SIZE - 1.0f; ++x) {
         mapGrid[i][j] = glm::vec3(x, 0.75f, z);
         j++;
      }
      i++;
   }
}

void World::drawGround() {
    ground->draw();
}

void World::drawOverWorld() {
   for (int i=0; i<structures.size(); ++i) {
          /*
          Object **bObjs = structure->getBooths();
          for (int i = 0; i < 3; i++) {
              drawObject(bObjs[i]);
          }
          */
      if (drawWorld) {
         structures[i]->draw();
         structures[i]->drawBounds = false;
      }
      else {
         structures[i]->drawBounds = true;
      }
         
   }
   for (int i=0; i<booths.size(); ++i) {
          /*
          Object **bObjs = structure->getBooths();
          for (int i = 0; i < 3; i++) {
              drawObject(bObjs[i]);
          }
          */
      if (drawWorld) {
         booths[i]->draw();
         booths[i]->booth[1]->drawBounds = false;
      }
      else {
         booths[i]->booth[1]->drawBounds = true;
      }
   }
   
   for (int i=0; i<structures.size(); ++i) {
      structures[i]->getBounds(&(structures[i]->bounds));
   }
   for (int i=0; i<booths.size(); ++i) {
      booths[i]->calculateBoundingBox();
   }
   for (int i=0; i<lanterns.size(); ++i) {
      lanterns[i]->draw();
   }
}

void World::inMiniGame() {
   inGame = true;
}

void World::leftMiniGame() {
   inGame = false;
}

void World::setupOverWorld() {
    // build walls based on map size
    Object* wall1 = new Object(shapes, materials, ShadeProg);
    wall1->load(WALL_FILE_NAME);
    wall1->translate(glm::vec3(-SIZE-0.5f, 2.5f, 0.0f));
    wall1->scale(glm::vec3(1.0f, 10.0f, SIZE*2.0f));
    wall1->setTexture(TEX_WOOD_WALL);
    structures.push_back(wall1);

    Object* wall2 = new Object(shapes, materials, ShadeProg);
    wall2->load(WALL_FILE_NAME);
    wall2->translate(glm::vec3(SIZE+0.5f, 2.5f, 0.0f));
    wall2->scale(glm::vec3(1.0f, 10.0f, SIZE*2.0f));
    wall2->setTexture(TEX_WOOD_WALL);
    structures.push_back(wall2);

    Object* wall3 = new Object(shapes, materials, ShadeProg);
    wall3->load(WALL_FILE_NAME);
    wall3->translate(glm::vec3(0.0f, 2.5f, -SIZE-0.5f));
    wall3->scale(glm::vec3(SIZE*2.0f, 10.0f, 1.0f));
    wall3->setTexture(TEX_WOOD_WALL);
    structures.push_back(wall3);

    Object* wall4 = new Object(shapes, materials, ShadeProg);
    wall4->load(WALL_FILE_NAME);
    wall4->translate(glm::vec3(0.0f, 2.5f, SIZE+0.5f));
    wall4->scale(glm::vec3(SIZE*2.0f, 10.0f, 1.0f));
    wall4->setTexture(TEX_WOOD_WALL);
    structures.push_back(wall4);
   
   parseMapFile(MAP_FILE_NAME);
   
   // initialize world along with bounding boxes
   drawOverWorld();
   for (int i=0; i<structures.size(); ++i) {
      structures[i]->getBounds(&(structures[i]->bounds));
   }
   for (int i=0; i<booths.size(); ++i) {
      booths[i]->calculateBoundingBox();
   }
    
   createPlayer((char *)"objs/squish_red.obj", TEX_SQUISH_RED);
}

/* Read in a map file and parse based on defined by-line format */
void World::parseMapFile(const char* fileName) {
   ifstream mapFile;
   mapFile.open(fileName);
   string line;
   if (mapFile.is_open()) {
      // read in the lines;
      while (getline(mapFile, line)) {
         // this line is a comment... skip it
         if (line[0] == '#') {
            continue;
         }
         
         char type[10];
         char minigame[20];
         float xt, yt, zt, xs, ys, zs, angle;
         // load the booth data
         sscanf(line.c_str(), "%s (%f,%f,%f) (%f,%f,%f) %f %s\n", type, &xt, &yt, &zt, &xs, &ys, &zs, &angle, minigame);

         Object* structure = new Object(shapes, materials, ShadeProg);
         
         glm::vec3 _pos = glm::vec3(xt, yt, zt);
         glm::vec3 _scalar = glm::vec3(xs, ys, zs);
         
         if (strcmp(type, "booth") == 0) {
            Booth* booth = new Booth(_pos, _scalar, angle, minigame, shapes, materials, ShadeProg);
            booths.push_back(booth);
         }
         else if (strcmp(type, "wall") == 0) {
            structure->translate(_pos);
            structure->rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f));   // all rotations for the map will be in the y-axis
            structure->scale(_scalar);
            structure->load(WALL_FILE_NAME);
            structure->setTexture(TEX_WOOD_RED);
            structures.push_back(structure);
         }
         else if (strcmp(type, "lantern") == 0) {
            structure->translate(_pos);
            structure->rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f));   // all rotations for the map will be in the y-axis
            structure->scale(_scalar);
            
            structure->load(LANTERN_FILE_NAME);
            structure->setTexture(TEX_LANTERN);
            structures.push_back(structure);
         }
         else if (strcmp(type, "fountain") == 0) {
            structure->translate(_pos);
            structure->rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f));   // all rotations for the map will be in the y-axis
            structure->scale(_scalar);
            
            structure->load(FOUNTAIN_FILE_NAME);
            structure->setTexture(TEX_LANTERN);
            structures.push_back(structure);
         }
      }
      mapFile.close();
   }
   else {
      printf("file %s was not available or could not be opened\n", fileName);
   }
}

bool World::detectSpawnCollision(Object* object) {
   for (int i = 0; i < structures.size(); ++i) {
      if (structures[i]->checkCollision(object)) {
         return true;
      }
   }
   for (int i = 0; i < booths.size(); ++i) {
      if (booths[i]->booth[1]->checkCollision(object)) {
         return true;
      }
   }
   for (int i=0; i<extras.size(); ++i) {
      if (extras[i]->object->collision(object)) {
         return true;
      }
   }
   return false;
}

void World::calcExtraSpawnPosition(struct Extra* extra) {
   int rand_i = (int)(randFloat(0.0f, SIZE*2.0f-3.0f) + 0.5f); 
   int rand_j = (int)(randFloat(0.0f, SIZE*2.0f-3.0f) + 0.5f);
   extra->object->setPos(mapGrid[rand_i][rand_j]);
   while (detectSpawnCollision(extra->object)) {
      rand_i = (int)(randFloat(0.0f, SIZE*2.0f-3.0f) + 0.5f); 
      rand_j = (int)(randFloat(0.0f, SIZE*2.0f-3.0f) + 0.5f);
      extra->object->pos = mapGrid[rand_i][rand_j];
   }
   extra->startPos = extra->currPos = extra->targetPos = extra->object->pos;
   extra->i = rand_i;
   extra->j = rand_j;
}

void World::createExtras(const string &meshName, int texID) {
   blerch = 0;
   for (int i = 0; i < MAX_OBJS; ++i) {
      struct Extra* extra;
      extra = new struct Extra;
      extra->object = new Object(shapes, materials, ShadeProg);
      extra->object->load(meshName);
      extra->object->setTexture(texID);
      extra->object->scale(glm::vec3(3.0f, 3.0f, 3.0f));
      extra->object->translate(glm::vec3(0.0f, 0.9f, 0.0f));
      
      extra->object->setDir(glm::vec3(0.0f, 0.0f, 1.0f));
      extra->rest = 10;
      calcExtraSpawnPosition(extra);
      
      findNewExtraTarget(extra);
      
      //extra->object->setDir(glm::normalize(glm::vec3(Util::randF()-0.5, 0.0, Util::randF()-0.5)));
      extra->object->setSpeed(OBJ_SPEED);
      extras.push_back(extra);
   }
}

void World::createPlayer(const string &meshName, int texID) {
   player = new Object(shapes, materials, ShadeProg);
   player->load(meshName);
   player->setTexture(texID);
   player->scale(glm::vec3(2.5f, 2.5f, 2.5));
   player->translate(glm::vec3(0.0f, -0.4f, 0.0f));
   camera->initPlayer(player);
   
   playerXZRad = player->getXZRadius();
//   printf("playerXZRad: %f\n", playerXZRad);
}

int World::numLeft() {
   return extras.size();
}

void World::drawObject(Object* obj) {
   // Get the model-view matrix for this object
   glm::mat4 matrix = camera->Projection * camera->View * obj->getModelMatrix();

   // Extract the planes of the view frustum
   glm::vec4* planes = (glm::vec4*) calloc(6, sizeof(glm::vec4));
   extractViewFrustumPlanes(planes, matrix);

   // Check if the object is in the view frustum
   glm::vec3 pos = obj->getPos();
   float rad = obj->getRadius();
   if (checkPlane(planes[0], pos, rad)      // left
      && checkPlane(planes[1], pos, rad)    // right
      && checkPlane(planes[2], pos, rad)    // top
      && checkPlane(planes[3], pos, rad)    // bottom
      && checkPlane(planes[4], pos, rad)    // near
      && checkPlane(planes[5], pos, rad)) { // far
      // Object is inside the view frustum, draw it
      obj->draw();
   }

   // Free the planes (stops memory leaks)
   free(planes);
}

void World::extractViewFrustumPlanes(glm::vec4* planes, const glm::mat4 matrix) {
   // Left plane
   planes[0].x = matrix[3][0] + matrix[0][0];
   planes[0].y = matrix[3][1] + matrix[0][1];
   planes[0].z = matrix[3][2] + matrix[0][2];
   planes[0].w = matrix[3][3] + matrix[0][3];

   // Right plane
   planes[1].x = matrix[3][0] - matrix[0][0];
   planes[1].y = matrix[3][1] - matrix[0][1];
   planes[1].z = matrix[3][2] - matrix[0][2];
   planes[1].w = matrix[3][3] - matrix[0][3];

   // Top plane
   planes[2].x = matrix[3][0] - matrix[1][0];
   planes[2].y = matrix[3][1] - matrix[1][1];
   planes[2].z = matrix[3][2] - matrix[1][2];
   planes[2].w = matrix[3][3] - matrix[1][3];

   // Bottom plane
   planes[3].x = matrix[3][0] + matrix[1][0];
   planes[3].y = matrix[3][1] + matrix[1][1];
   planes[3].z = matrix[3][2] + matrix[1][2];
   planes[3].w = matrix[3][3] + matrix[1][3];

   // Near plane
   planes[4].x = matrix[3][0] + matrix[2][0];
   planes[4].y = matrix[3][1] + matrix[2][1];
   planes[4].z = matrix[3][2] + matrix[2][2];
   planes[4].w = matrix[3][3] + matrix[2][3];

   // Far plane
   planes[5].x = matrix[3][0] - matrix[2][0];
   planes[5].y = matrix[3][1] - matrix[2][1];
   planes[5].z = matrix[3][2] - matrix[2][2];
   planes[5].w = matrix[3][3] - matrix[2][3];

   // Normalize planes
   normalizePlane(planes[0]);
   normalizePlane(planes[1]);
   normalizePlane(planes[2]);
   normalizePlane(planes[3]);
   normalizePlane(planes[4]);
   normalizePlane(planes[5]);
}

void World::normalizePlane(glm::vec4& plane) {
   float mag = sqrt(plane.x * plane.x + plane.y * plane.y + plane.z * plane.z);
   plane.x = plane.x / mag;
   plane.y = plane.y / mag;
   plane.z = plane.z / mag;
   plane.w = plane.w / mag;
}

bool World::checkPlane(glm::vec4 plane, glm::vec3 pos, float rad) {
   glm::vec4 v = glm::vec4(pos.x, pos.y, pos.z, 1.0);
   float result = plane.x * v.x + plane.y * v.y + plane.z * v.z + plane.w * v.w;
   bool correctHalfSpace = result > 0;
   bool clipping = glm::abs(result) < glm::abs(rad);

   return correctHalfSpace || clipping;
}
