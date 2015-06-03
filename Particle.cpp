//
// sueda
// November, 2014
//
#include "Particle.h"

using namespace std;

Particle::Particle() :
	charge(1.0f),
	m(1.0f),
	d(0.0f),
	x(0.0f, 0.0f, 0.0f),
	v(0.0f, 0.0f, 0.0f),
	lifespan(1.0f),
	tEnd(0.0f),
	scale(1.0f),
	color(1.0f, 1.0f, 1.0f, 1.0f),
	posBufID(0),
	texBufID(0),
	indBufID(0)
{
   // custom attributes
	startPos = glm::vec3(0.0f,2.0f,0.0f);
	customPos = false;
	startVel = glm::vec3(randFloat(-0.1f, 0.1f), randFloat(-0.1f, 0.1f), randFloat(-0.1f, 0.1f));
	customVel = false;
	startCol = glm::vec3(randFloat(0.0f, 1.0f), randFloat(0.0f, 1.0f), randFloat(0.0f, 1.0f));
	customCol = false;
	startOpacity = 1.0f;
	customOpacity = false;
	startTTL = 50.0f;
	customTTL = false;
	startMass = 1.0f;
	customMass = false;
	startScale = 0.25f;
	customScale = false;
	customUpdate = false;
	randPosListSize = 0;
	taperOpacity = true;
	
	cycles = 0;
	
	time = 0.0f;
	startTime = 0.0f;
	drawNow = false;
	oneCycle = false;
}

Particle::~Particle()
{
}

void Particle::load()
{
	// Load geometry
	// 0
	posBuf.push_back(-0.5f);
	posBuf.push_back(-0.5f);
	posBuf.push_back(0.0f);
	texBuf.push_back(0.0f);
	texBuf.push_back(0.0f);
	// 1
	posBuf.push_back(0.5f);
	posBuf.push_back(-0.5f);
	posBuf.push_back(0.0f);
	texBuf.push_back(1.0f);
	texBuf.push_back(0.0f);
	// 2
	posBuf.push_back(-0.5f);
	posBuf.push_back(0.5f);
	posBuf.push_back(0.0f);
	texBuf.push_back(0.0f);
	texBuf.push_back(1.0f);
	// 3
	posBuf.push_back(0.5f);
	posBuf.push_back(0.5f);
	posBuf.push_back(0.0f);
	texBuf.push_back(1.0f);
	texBuf.push_back(1.0f);
	// indices
	indBuf.push_back(0);
	indBuf.push_back(1);
	indBuf.push_back(2);
	indBuf.push_back(3);
	
	// Random initialization
	rebirth(0.0f);
}

void Particle::init(Program *prog)
{
	this->prog = prog;
	
	// Send the position array to the GPU
	glGenBuffers(1, &posBufID);
	glBindBuffer(GL_ARRAY_BUFFER, posBufID);
	glBufferData(GL_ARRAY_BUFFER, posBuf.size()*sizeof(float), &posBuf[0], GL_STATIC_DRAW);
	
	// Send the texture coordinates array to the GPU
	glGenBuffers(1, &texBufID);
	glBindBuffer(GL_ARRAY_BUFFER, texBufID);
	glBufferData(GL_ARRAY_BUFFER, texBuf.size()*sizeof(float), &texBuf[0], GL_STATIC_DRAW);
	
	// Send the index array to the GPU
	glGenBuffers(1, &indBufID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBufID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indBuf.size()*sizeof(unsigned int), &indBuf[0], GL_STATIC_DRAW);
	
	// Unbind the arrays
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	rebirth(0.0f);
	//assert(glGetError() == GL_NO_ERROR);
}

void Particle::draw(MatrixStack *MV) const
{
	// Don't draw if at the origin
	if(glm::dot(x, x) < 0.01f) {
		//return;
	}
	
   glEnable(GL_TEXTURE_2D);
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
   
   glActiveTexture(GL_TEXTURE0);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glBindTexture(GL_TEXTURE_2D, texture_id);
	
	// Enable and bind position array for drawing
	GLint h_pos = prog->getAttribute("vertPosition");
	GLSL::enableVertexAttribArray(h_pos);
	glBindBuffer(GL_ARRAY_BUFFER, posBufID);
	glVertexAttribPointer(h_pos, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	// Enable and bind texcoord array for drawing
	GLint h_tex = prog->getAttribute("vertTexCoords");
	GLSL::enableVertexAttribArray(h_tex);
	glBindBuffer(GL_ARRAY_BUFFER, texBufID);
	glVertexAttribPointer(h_tex, 2, GL_FLOAT, GL_FALSE, 0, 0);
	
	// Bind index array for drawing
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBufID);
	
	// Transformation matrix
	MV->pushMatrix();
	//printf("Drawing particle at: ");
	//printVec3(x);
	MV->translate(x);
	glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
	MV->popMatrix();
	
	// Color and scale
	glUniform4fv(prog->getUniform("color"), 1, glm::value_ptr(color));
	glUniform1f(prog->getUniform("scale"), scale);
	
	// Draw
	glDrawElements(GL_TRIANGLE_STRIP, (int)indBuf.size(), GL_UNSIGNED_INT, 0);
	
	// Disable and unbind
	GLSL::disableVertexAttribArray(h_tex);
	GLSL::disableVertexAttribArray(h_pos);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Particle::rebirth(float t)
{
	charge = 1.0f;
	if (customMass) {
	   m = startMass;
	}
	else {
	   m = 1.0f;
	}
	d = 0.0f;
	if (customPos) {
	   x.x = startPos.x;
	   x.y = startPos.y;
	   x.z = startPos.z;
	}
	else {
	   x.x = 0.0f;
	   x.y = 2.5f; 
	   x.z = 0.0f;
	}
	if (randPosListSize > 0) {
	   x = randPosList[(int)(randFloat(0.0f, (float)(randPosListSize-1)) + 0.5f)];
	}
	if (customVel) {
	   v.x = startVel.x;
	   v.y = startVel.y;
	   v.z = startVel.z;
	}
	else {
	   v.x = randFloat(-0.05f, 0.05f);
	   v.y = randFloat(0.0f, 0.1f);
	   v.z = randFloat(-0.05f, 0.05f);
	}
	if (customTTL) {
	   lifespan = startTTL;
	}
	else {
	   lifespan = 50.0f;
	}
	tEnd = t + lifespan;
	if (customScale) {
	   scale = startScale;
	}
	else {
	   scale = 0.25f;
	}
	if (customCol) {
	   color.x = startCol.x;
	   color.y = startCol.y;
	   color.z = startCol.z;
	}
	else {
	   color.x = randFloat(0.0f, 1.0f);
	   color.y = randFloat(0.0f, 1.0f);
	   color.z = randFloat(0.0f, 1.0f);
	}
	if (customOpacity) {
	   color.w = startOpacity;
	}
	else {
	   color.w = 1.0f;
	}
	time = 0.0f;
}

void Particle::update(float t, float h, const glm::vec3 &g)
{
   if (!drawNow) {
      if (time >= startTime) {
         if (!oneCycle) {
            //cycles++;
            rebirth(t);
         }
         drawNow = true;
         return;
      }
   }
   
	if (time > lifespan) {
	   drawNow = false;
	   if (!oneCycle) {
	      cycles++;
	      rebirth(t);
	   }
	   return;
	}
	
	glm::vec3 grav = m*g; // force due to grav
	
	if (taperOpacity) {
	   color.w = time/lifespan;//(tEnd-t)/(lifespan);
	}
	
	if (customUpdate) {
	   customUpdateFunc(&x, &v, &grav, &color, time);
	}
	else {
	   glm::vec3 vf = v + time*grav;
	   x += vf;
	}
	time += h;
}

void Particle::setRandPosList(glm::vec3* list, int s) {
   randPosList = list;
   randPosListSize = s;
}

void Particle::setStartPos(glm::vec3 p) {
   startPos = p;
   customPos = true;
}

void Particle::setStartVel(glm::vec3 v) {
   startVel = v;
   customVel = true;
}

void Particle::setStartCol(glm::vec3 c) {
   startCol = c;
   customCol = true;
}

void Particle::setStartOpacity(float o) {
   startOpacity = o;
   customOpacity = true;
}

void Particle::setStartTTL(float ttl) {
   startTTL = ttl;
   customTTL = true;
}

void Particle::setStartMass(float mass) {
   startMass = mass;
   customMass = true;
}

void Particle::setStartScale(float s) {
   startScale = s;
   customScale = true;
}

void Particle::setUpdateFunc(void (*func)(glm::vec3*,glm::vec3*,glm::vec3*,glm::vec4*,float)) {
   customUpdateFunc = func;
   customUpdate = true;
}
