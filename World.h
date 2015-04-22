#ifndef __World__
#define __World__

#include "includes.h"
#include "Object.h"
#include "Window.h"
#include "Camera.h"
#include "SkyBox.h"
#include "Booth.h"
#include "Player.h"

class World {
   public:
      int numCollected;

      World(GLuint ShadeProg, Player* _player);
      virtual ~World();

      void step(Camera *camera, Window* window);
      void initGround();
      void drawGround();
      int numLeft();
      
   private:
      vector<tinyobj::shape_t> shapes; // TODO map of mesh data
      vector<Object*> extras; // bunnies for now.
      vector<Booth*> structures;
      vector<tinyobj::material_t> materials;
      GLuint ShadeProg;
      GLint h_aPos, h_aNor;
      GLint h_uM;
      GLint h_uAClr, h_uDClr, h_uSClr, h_uS;
      GLint h_uTexUnit;
      bufID_t groundBufIDs;
      vector<Object*> objects;
      double objStartTime;

      SkyBox* skybox;
      Player* player;

      inline void safe_glUniformMatrix4fv(const GLint handle, const GLfloat data[]);
      void createPlayer(const string &meshName);
      void createExtra(const string &meshName);
      void setupOverWorld();
      void drawOverWorld();
};

#endif
