#ifndef __ShootingGallery__
#define __ShootingGallery__

#include "includes.h"
#include "Clicks.h"

class ShootingGallery {
   public:
       ShootingGallery(GLuint _ShadeProg, Clicks* _clicks);
       virtual ~ShootingGallery();

       void step();
   private:
      vector<Object*> objects;
      vector<tinyobj::shape_t> shapes;
      vector<tinyobj::material_t> materials;
      GLuint ShadeProg;
      Clicks* clicks;
};

#endif
