#include "ShootingGallery.h"

ShootingGallery::ShootingGallery(GLuint _ShadeProg, Clicks* _clicks) {
   ShadeProg = _ShadeProg;
   clicks = _clicks;

   // Initialize a sphere to test clicking on TODO remove
   Object* object = new Object(shapes, materials, ShadeProg);
   object->load("sphere.obj");
   object->setPos(glm::vec3(0.0, 2.0, 3.0));
   objects.push_back(object);
}

ShootingGallery::~ShootingGallery() {
   for (int i=0; i<objects.size(); ++i) { 
      delete objects[i];
   }
}

void ShootingGallery::step() {
   for (int i=0; i < objects.size(); ++i) {
      objects[i]->draw();
   }
}
