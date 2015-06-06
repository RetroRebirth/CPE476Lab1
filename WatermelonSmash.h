#ifndef __WatermelonSmash__
#define __WatermelonSmash__

#include <queue>
#include <algorithm>
#include <functional>
#include "includes.h"
#include "Clicks.h"
#include "Texture.h"
#include "Window.h"
#include "Sound.h"
#include "Program.h"
#include "FontEngine.h"
#include "Particle.h"
#include "Camera.h"
#include "ParticleSorter.h"

#define BULLET_SPD 20.0
#define MELON_DEPTH 4.0
#define MELON_TIME 30.0
#define MELON_SPAWN 0.5
#define MELON_LEFT -2.5
#define MELON_RIGHT 2.5
#define MELON_SWING 0.5
#define NUM_PARTICLES 50
#define EXPLOSION_TIME 50

class Watermelon {
public:
    // constructor & deconstructor
    Watermelon(Camera* _camera, Object *obj, float x) {
        camera = _camera;
        object = obj;
        xPos = x;
        
        // decide how big to make the watermelon
        int num = Util::randF() * 10 + 1;
        if (num <= 5) {         // small = 50% chance, 2 hit break
            maxHits = 2;
            points = 5;
            size = 2.0;
            yPos = 1.3;
            lifeSpan = 3;
        }
        else if (num <= 9) {    // medium = 40% chance, 7 hit break
            maxHits = 6;
            points = 20;
            size = 2.5;
            yPos = 1.5;
            lifeSpan = 4;
        }
        else if (num <= 10) {   // large = 10% chance, 15 hit break
            maxHits = 15;
            points = 50;
            size = 3.0;
            yPos = 2.0;
            lifeSpan = 5;
        }
        hits = maxHits;
        scale = (size - 1.0)/(maxHits - 1);
        yScale = (yPos - 1.0)/(maxHits - 1);
        
        object->setPos(glm::vec3(xPos, yPos, MELON_DEPTH));
        object->scale(glm::vec3(size, size, size));
        object->updateRadius();
        
        isRed = false;
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
            object->setTexture(textures[TEX_MELON_IN]);
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
    float hits, xPos, yPos, size, lifeSpan;
    bool isRed;
    
private:
    Camera* camera;
    vector<tinyobj::shape_t> shapes;
    vector<tinyobj::material_t> materials;
    int maxHits, points;
    float scale, yScale;
    float timer;
};

class WatermelonSmash {
public:
    // constructor & deconstructor
    WatermelonSmash(GLuint _ShadeProg, Program* _particleProg, Camera* _camera, Sound* _sound);
    virtual ~WatermelonSmash();
    
    // clicks
    void mouseClick(glm::vec3 direction, glm::vec4 point);
    // called by main program
    void step(Window* window);
    
    // getters and setters
    bool gameOver, gameStart;
    
private:
    // Variables
    Program *particleProg;
    Camera* camera;
    
    vector<Object*> bullets;
    vector<Object*> misc_objects;
    vector<tinyobj::shape_t> shapes;
    vector<tinyobj::material_t> materials;
    
    // Paticles
    vector<vector<Particle*> > fireworks;
    int numFireworks;
    // time info for particles
    float t, t0_disp, t_disp;
    float h;
    glm::vec3 g;
    
    GLuint ShadeProg;
    Sound* sound;
    Object *hammer;
    
    // Minigame variables
    vector<Watermelon*> melons;
    int score, numMelons, timeLMelon, timeRMelon;
    double timeStart, timer, timeLeft, timeRight, timeSwing;
    double ageRight, ageLeft;
    bool spawnLeft, spawnRight;
    
    // Minigame functions
    void newMelon(float xPos);
    void checkTime(Window *window);
    void setUp();
    void addNewFirework(glm::vec3 pos, float melonSize);
    void particleStep();
    
    // Text functions
    void textStep(Window* window);
    void printInstructions();
};

#endif
