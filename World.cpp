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
   if (!inGame) {
      for (int i=0; i<extras.size(); ++i) {
         Object* extra = extras[i];
         drawObject(extra);
         //extra->draw();
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
    ground->scale(glm::vec3(SIZE * 2, SIZE * 2, SIZE * 2));
    ground->setTexture(TEX_GROUND_SAKURA);
    ground->setShadows(false);
}

void World::drawGround() {
    ground->draw();
}

void World::drawOverWorld() {
   for (int i=0; i<structures.size(); ++i) {
      Booth* structure = structures[i];
       /*
       Object **bObjs = structure->getBooths();
       for (int i = 0; i < 3; i++) {
           drawObject(bObjs[i]);
       }
       */
      structure->draw();
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
    extras.push_back(wall1);

    Object* wall2 = new Object(shapes, materials, ShadeProg);
    wall2->load(WALL_FILE_NAME);
    wall2->translate(glm::vec3(SIZE+0.5f, 2.5f, 0.0f));
    wall2->scale(glm::vec3(1.0f, 10.0f, SIZE*2.0f));
    wall2->setTexture(TEX_WOOD_WALL);
    extras.push_back(wall2);

    Object* wall3 = new Object(shapes, materials, ShadeProg);
    wall3->load(WALL_FILE_NAME);
    wall3->translate(glm::vec3(0.0f, 2.5f, -SIZE-0.5f));
    wall3->scale(glm::vec3(SIZE*2.0f, 10.0f, 1.0f));
    wall3->setTexture(TEX_WOOD_WALL);
    extras.push_back(wall3);

    Object* wall4 = new Object(shapes, materials, ShadeProg);
    wall4->load(WALL_FILE_NAME);
    wall4->translate(glm::vec3(0.0f, 2.5f, SIZE+0.5f));
    wall4->scale(glm::vec3(SIZE*2.0f, 10.0f, 1.0f));
    wall4->setTexture(TEX_WOOD_WALL);
    extras.push_back(wall4);
   
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
        // read in the lines;
        while (getline(mapFile, line)) {
            char type[10];
            
            // this line is a comment... skip it
            if (line[0] == '#') {
                continue;
            }
            // read the type of the object
            sscanf(line.c_str(), "%s\n", type);
            
            // add a new booth
            if (strcmp(type, "booth") == 0) {
                Booth *booth = new Booth(line.c_str(), shapes, materials, ShadeProg);
                structures.push_back(booth);
            }
            // add a new wall
            if (strcmp(type, "wall") == 0) {
                // TODO:something to do with the walls
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
   for (int i=0; i<extras.size(); ++i) {
      if (extras[i]->collision(object)) {
         return true;
      }
   }
   return false;
}

void World::createExtras(const string &meshName) {
   for (int i = 0; i < MAX_OBJS; ++i) {
      Object* extra = new Object(shapes, materials, ShadeProg);
      extra->load(meshName);
      extra->setPos(glm::vec3(2*Util::randF()*SIZE - SIZE, 1.0, 2*Util::randF()*SIZE - SIZE));
       /*
      while (detectSpawnCollision(extra)) {
         extra->setPos(glm::vec3(2*Util::randF()*SIZE - SIZE, 1.0, 2*Util::randF()*SIZE - SIZE));
      }
        */
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

//   return correctHalfSpace;// || clipping;
   return true; // TODO why does correctHalfSpace not work correctly?
}