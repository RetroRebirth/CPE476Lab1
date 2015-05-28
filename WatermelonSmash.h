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
#include "ParticleSorter.h"
#include "Camera.h"

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
    Watermelon(Program* _particleProg, Camera* _camera, Object *obj, float x) {
        particleProg = _particleProg;
        camera = _camera;
        
        object = obj;
        xPos = x;
        
        // decide how big to make the watermelon
        int num = Util::randF() * 10 + 1;
        if (num <= 5) {         // small = 50% chance, 2 hit break
            maxHits = 2;
            points = 10;
            size = 2.0;
            yPos = 1.3;
            lifeSpan = 3;
        }
        else if (num <= 9) {    // medium = 40% chance, 7 hit break
            maxHits = 7;
            points = 20;
            size = 2.5;
            yPos = 1.5;
            lifeSpan = 4;
        }
        else if (num <= 10) {   // large = 10% chance, 15 hit break
            maxHits = 15;
            points = 30;
            size = 3.0;
            yPos = 2.0;
            lifeSpan = 5;
        }
        hits = maxHits;
        scale = (size - 1.0)/(maxHits - 1);
        yScale = (yPos - 1.0)/(maxHits - 1);
        
        // initialize time and gravity for particles
        t = 0.0f;
        t0_disp = 0.0f;
        t_disp = 0.0f;
        h = 1.0f;
        g = glm::vec3(0.0f, -0.01f, 0.0f);
        initParticles();
        
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
        
        explosionsStarted.push_back(explosions.front());
        explosions.pop();

        return pEarned;
    }
    
    vector<vector<Particle*> > explosionsStarted;
    
    void particleStep() {
       // Display every 60 Hz
	   t += h;
      
      // Create matrix stacks
	   MatrixStack P, MV;
	   // Apply camera transforms
	   P.pushMatrix();
	   camera->applyProjectionMatrix(&P);
	   MV.pushMatrix();
	   camera->applyViewMatrix(&MV);
	
	   // Bind the program
	   particleProg->bind();
	   ParticleSorter sorter;
	
	   glUniformMatrix4fv(particleProg->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P.topMatrix()));
	   
	   int j = 0;
	   for(vector<vector<Particle*> >::iterator it1 = explosionsStarted.begin(); it1 != explosionsStarted.end(); ++it1) {
	      vector<Particle*> v = *it1;
	      
	      // sort the explosions' Particles from back to front
         MatrixStack temp;
         camera->applyViewMatrix(&temp);
         glm::mat4 V = temp.topMatrix();
   
         sorter.C = glm::transpose(glm::inverse(V)); // glm is transposed!
         std::sort(v.begin(), v.end(), sorter);
               
	      for (int i=0; i<v.size(); ++i) {
	         v[i]->update(t, h, g);
	         if (v[i] != NULL && v[i]->cycles < 1) {
		         v[i]->draw(&MV);
		      }
		   }
		   j++;
	   }
	   
	   // Unbind the program
	   particleProg->unbind();
   }
    
    // getters
    Object *object;
    float xPos, yPos, size, lifeSpan;
    
private:
    Program* particleProg;
    Camera* camera;
    vector<tinyobj::shape_t> shapes;
    vector<tinyobj::material_t> materials;
    queue<vector<Particle*> > explosions;
    int hits, maxHits, points;
    float scale, yScale;
    float timer;
    
    // time info for particles
    float t, t0_disp, t_disp;
    float h;
    glm::vec3 g;
    
    void initParticles() {
      explosionsStarted.clear();
      // load explosion Particles
	   for(int j = 0; j < maxHits; ++j) {
	      vector<Particle*> explosion;
	      explosion.clear();
	      for (int i = 0; i < NUM_PARTICLES; ++i) {
		      Particle* particle = new Particle(); // !C++11: Particle *particle = new Particle();
		      particle->load();
		      particle->setTexture(TEX_PARTICLE);
		      particle->setStartPos(glm::vec3(xPos, yPos, MELON_DEPTH));
		      glm::vec3 vel = glm::vec3(randFloat(-1.0f, 1.0f), randFloat(-1.0f, 1.0f), randFloat(-1.0f, 1.0f));
		      
		      particle->setStartVel(glm::normalize(vel)/2.0f);
		      particle->setStartScale(3.0f);
		      if (i%2 == 0) {
		         particle->setStartCol(glm::vec3(0.8f, 0.1f, 0.1f));
		      }
		      else {
		         particle->setStartCol(glm::vec3(0.1f, 0.8f, 0.1f));
		      }
		      particle->setStartTTL(EXPLOSION_TIME);
		      explosion.push_back(particle);
	
            explosion[i]->init(particleProg);
         }
         explosions.push(explosion);
      }
   }
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
    Program* particleProg;
    Camera* camera;
    
    vector<Object*> bullets;
    vector<Object*> misc_objects;
    vector<tinyobj::shape_t> shapes;
    vector<tinyobj::material_t> materials;
    
    GLuint ShadeProg;
    Sound* sound;
    Object* hammer;
    
    // Minigame variables
    vector<Watermelon*> melons;
    int score, numMelons;
    double timeStart, timer, timeLeft, timeRight, timeSwing;
    double ageRight, ageLeft;
    bool spawnLeft, spawnRight;
    
    // Minigame functions
    void newMelon(float xPos);
    void checkTime(Window *window);
    void setUp();
    void printInstructions();
};

#endif
