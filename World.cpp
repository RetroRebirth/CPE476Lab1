#include "World.h"
#include "ParticleSorter.h"
#include <algorithm>

static string objectFiles[] = {"bunny.obj"};


// custom particle functions
void fireflyFunc(glm::vec3* pos, glm::vec3* vel, glm::vec3* grav, glm::vec4* color, float time);


World::World(GLuint _ShadeProg, Camera* _camera) {
   // Default attribute values
   objStartTime = 0.0;
   numCollected = 0;
   drawWorld = true;

   // Defined attribute values
   ShadeProg = _ShadeProg;
   h_aPos = GLSL::getAttribLocation(ShadeProg, "aPos");
   h_aNor = GLSL::getAttribLocation(ShadeProg, "aNor");
   h_uM = GLSL::getUniformLocation(ShadeProg, "uM");
   h_uTexUnit = GLSL::getUniformLocation(ShadeProg, "uTexUnit");

   // Reserve space in vectors to avoid reallocation
   booths.reserve(6);
   structures.reserve(16);
   extras.reserve(MAX_OBJS);
   grass.reserve(MAX_OBJS);

   inGame = false;
   drawWorld = true;
    
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
}

World::~World() {
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
   for (int i=0; i<fountainParticles.size(); ++i) {
      delete fountainParticles[i];
   }
   for (int i=0; i<grass.size(); ++i) {
      delete grass[i];
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
		particle->setTexture(textures[TEX_PARTICLE]);
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
      particle->setTexture(textures[TEX_PARTICLE]);
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
   int i = 0;
   for (float k = -SIZE; k < SIZE-3.0f; ++k) {
      fireworkPositions[i++] = glm::vec3(SIZE+5.0f, 30.0f, k);
      fireworkPositions[i++] = glm::vec3(-SIZE-5.0f, 30.0f, k);
      fireworkPositions[i++] = glm::vec3(k, 30.0f, SIZE+5.0f);
      fireworkPositions[i++] = glm::vec3(k, 30.0f, -SIZE-5.0f);
   }
   // init all avalable colors
   fireworkColors[0] = RED;
   fireworkColors[1] = YELLOW;
   fireworkColors[2] = ORANGE;
   fireworkColors[3] = BLUE;
   fireworkColors[4] = GREEN;
   fireworkColors[5] = PURPLE;
   
   for (int j = 0; j < NUM_FIREWORKS; ++j) {
      glm::vec3 fireworkPos = fireworkPositions[(int)(randFloat(0.0f,SIZE*8.0f)+0.5f)];
      if ((fireworkPos.x < SIZE && fireworkPos.x > -SIZE) && (fireworkPos.z < SIZE && fireworkPos.z > -SIZE))
         continue;
         
      glm::vec3 fireworkCol = fireworkColors[(int)(randFloat(0.0f,(float)COLOR_COUNT-1.0f)+0.5f)];
      float startTime = randFloat(0.0f, 40.0f);
      
      vector<Particle*> particles;
      for (int i = 0; i < NUM_FIREWORK_PARTICLES; ++i) {
         Particle* particle = new Particle();
         particle->load();
         particle->setTexture(textures[TEX_PARTICLE]);
         particle->setStartPos(fireworkPos);
         particle->setStartVel(glm::vec3(randFloat(-0.2f, 0.2f), randFloat(-0.2f, 0.2f), randFloat(-0.2f, 0.2f)));
         particle->setStartCol(fireworkCol);
         particle->setStartTTL(40.0f);
         particle->startTime = startTime;
         particle->setStartScale(1.5f);
         particle->setOpacityTaper(true);
         particles.push_back(particle);
         
         particles[i]->init(prog);
      }
      fireworkParticles.push_back(particles);
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
   std::sort(fireflyParticles.begin(), fireflyParticles.end(), sorter);
   
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
      std::sort(fireworkParticles[i].begin(), fireworkParticles[i].end(), sorter);
	      
	   for (int j=0; j<fireworkParticles[i].size(); ++j) {
	      fireworkParticles[i][j]->update(t, h, glm::vec3(0.0f, -0.001f, 0.0f));
	      fireworkParticles[i][j]->draw(&MV);
	   }
	}
	// Unbind the program
	prog->unbind();
}

void World::step(Window* window) {

	float alpha = std::fmod(window->dt, 1.0f);
   
   bool playerHit = false;
   
   if (!inGame) {
      for (int i=0; i<extras.size(); ++i) {
         struct Extra* extra = extras[i];
         // use alpha to linearly interpolate between startPos and targetPos
         if (extra->rest > 0) {
            extra->rest--;
         }
         if (extra->rest == 0) {
            if (!passedTarget(extra)) {
               extra->object->pos += extra->object->getDir() * alpha;
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
         drawObject(extra->object);
      }
      grassWave(window);
      drawGround();
      drawOverWorld();
   }
   
   if (!inGame) {
      skybox->draw(camera, window);
   }
   camera->step(window, game_state);
}

void World::grassWave(Window* window) {
   for (int i=0; i<grass.size(); ++i) {
      float shearX = 0.2 * sin(window->time + i);
      float shearZ = 0.2 * cos(window->time + i);
      grass[i]->shear(shearX, shearZ);
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
    ground->bumpy = true;
    ground->load("objs/ground_sakura.obj", "objs/ground_sakura.mtl");
    ground->scale(glm::vec3(20.0, 20.0, 20.0));
    ground->setShadows(false, 0.0, 0.0);
    
    int i = 0;
    for (float z = -SIZE + 1.0f; z < SIZE - 1.0f; ++z) {
      int j = 0;
      for (float x = -SIZE + 1.0f; x < SIZE - 1.0f; ++x) {
         if ((i < (int)SIZE * 2 - 2) && (j < (int)SIZE * 2 - 2)) {
            mapGrid[i][j] = glm::vec3(x, 0.75f, z);
         }
         j++;
      }
      i++;
   }
}

void World::drawGround() {
   glm::vec3 curPos;
   ground->setPos(glm::vec3(40.0, 0.0, -40.0));

   for (int y = 0; y < 5; y++) {
      for (int x = 0; x < 5; x++) {
         curPos = ground->getPos();
         
         // zen garden
         if (curPos.x <= -40.0 && curPos.z >= 20.0) {
            ground->bumpy = true;
            ground->setTexture(textures[TEX_SAND_LINE]);
            ground->setNormalmap(textures[TEX_SAND_NORM]);
            ground->translate(glm::vec3(10.0, 0.01, 0.0));
            ground->draw();
            ground->translate(glm::vec3(0.0, 0.0, 0.0));
         }
         else {
            ground->bumpy = false;
            ground->setTexture(textures[TEX_GROUND_SAKURA]);
         }
         ground->draw();
         
         if (y % 2 == 0)
            ground->setPos(glm::vec3(curPos.x, curPos.y, curPos.z + 20.0));
         else
            ground->setPos(glm::vec3(curPos.x, curPos.y, curPos.z - 20.0));
      }
      ground->setPos(glm::vec3(curPos.x - 20.0, curPos.y, curPos.z));
      curPos = ground->getPos();
   }
}

void World::drawOverWorld() {
   for (int i=0; i<grass.size(); ++i) {
      if (drawWorld) {
         drawObject(grass[i]);//->draw();
      }
   }
   for (int i=0; i<structures.size(); ++i) {
      if (drawWorld) {
//printf("STRUCTURES\n");
         if (strcmp(structures[i]->type, "wall") == 0) {  
            structures[i]->draw();
         } else {
            drawObject(structures[i]);//->draw();
         }
         structures[i]->drawBounds = false;
      }
      else {
         //structures[i]->drawBounds = true;
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
//printf("BOOTHS\n");
         /*
         drawObject(booths[i]->booth[0]);
         drawObject(booths[i]->booth[1]);
         drawObject(booths[i]->booth[2]);
         */
         booths[i]->booth[1]->drawBounds = false;
      }
      else {
         //booths[i]->booth[1]->drawBounds = true;
      }
   }
   
   for (int i=0; i<structures.size(); ++i) {
      //structures[i]->getBounds(&(structures[i]->bounds));
   }
   for (int i=0; i<booths.size(); ++i) {
      //booths[i]->calculateBoundingBox();
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
    wall1->setPos(glm::vec3(-SIZE-0.5f, 5.0f, 0.0f));
    wall1->scale(glm::vec3(1.0f, 10.0f, SIZE*2.5f));
    wall1->setTexture(textures[TEX_WOOD_WALL]);
    strcpy(wall1->type, "wall");
    structures.push_back(wall1);

    Object* wall2 = new Object(shapes, materials, ShadeProg);
    wall2->load(WALL_FILE_NAME);
    wall2->setPos(glm::vec3(SIZE+0.5f, 5.0f, 0.0f));
    wall2->scale(glm::vec3(1.0f, 10.0f, SIZE*2.5f));
    wall2->setTexture(textures[TEX_WOOD_WALL]);
    strcpy(wall2->type, "wall");
    structures.push_back(wall2);

    Object* wall3 = new Object(shapes, materials, ShadeProg);
    wall3->load(WALL_FILE_NAME);
    wall3->setPos(glm::vec3(0.0f, 5.0f, -SIZE-0.5f));
    wall3->scale(glm::vec3(SIZE*2.5f, 10.0f, 1.0f));
    wall3->setTexture(textures[TEX_WOOD_WALL]);
    strcpy(wall3->type, "wall");
    structures.push_back(wall3);

    Object* wall4 = new Object(shapes, materials, ShadeProg);
    wall4->load(WALL_FILE_NAME);
    wall4->setPos(glm::vec3(0.0f, 5.0f, SIZE+0.5f));
    wall4->scale(glm::vec3(SIZE*2.5f, 10.0f, 1.0f));
    wall4->setTexture(textures[TEX_WOOD_WALL]);
    strcpy(wall4->type, "wall");
    structures.push_back(wall4);
   
   parseMapFile(MAP_FILE_NAME);
   
   // initialize spatial data structure
   float gridDimension = (int)sqrt((double)UNIFORM_GRID_SIZE);
   for (int i=0; i<gridDimension; ++i) { // rows (z)
      for (int j=0; j<gridDimension; ++j) { // cols (x)
         int index = j + (i * gridDimension);
         spatialGrid[index].members.clear();
         spatialGrid[index].hasPlayer = false;
         
         spatialGrid[index].x_min = -SIZE + (float)(j * gridDimension);
         spatialGrid[index].x_max = -SIZE + (float)((j+1) * gridDimension);
         spatialGrid[index].z_min = -SIZE + (float)(i * gridDimension);
         spatialGrid[index].z_max = -SIZE + (float)((i+1) * gridDimension);
      }
   }
   
   // initialize world along with bounding boxes
   drawOverWorld();
   for (int i=0; i<structures.size(); ++i) {
      structures[i]->getBounds(&(structures[i]->bounds));
   }
   for (int i=0; i<booths.size(); ++i) {
      booths[i]->calculateBoundingBox();
   }
    
   createPlayer((char *)"objs/squish_red.obj", TEX_GIRL_RED);
}

/* Read in a map file and parse based on defined by-line format */
void World::parseMapFile(const char* fileName) {
   ifstream mapFile;
   mapFile.open(fileName);
   string line;
   float wall_dy = 0.0;

   if (mapFile.is_open()) {
      // read in the lines;
      while (getline(mapFile, line)) {
         // this line is a comment... skip it
         if (line[0] == '#') {
            continue;
         }
         
         char type[10] = { 'H', 'e', 'l', 'l', 'o', '\0' };
         char minigame[20] = { 'H', 'e', 'l', 'l', 'o', '\0' };
         float xt, yt, zt, xs, ys, zs, angle;
         // load the booth data
         sscanf(line.c_str(), "%s (%f,%f,%f) (%f,%f,%f) %f %s\n", type, &xt, &yt, &zt, &xs, &ys, &zs, &angle, minigame);
         
         glm::vec3 _pos = glm::vec3(xt, yt, zt);
         glm::vec3 _scalar = glm::vec3(xs, ys, zs);

         Object* structure = new Object(shapes, materials, ShadeProg);
         if (strcmp(type, "booth") != 0) {
            strcpy(structure->type, type);
         }

         if (strcmp(type, "booth") == 0) {
            Booth* booth = new Booth(_pos, _scalar, angle, minigame, shapes, materials, ShadeProg);
            booths.push_back(booth);
         }
         else if (strcmp(type, "wall") == 0) {  
            structure->setPos(_pos);
            structure->rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f));   // all rotations for the map will be in the y-axis
            structure->scale(_scalar);
            structure->load(WALL_FILE_NAME);
            structure->setShadows(true, 0.0 + wall_dy, 0.7);
            wall_dy += 0.001;
            structure->setTexture(textures[TEX_WOOD_RED]);
            structures.push_back(structure);
         }
         else if (strcmp(type, "lantern") == 0) {
            structure->setPos(_pos);
            structure->rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f));   // all rotations for the map will be in the y-axis
            structure->scale(_scalar);
            structure->load(LANTERN_FILE_NAME);
            structure->setShadows(true, 0.01, 0.9);
            structure->setTexture(textures[TEX_LANTERN]);
            structure->updateRadius();
            structures.push_back(structure);
         }
         else if (strcmp(type, "fountain") == 0) {
            structure->setPos(_pos);
            structure->rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f));   // all rotations for the map will be in the y-axis
            structure->scale(_scalar);
            
            structure->load(FOUNTAIN_FILE_NAME);
            structure->setShadows(true, 0.01, 0.8);
            structure->setTexture(textures[TEX_LANTERN]);
            structure->updateRadius();
            structures.push_back(structure);
         }
         else if (strcmp(type, "grass") == 0) {
            structure->load(GRASS_FILE_NAME);
            structure->setTexture(textures[TEX_GROUND_GRASS]);
            structure->translate(_pos);
            structure->rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f));
            structure->scale(_scalar);
            grass.push_back(structure);
         }
         else if (strcmp(type, "tree") == 0){
            structure->load((char *)"objs/tree.obj");
            structure->setTexture(textures[TEX_TREE]);
            structure->setPos(_pos);
            structure->rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f));
            structure->scale(_scalar);
            structure->setShadows(true, 0.01, 0.7);
            structure->isTree = true;
            structures.push_back(structure);
         }
         else if (strcmp(type, "bench") == 0) {
            structure->load((char *)"objs/bench.obj");
            structure->setTexture(textures[TEX_BENCH]);
            structure->translate(_pos);
            structure->rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f));
            structure->scale(_scalar);
            structure->setShadows(true, 0.02, 0.7);
            structures.push_back(structure);
         }
         else if (strcmp(type, "rock") == 0) {
            structure->bumpy = true;
            structure->load((char *)"objs/rock.obj");
            structure->setTexture(textures[TEX_ROCK]);
            structure->setNormalmap(textures[TEX_ROCK_NORM]);
            structure->translate(_pos);
            structure->rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f));
            structure->scale(_scalar);
            structure->setShadows(true, 0.022, 0.7);
            structures.push_back(structure);
         }
         else if (strcmp(type, "miku") == 0) {
             structure->load((char *)"objs/miku.obj");
             structure->setTexture(textures[TEX_MIKU]);
             structure->translate(_pos);
             structure->rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f));
             structure->scale(_scalar);
             structure->setShadows(true, 0.023, 0.9);
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
      //extra->object->setDirectional(true);
      extra->object->setShadows(true, 0.03, 1.0);
      extra->object->setTexture(textures[texID]);
      extra->object->scale(glm::vec3(3.0f, 3.0f, 3.0f));
      extra->object->translate(glm::vec3(0.0f, 0.9f, 0.0f));
      extra->object->updateRadius();
      extra->object->setDir(glm::vec3(0.0f, 0.0f, 1.0f));
      extra->rest = 10;
      calcExtraSpawnPosition(extra);
      
      findNewExtraTarget(extra);
      
      extra->object->setSpeed(OBJ_SPEED);
      extras.push_back(extra);
   }
}

void World::createPlayer(const string &meshName, int texID) {
   player = new Object(shapes, materials, ShadeProg);
   player->bumpy = true;
   player->load(meshName);
   player->setTexture(textures[texID]);
   player->scale(glm::vec3(3.0f, 3.0f, 3.0f));
   player->translate(glm::vec3(0.0f, -0.3f, 0.0f));
   player->setShadows(true, 0.03, 1.0);
   player->bumpy = false;
   camera->initPlayer(player);
    
   playerXZRad = player->getXZRadius();
}

int World::numLeft() {
   return extras.size();
}

// ~~~~~~~~~~~~ VIEW FRUSTUM CULLING ~~~~~~~~~~~~~~~~
// http://gamedevs.org/uploads/fast-extraction-viewing-frustum-planes-from-world-view-projection-matrix.pdf
void World::drawObject(Object* obj) {
   // Combine the projection and model-view matrix for this object
   glm::mat4 matrix = camera->Projection * camera->View * obj->getModelMatrix();

   // Extract the planes of the view frustum
   struct plane planes[6];
   extractViewFrustumPlanes(planes, matrix);

   // Get the object's position and size
//   glm::vec3 pos = obj->getPos();
   glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
   float rad = obj->getXZRadius();

   // Check if the object is in the view frustum
   if (checkPlane(planes[0], pos, rad)       // left
      && checkPlane(planes[1], pos, rad)     // right
      && checkPlane(planes[2], pos, rad)     // top
      && checkPlane(planes[3], pos, rad)     // bottom
      && checkPlane(planes[4], pos, rad)     // near
      && checkPlane(planes[5], pos, rad)) {  // far
      // Draw the object if it is inside the view frustum
      obj->draw();
   }
}

void World::extractViewFrustumPlanes(struct plane* planes, const glm::mat4 matrix) {
   // Left plane
   planes[0].a = matrix[3][0] + matrix[0][0];
   planes[0].b = matrix[3][1] + matrix[0][1];
   planes[0].c = matrix[3][2] + matrix[0][2];
   planes[0].d = matrix[3][3] + matrix[0][3];

   // Right plane
   planes[1].a = matrix[3][0] - matrix[0][0];
   planes[1].b = matrix[3][1] - matrix[0][1];
   planes[1].c = matrix[3][2] - matrix[0][2];
   planes[1].d = matrix[3][3] - matrix[0][3];

   // Top plane
   planes[2].a = matrix[3][0] - matrix[1][0];
   planes[2].b = matrix[3][1] - matrix[1][1];
   planes[2].c = matrix[3][2] - matrix[1][2];
   planes[2].d = matrix[3][3] - matrix[1][3];

   // Bottom plane
   planes[3].a = matrix[3][0] + matrix[1][0];
   planes[3].b = matrix[3][1] + matrix[1][1];
   planes[3].c = matrix[3][2] + matrix[1][2];
   planes[3].d = matrix[3][3] + matrix[1][3];

   // Near plane
   planes[4].a = matrix[3][0] + matrix[2][0];
   planes[4].b = matrix[3][1] + matrix[2][1];
   planes[4].c = matrix[3][2] + matrix[2][2];
   planes[4].d = matrix[3][3] + matrix[2][3];

   // Far plane
   planes[5].a = matrix[3][0] - matrix[2][0];
   planes[5].b = matrix[3][1] - matrix[2][1];
   planes[5].c = matrix[3][2] - matrix[2][2];
   planes[5].d = matrix[3][3] - matrix[2][3];

   // Normalize planes (so we can calculate distance from plane)
   normalizePlane(planes[0]);
   normalizePlane(planes[1]);
   normalizePlane(planes[2]);
   normalizePlane(planes[3]);
   normalizePlane(planes[4]);
   normalizePlane(planes[5]);
}

void World::normalizePlane(struct plane p) {
   float mag = sqrt(p.a * p.a + p.b * p.b + p.c * p.c + p.d * p.d);
   p.a = p.a / mag;
   p.b = p.b / mag;
   p.c = p.c / mag;
   p.d = p.d / mag;
}

bool World::checkPlane(struct plane p, glm::vec3 pos, float rad) {
   // Convert the object's position to a vec4
   glm::vec4 v = glm::vec4(pos.x, pos.y, pos.z, 1.0);

   // Dot product plane with object's position
   float dist = p.a * v.x + p.b * v.y + p.c * v.z + p.d;

   // Is the center of the object in the correct half-space?
   bool correctHalfSpace = dist > 0;
   // Is the object partially visible (clipping)?
   bool notClipping = glm::abs(dist) < glm::abs(rad);

   return correctHalfSpace || notClipping;
}

