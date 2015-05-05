#include "World.h"
#include "Booth.h"

static string objectFiles[] = {"bunny.obj"};

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
   createExtras(EXTRA_FILE_NAME);
}

World::~World() {
   for (int i=0; i<objects.size(); ++i) { 
      delete objects[i];
   }
   for (int i=0; i<extras.size(); ++i) { 
      delete extras[i];
   }
   for (int i=0; i<structures.size(); ++i) {
      delete structures[i];
   }
}

void World::step(Window* window) {
   // Create a new object every SECS_PER_OBJ
   /*if (numLeft() < MAX_OBJS && window->time - objStartTime >= SECS_PER_OBJ) {
      createExtra(EXTRA_FILE_NAME);
      objStartTime = window->time;
   }*/

   if (!inGame) {
      for (int i=0; i<extras.size(); ++i) {
         Object* extra = extras[i];
         extra->draw();
      }

      drawGround();
      drawOverWorld();
      
      // code to stop camera from going through things... will probably need to be moved somewhere else
      for (int i=0; i<structures.size(); ++i) {
         // if camera.pos violates bounds, then set pos to be equal to bound... 
         glm::vec3 colPlane;
         // TODO influence check should use player position in the future
         structures[i]->checkInteract(camera->pos);
         
         // collision detection
         if (structures[i]->checkCameraCollision(camera->pos, &colPlane)) {

            /* debug output for camera collisions
            printf("boundry hit: ");
            printVec3(colPlane);
            printf("camera position: ");
            printVec3(camera->pos);*/
              
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
   }

   camera->step(window);
   skybox->draw(camera, window);
}

Booth* World::currentActiveBooth() {
   for (int i=0; i<structures.size(); ++i) {
      if (structures[i]->isActive()) {
         return structures[i];
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
    ground->scale(glm::vec3(SIZE, SIZE, SIZE));
    ground->setTexture(TEX_GROUND_SAKURA);
}

void World::drawGround() {
    ground->draw();
}

void World::drawOverWorld() {
   for (int i=0; i<structures.size(); ++i) {
      Booth* structure = structures[i];
      structure->object->draw();
   }
}

void World::inMiniGame() {
   inGame = true;
}

void World::leftMiniGame() {
   inGame = false;
}
   

void World::setupOverWorld() {
/*
   // build walls based on map size
   Object* wall1 = new Object(shapes, materials, ShadeProg);
   wall1->load(WALL_FILE_NAME);
   wall1->translate(glm::vec3(-SIZE-0.5f, 2.5f, 0.0f));
   wall1->scale(glm::vec3(1.0f, 10.0f, SIZE*2.0f));
   Booth* bwall1 = new Booth(wall1, (const string*)"wall1", NULL);
   bwall1->setType(WALL_TYPE);
   structures.push_back(bwall1);
   
   Object* wall2 = new Object(shapes, materials, ShadeProg);
   wall2->load(WALL_FILE_NAME);
   wall2->translate(glm::vec3(SIZE+0.5f, 2.5f, 0.0f));
   wall2->scale(glm::vec3(1.0f, 10.0f, SIZE*2.0f));
   Booth* bwall2 = new Booth(wall2, (const string*)"wall2", NULL);
   bwall2->setType(WALL_TYPE);
   structures.push_back(bwall2);
   
   Object* wall3 = new Object(shapes, materials, ShadeProg);
   wall3->load(WALL_FILE_NAME);
   wall3->translate(glm::vec3(0.0f, 2.5f, -SIZE-0.5f));
   wall3->scale(glm::vec3(SIZE*2.0f, 10.0f, 1.0f));
   Booth* bwall3 = new Booth(wall3, (const string*)"wall3", NULL);
   bwall3->setType(WALL_TYPE);
   structures.push_back(bwall3);
   
   Object* wall4 = new Object(shapes, materials, ShadeProg);
   wall4->load(WALL_FILE_NAME);
   wall4->translate(glm::vec3(0.0f, 2.5f, SIZE+0.5f));
   wall4->scale(glm::vec3(SIZE*2.0f, 10.0f, 1.0f));
   Booth* bwall4 = new Booth(wall4, (const string*)"wall4", NULL);
   bwall4->setType(WALL_TYPE);
   structures.push_back(bwall4);
*/
   
   parseMapFile(MAP_FILE_NAME);
   
   // initialize world along with bounding boxes
   drawOverWorld();
   for (int i=0; i<structures.size(); ++i) {
      structures[i]->calculateBoundingBox();
   }

   createPlayer(PLAYER_FILE_NAME);
}

/* Read in a map file and parse based on defined by-line format */
void World::parseMapFile(const char* fileName) {
   ifstream mapFile;
   mapFile.open(fileName);
   string line;
   if (mapFile.is_open()) {
      while (getline(mapFile,line)) {
         // read in the lines;
         //const char* charline = line.c_str();
         if (line[0] == '#') {
            // this line is a comment... skip it
            continue;  
         }
         // parse line format
         char booth_type[10];
         float x_pos = 0.0f, y_pos = 0.0f, z_pos = 0.0f;
         float x_dim = 0.0f, y_dim = 0.0f, z_dim = 0.0f;
         float angle = 0.0f;
         char* minigame = (char*) calloc(20, sizeof(char));
         
         // create a string layout to scan in data pointers...
         sscanf(line.c_str(), "%s (%f,%f,%f) (%f,%f,%f) %f %s\n",
            booth_type,&x_pos,&y_pos,&z_pos,&x_dim,&y_dim,&z_dim,&angle,minigame);
            
         Object* structure = new Object(shapes, materials, ShadeProg);
         if (strcmp(booth_type, "booth") == 0) {
            structure->load(STALL_FILE_NAME, "objs/base_lantern_stall.mtl");
         }
         else if (strcmp(booth_type, "wall") == 0) {
            structure->load(WALL_FILE_NAME);
         }
         structure->translate(glm::vec3(x_pos, y_pos, z_pos));
         structure->rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f));   // all rotations for the map will be in the y-axis
         structure->scale(glm::vec3(x_dim, y_dim, z_dim));
         Booth* booth = new Booth(structure, (const string*)"booth", &minigame);
         booth->setType(BOOTH_TYPE);
         structures.push_back(booth);
      }
      mapFile.close();
   }
   else {
      printf("file %s was not available or could not be opened\n", fileName);
   }
}

bool World::detectSpawnCollision(Object* object) {
   for (int i=0; i<structures.size(); ++i) {
      if (structures[i]->checkCollision(object)) {
         return true;
      }
   }
   for (int i=0; i<extras.size(); ++i) {
      if (extras[i]->collision(object)) {
         return true;
      }
   }
   return false;
}

void World::createExtras(const string &meshName) {
   for (int i=0; i<MAX_OBJS; ++i) {
      Object* extra = new Object(shapes, materials, ShadeProg);
      extra->load(meshName);
      extra->setPos(glm::vec3(2*Util::randF()*SIZE - SIZE, 1.0, 2*Util::randF()*SIZE - SIZE));
      while (detectSpawnCollision(extra)) {
         extra->setPos(glm::vec3(2*Util::randF()*SIZE - SIZE, 1.0, 2*Util::randF()*SIZE - SIZE));
      }
      extra->setDir(glm::normalize(glm::vec3(Util::randF()-0.5, 0.0, Util::randF()-0.5)));
      extra->setSpeed(OBJ_SPEED);
      extras.push_back(extra);
   }
}

void World::createPlayer(const string &meshName) {
   Object* player = new Object(shapes, materials, ShadeProg);
   player->load(meshName);
   camera->initPlayer(player);
}

int World::numLeft() {
   return extras.size();
}

void World::drawObject(Object* obj) {
   // TODO Get the model-view matrix for this object
   glm::mat4 matrix = obj->getModelMatrix();

   // Extract the planes of the view frustum
   glm::vec4* planes = (glm::vec4*) calloc(6, sizeof(glm::vec4));
   extractViewFrustumPlanes(planes, matrix);

   // TODO Check if the object is in the view frustum

   // Free the planes (stops memory leaks)
   free(planes);
}

void World::extractViewFrustumPlanes(glm::vec4* planes, const glm::mat4 matrix) {
   // TODO Get the 6 planes of the view frustum from the model-view matrix
}

