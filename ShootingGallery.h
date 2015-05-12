#ifndef __ShootingGallery__
#define __ShootingGallery__

#include "includes.h"
#include "Clicks.h"
#include "Texture.h"
#include "Window.h"
#include "Sound.h"

#define DEPTH 3.0
#define BULLET_SPD 20.0
#define ROWS 2
#define COLS 4

class ShootingGallery {
    public:
        ShootingGallery(GLuint _ShadeProg, Clicks* _clicks, Sound* _sound);
        virtual ~ShootingGallery();
        void mouseClick(glm::vec3 direction);
        void step(Window* window);

    private:
        vector<Object*> targets;
        vector<tinyobj::shape_t> shapes;
        vector<tinyobj::material_t> materials;
        GLuint ShadeProg;
        Clicks* clicks;
        Sound* sound;
        Object* wall;
        int score;
        double time;
        void newTarget();
        vector<Object*> bullets;
};

#endif
