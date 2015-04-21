#include "World.h"

static string objectFiles[] = {"bunny.obj"};

World::World(GLuint _ShadeProg) {
   // Default attribute values
   objStartTime = 0.0;
   numCollected = 0;

   // Defined attribute values
   ShadeProg = _ShadeProg;
   h_aPos = GLSL::getAttribLocation(ShadeProg, "aPos");
   h_aNor = GLSL::getAttribLocation(ShadeProg, "aNor");
   h_uM = GLSL::getUniformLocation(ShadeProg, "uM");
   h_uAClr = GLSL::getUniformLocation(ShadeProg, "uAClr");
   h_uDClr = GLSL::getUniformLocation(ShadeProg, "uDClr");
   h_uSClr = GLSL::getUniformLocation(ShadeProg, "uSClr");
   h_uS = GLSL::getUniformLocation(ShadeProg, "uS");
   h_uTexUnit = GLSL::getUniformLocation(ShadeProg, "uTexUnit");

   skybox = new SkyBox(ShadeProg);
   initGround();
   setupOverWorld();
}

World::~World() {
   for (int i=0; i<objects.size(); ++i) { 
      delete objects[i];
   }
   for (int i=0; i<extras.size(); ++i) { 
      delete extras[i];
   }
}

void World::step(Camera *camera, Window* window) {
   // Create a new object every SECS_PER_OBJ
   if (numLeft() < MAX_OBJS && window->time - objStartTime >= SECS_PER_OBJ) {
      createExtra(EXTRA_FILE_NAME);
      objStartTime = window->time;
   }

// TODO I know I said looping with ints is bad but this link below proved otherwise. Use it if getting segfaults
// http://gamedev.stackexchange.com/questions/46584/how-to-remove-an-object-from-a-stdvector
   /*for (int i=0; i<activeObjects.size(); ++i) { 
      Object* obj1 = activeObjects[i];
      if (obj1->collidedWithPlayer(camera->calcNewPos(window), window->dt)) {
         // Delete object from collision detection
         activeObjects[i] = activeObjects.back();
         activeObjects.pop_back();
         --i;
         continue;
      }
      if (obj1->collidedWithWall(window->dt)) {
         continue;
      }
      for (int j=0; j<activeObjects.size(); ++j) { 
         Object* obj2 = activeObjects[j];
         if (obj1 == obj2) {
            continue;
         }
         if (obj1->collidedWithObj(*obj2, window->dt)) {
            break;
         }
      }
   }

   for (int i=0; i<objects.size(); ++i) { 
      bject* obj = objects[i];
      // If the object has finished shrinking, remove it
      if (obj->radius <= 0.0) {
         objects[i] = objects.back();
         objects.pop_back();
         --i;
         delete obj;
         // Increment the score
         numCollected++;
         continue;
      }
      if (obj->collidedWithPlayer(camera->calcNewPos(window), window->dt)) {
         // Stop the camera from moving into object
         camera->blocked = true;
      }
      // Update the object normally
      obj->step(window->dt);
   }*/
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

   camera->step(window);
   skybox->draw(camera, window);
}

inline void World::safe_glUniformMatrix4fv(const GLint handle, const GLfloat data[]) {
   if (handle >= 0)
      glUniformMatrix4fv(handle, 1, GL_FALSE, data);
}

void World::initGround() {

   // Load the texture for the ground
   loadTexture((char *)"ground_grass2.bmp", TEXTURE_GROUND);

   // Position array of ground
   GLfloat vertices[] = {
      -1.0f, 0.0, -1.0f, //0
      -1.0f, 0.0, +1.0f, //1
      +1.0f, 0.0, -1.0f, //2
      +1.0f, 0.0, +1.0f, //3
   };
   // Amplify ground size
   for (int i = 0, last = sizeof(vertices)/sizeof(GLfloat); i < last; i++) {
      // Don't amplify the floor height
      if (i % 3 != 1)
         vertices[i] *= SIZE;
   }
   glGenBuffers(1, &(groundBufIDs.pos));
   glBindBuffer(GL_ARRAY_BUFFER, groundBufIDs.pos);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

   // Index array of ground
   GLuint indices[] = {
      0, 1, 2,
      1, 3, 2,
   };
   glGenBuffers(1, &(groundBufIDs.ind));
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, groundBufIDs.ind);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

   // Normal array of ground
   vector<float> posBuf(vertices, vertices+sizeof(vertices)/sizeof(vertices[0]));
   vector<unsigned int> indBuf(indices, indices+sizeof(indices)/sizeof(indices[0]));

   glGenBuffers(1, &(groundBufIDs.nor));
   glBindBuffer(GL_ARRAY_BUFFER, groundBufIDs.nor);
}

void World::drawGround() {

   // Enable textures
   glEnable(GL_TEXTURE_2D);
   glActiveTexture(GL_TEXTURE0);
   glUniform1i(h_uTexUnit, 0);
   // Bind the ground texture
   glBindTexture(GL_TEXTURE_2D, TEXTURE_GROUND);

   // Bind position buffer
   glBindBuffer(GL_ARRAY_BUFFER, groundBufIDs.pos);
   glVertexAttribPointer(h_aPos, 3, GL_FLOAT, GL_FALSE, 0, 0);
   // Bind index buffer
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, groundBufIDs.ind);
   // Bind normal buffer
   glVertexAttribPointer(h_aNor, 3, GL_FLOAT, GL_FALSE, 0, 0);
   // Set color
   glUniform3f(h_uAClr, 0.13, 0.13, 0.14);
   glUniform3f(h_uDClr, 0.0, 0.0, 0.0);
   glUniform3f(h_uSClr, 0.0, 0.0, 0.0);
   glUniform1f(h_uS, 1.0);
   // Apply translation
   glm::vec3 trans = glm::vec3(0.0, 0.0f, 0.0);
   glm::mat4 T = glm::translate(glm::mat4(1.0f), trans);
   safe_glUniformMatrix4fv(h_uM, glm::value_ptr(T));

   glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void World::drawOverWorld() {
   for (int i=0; i<structures.size(); ++i) {
      Booth* structure = structures[i];
      structure->object->draw();
   }
}

void World::setupOverWorld() {
   Object* wall1 = new Object(shapes, materials, ShadeProg);
   wall1->load(WALL_FILE_NAME);
   wall1->translate(glm::vec3(-SIZE-0.5f, 2.5f, 0.0f));
   wall1->scale(glm::vec3(1.0f, 5.0f, SIZE*2.0f));
   Booth* bwall1 = new Booth(wall1, (const string*)"wall1");
   structures.push_back(bwall1);
   
   Object* wall2 = new Object(shapes, materials, ShadeProg);
   wall2->load(WALL_FILE_NAME);
   wall2->translate(glm::vec3(SIZE+0.5f, 2.5f, 0.0f));
   wall2->scale(glm::vec3(1.0f, 5.0f, SIZE*2.0f));
   Booth* bwall2 = new Booth(wall2, (const string*)"wall2");
   structures.push_back(bwall2);
   
   Object* wall3 = new Object(shapes, materials, ShadeProg);
   wall3->load(WALL_FILE_NAME);
   wall3->translate(glm::vec3(0.0f, 2.5f, -SIZE-0.5f));
   wall3->scale(glm::vec3(SIZE*2.0f, 5.0f, 1.0f));
   Booth* bwall3 = new Booth(wall3, (const string*)"wall3");
   structures.push_back(bwall3);
   
   Object* wall4 = new Object(shapes, materials, ShadeProg);
   wall4->load(WALL_FILE_NAME);
   wall4->translate(glm::vec3(0.0f, 2.5f, SIZE+0.5f));
   wall4->scale(glm::vec3(SIZE*2.0f, 5.0f, 1.0f));
   Booth* bwall4 = new Booth(wall4, (const string*)"wall4");
   structures.push_back(bwall4);
   
   Object* booth1 = new Object(shapes, materials, ShadeProg);
   booth1->load(WALL_FILE_NAME);
   booth1->translate(glm::vec3(0.0f, 0.0f, -15.0f));
   booth1->scale(glm::vec3(5.0f, 5.0f, 5.0f));
   //booth1->scale(glm::vec3(5.0f, 5.0f, 5.0f));
   Booth* bbooth1 = new Booth(booth1, (const string*)"booth1");
   structures.push_back(bbooth1);
   
   Object* booth2 = new Object(shapes, materials, ShadeProg);
   booth2->load(WALL_FILE_NAME);
   booth2->translate(glm::vec3(15.0f, 0.0f, 10.0f));
   booth2->scale(glm::vec3(5.0f, 5.0f, 5.0f));
   //booth1->scale(glm::vec3(5.0f, 5.0f, 5.0f));
   Booth* bbooth2 = new Booth(booth2, (const string*)"booth2");
   structures.push_back(bbooth2);
   
   Object* booth3 = new Object(shapes, materials, ShadeProg);
   booth3->load(WALL_FILE_NAME);
   booth3->translate(glm::vec3(-15.0f, 0.0f, 10.0f));
   booth3->scale(glm::vec3(5.0f, 5.0f, 5.0f));
   //booth1->scale(glm::vec3(5.0f, 5.0f, 5.0f));
   Booth* bbooth3 = new Booth(booth3, (const string*)"booth3");
   structures.push_back(bbooth3);
   
   // initialize world along with bounding boxes
   drawOverWorld();
   for (int i=0; i<structures.size(); ++i) {
      structures[i]->calculateBoundingBox();
   }
}

void World::createExtra(const string &meshName) {
   Object* extra = new Object(shapes, materials, ShadeProg);
   extra->load(meshName);
   extra->setPos(glm::vec3(2*Util::randF()*SIZE - SIZE, 1.0, 2*Util::randF()*SIZE - SIZE));
   extra->setDir(glm::normalize(glm::vec3(Util::randF()-0.5, 0.0, Util::randF()-0.5)));
   extra->setSpeed(OBJ_SPEED);
   extras.push_back(extra);
}

int World::numLeft() {
   return extras.size();
}
