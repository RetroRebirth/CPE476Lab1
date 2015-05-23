#ifndef __WatermelonSmash__
#define __WatermelonSmash__

#include "includes.h"
#include "Clicks.h"
#include "Texture.h"
#include "Window.h"
#include "Sound.h"

#define BULLET_SPD 20.0
#define MELON_DEPTH 4.0
#define MELON_TIME 30.0
#define MELON_SPAWN 0.5
#define MELON_LEFT -2.5
#define MELON_RIGHT 2.5
#define MELON_SWING 0.5

class Watermelon {
public:
    // constructor & deconstructor
    Watermelon(Object *obj, float x) {
        object = obj;
        xPos = x;
        
        // decide how big to make the watermelon
        int num = Util::randF() * 10 + 1;
        if (num <= 5) {         // small = 50% chance, 2 hit break
            maxHits = 2;
            points = 10;
            size = 2.0;
            yPos = 1.3;
        }
        else if (num <= 9) {    // medium = 40% chance, 7 hit break
            maxHits = 7;
            points = 20;
            size = 2.5;
            yPos = 1.5;
        }
        else if (num <= 10) {   // large = 10% chance, 15 hit break
            maxHits = 15;
            points = 30;
            size = 3.0;
            yPos = 2.0;
        }
        hits = maxHits;
        scale = (size - 1.0)/(maxHits - 1);
        yScale = (yPos - 1.0)/(maxHits - 1);
        
        object->setPos(glm::vec3(xPos, yPos, MELON_DEPTH));
        object->scale(glm::vec3(size, size, size));
        object->updateRadius();
    }
    virtual ~Watermelon() {
        delete object;
    }
    
    // hits the watermelon and returns the score earned
    int hit() {
        int pEarned = 1;
        hits--;
        size -= scale;
        yPos -= yScale;
        
        // determine if the shell was broken
        if (hits == 1) {
            object->setTexture(TEX_MELON_IN);
        }
        // determine if the watermelon was broken
        else if (hits == 0) {
            pEarned = points;
        }
        
        object->setPos(glm::vec3(xPos, yPos, MELON_DEPTH));
        object->scale(glm::vec3(size, size, size));
        object->updateRadius();

        return pEarned;
    }
    
    // getters
    Object *object;
    float xPos, yPos, size;
    
private:
    vector<tinyobj::shape_t> shapes;
    vector<tinyobj::material_t> materials;
    int hits, maxHits, points;
    float scale, yScale;
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
    vector<Object*> misc_objects;
    vector<tinyobj::shape_t> shapes;
    vector<tinyobj::material_t> materials;
    GLuint ShadeProg;
    Clicks* clicks;
    Sound* sound;
    Object* hammer;
    
    // Minigame variables
    vector<Watermelon*> melons;
    int score;
    double timeStart, timer, timeLeft, timeRight, timeSwing;
    bool spawnLeft, spawnRight;
    
    // Minigame functions
    void newMelon(float xPos);
    void checkTime(Window *window);
    void setUp();
};

#endif
