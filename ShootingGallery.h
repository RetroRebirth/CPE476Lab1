#ifndef __ShootingGallery__
#define __ShootingGallery__

#include "includes.h"
#include "Clicks.h"
#include "Texture.h"
#include "Window.h"

class ShootingGallery {
   public:
       ShootingGallery(GLuint _ShadeProg, Clicks* _clicks);
       virtual ~ShootingGallery();
    


       void step(Window* window);
   private:
      vector<Object*> objects;
      vector<tinyobj::shape_t> shapes;
      vector<tinyobj::material_t> materials;
      GLuint ShadeProg;
      Clicks* clicks;
    Object* wall;
    int score;
    //Adds a new target to the objects array
    void newTarget();
    vector<Object*> bullets;
    //makes bullets
    void makeBullets();
};

#endif
