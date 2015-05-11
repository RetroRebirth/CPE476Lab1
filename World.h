#ifndef __World__
#define __World__

#include "includes.h"
#include "Object.h"
#include "Window.h"
#include "Camera.h"
#include "SkyBox.h"
#include "Booth.h"

class World {
   public:
      int numCollected;

      World(GLuint ShadeProg, Camera* _camera);
      virtual ~World();

      void step(Window* window);
      void initGround();
      void drawGround();
      int numLeft();
      Booth* currentActiveBooth();
      void inMiniGame();
      void leftMiniGame();
      
      bool drawWorld;
      
   private:
      vector<tinyobj::shape_t> shapes; // TODO map of mesh data
      vector<Object*> extras; // bunnies for now.
      vector<Booth*> booths;
      vector<Object*> structures;
      vector<Object*> lanterns;
      vector<tinyobj::material_t> materials;
      
      Object* player;
      float playerXZRad;
      
      GLuint ShadeProg;
      GLint h_aPos, h_aNor;
      GLint h_uM;
      GLint h_uAClr, h_uDClr, h_uSClr, h_uS;
      GLint h_uTexUnit;
      vector<Object*> objects;
      double objStartTime;
      bool inGame;

      SkyBox* skybox;
      Object* ground;
      Camera* camera;

      inline void safe_glUniformMatrix4fv(const GLint handle, const GLfloat data[]);
      void createPlayer(const string &meshName);
      void createExtras(const string &meshName);
      void setupOverWorld();
      void drawOverWorld();
      bool detectSpawnCollision(Object* object);
      void parseMapFile(const char* fileName);
      void drawObject(Object* obj);
      void extractViewFrustumPlanes(glm::vec4* planes, const glm::mat4 matrix);
      void normalizePlane(glm::vec4& plane);
      bool checkPlane(glm::vec4 plane, glm::vec3 pos, float rad);
};

#endif
