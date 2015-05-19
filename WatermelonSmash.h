#ifndef __WatermelonSmash__
#define __WatermelonSmash__

#include "includes.h"
#include "Clicks.h"
#include "Texture.h"
#include "Window.h"
#include "Sound.h"

#define BULLET_SPD 20.0
#define MELON_DEPTH 5.0
#define MELON_TIME 30.0
#define MELON_SPAWN 0.5
#define MELON_LEFT -2.5
#define MELON_RIGHT 2.5

class Watermelon {
public:
    // constructor & deconstructor
    Watermelon(Object *obj) {
        object = obj;
        
        // decide how big to make the watermelon
        int num = Util::randF() * 10 + 1;
        if (num <= 5) {
            maxSize = 2;
            scale = 1.0;
        }
        else if (num <= 8) {
            maxSize = 8;
            scale = 2.0;
        }
        else if (num <= 10) {
            maxSize = 18;
            scale = 3.0;
        }
        size = maxSize;
        
        object->scale(glm::vec3(size/scale, size/scale, size/scale));
        object->updateRadius();
    }
    virtual ~Watermelon() {
        delete object;
    }
    
    // hits the watermelon and returns the score earned
    int hit() {
        int points = 1;

        size--;
        // determine if the shell was broken
        if (size == 1) {
            object->setTexture(TEX_MELON_IN);
        }
        // determine if the watermelon was broken
        if (size == 0) {
            points = (maxSize / 2) * 10;
        }
        object->scale(glm::vec3(size/scale, size/scale, size/scale));
        object->updateRadius();

        return points;
    }
    /* TO DO -- implement more accurate collision */
    // checks whether the bullet hit the watermelon
    bool collided(Object *bullet) {
        bool collided = false;
        
        return collided;
    }
    
    // getters
    Object *object;
    float xPos;
    
private:
    vector<tinyobj::shape_t> shapes;
    vector<tinyobj::material_t> materials;
    int size, maxSize;
    float scale;
    glm::vec3 position;
};

class WatermelonSmash {
public:
    // constructor & deconstructor
    WatermelonSmash(GLuint _ShadeProg, Clicks* _clicks, Sound* _sound);
    virtual ~WatermelonSmash();
    
    // clicks
    void mouseClick(glm::vec3 direction, glm::vec4 point);
    // called by main program
    void step(Window* window);
    
    // getters and setters
    bool gameOver, gameStart;
    
private:
    // Variables
    vector<Object*> bullets;
    vector<tinyobj::shape_t> shapes;
    vector<tinyobj::material_t> materials;
    GLuint ShadeProg;
    Clicks* clicks;
    Sound* sound;
    Object* wall;
    
    // Minigame variables
    vector<Watermelon*> melons;
    int score;
    double timeStart, timer, timeLeft, timeRight;
    bool spawnLeft, spawnRight;
    
    // Minigame functions
    void newMelon(float xPos);
    void checkTime(Window *window);
    void setUp();
};

#endif