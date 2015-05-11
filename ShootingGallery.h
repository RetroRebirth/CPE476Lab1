#ifndef __ShootingGallery__
#define __ShootingGallery__

#include "includes.h"
#include "Clicks.h"
#include "Texture.h"
#include "Window.h"

#define DEPTH 10.0
#define WIDTH 20.0
#define HEIGHT 15.0
#define BULLET_SPD 20.0

class ShootingGallery {
    public:
        ShootingGallery(GLuint _ShadeProg, Clicks* _clicks);
        virtual ~ShootingGallery();
        void mouseClick(glm::vec3 direction);
        void step(Window* window);

    private:
        vector<Object*> targets;
        vector<tinyobj::shape_t> shapes;
        vector<tinyobj::material_t> materials;
        GLuint ShadeProg;
        Clicks* clicks;
        Object* wall;
        int score;
        double time;
        void newTarget();
        vector<Object*> bullets;
        void makeBullets();
};

#endif
