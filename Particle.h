// Particle stuff... default creates a firework!
// use custom setters to make a new particle system

#pragma once
#ifndef _PARTICLE_H_
#define _PARTICLE_H_
#include "includes.h"
#include "Program.h"

class MatrixStack;
class Program;
class Texture;

class Particle
{
public:
	Particle();
	virtual ~Particle();
	void load();
	void init(Program *prog);
	void draw(MatrixStack *MV) const;
	void rebirth(float t);
	void update(float t, float h, const glm::vec3 &g);
	const glm::vec3 &getPosition() const { return x; };
	const glm::vec3 &getVelocity() const { return v; };
   void setTexture(int tex)            { texture_id = tex;  }
   
   float startTime;
   bool drawNow;
   
   int cycles;
   
   // customization setters
   void setStartPos(glm::vec3 p);
   void setStartVel(glm::vec3 v);
   void setStartCol(glm::vec3 c);
   void setStartOpacity(float o);
   void setStartTTL(float ttl);
   void setStartMass(float mass);
   void setStartScale(float s);
   void setOpacityTaper(bool taper) { taperOpacity = taper; };
   void setOneCycle(bool c) { oneCycle = c; };
   
   // pass in a custom update function
   // argument must be a function pointer with the arguments:
   // void foo(glm::vec3* pos, glm::vec3* vel, glm::vec3* grav, glm::vec3* color, float time)
   void setUpdateFunc(void (*func)(glm::vec3*,glm::vec3*,glm::vec3*,glm::vec4*,float));
   void setRandPosList(glm::vec3* list, int s);
	
private:
	float charge; // +1 or -1
	float m; // mass
	float d; // viscous damping
	glm::vec3 x; // position
	glm::vec3 v; // velocity
	float lifespan; // how long this particle lives
	float tEnd;     // time this particle dies
	float scale;
	glm::vec4 color;
	std::vector<float> posBuf;
	std::vector<float> texBuf;
	std::vector<unsigned int> indBuf;
	GLuint posBufID;
	GLuint texBufID;
	GLuint indBufID;
	Program *prog;
	int texture_id;
	float time;
	
	// custom attributes
	//float updateTime;
	glm::vec3 startPos;
	bool customPos;
	glm::vec3 startVel;
	bool customVel;
	glm::vec3 startCol;
	float startOpacity;
	bool customCol;
	bool customOpacity;
	float startTTL;
	bool customTTL;
	float startMass;
	bool customMass;
	float startScale;
	bool customScale;
	bool taperOpacity;
	bool oneCycle;
	
	// custom updatestuff
	void (*customUpdateFunc)(glm::vec3*,glm::vec3*,glm::vec3*,glm::vec4*,float);
	bool customUpdate;
	
	glm::vec3* randPosList;
	int randPosListSize;
};

#endif
