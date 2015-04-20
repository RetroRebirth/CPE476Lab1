#ifndef __World__
#define __World__

#include "includes.h"
#include "Object.h"
#include "Window.h"
#include "Camera.h"

#define EXTRA_FILE_NAME "bunny.obj"

class World {
   public:
      int numCollected;

      World(
         GLint _h_uAClr,
         GLint _h_uDClr,
         GLint _h_uSClr,
         GLint _h_uS,
         GLint _h_uM,
         GLint _h_aPos,
         GLint _h_aNor);
      virtual ~World();

      void step(Camera *camera, Window* window);
      void initGround();
      void drawGround();
      int numLeft();
   private:
      vector<tinyobj::shape_t> shapes; // TODO map of mesh data
      vector<Object*> extras; // bunnies for now.
      vector<tinyobj::material_t> materials;
      GLint h_aPos, h_aNor;
      GLint h_uM;
      GLint h_uAClr, h_uDClr, h_uSClr, h_uS;
      bufID_t groundBufIDs;
      vector<Object*> objects;
      double objStartTime;

      inline void safe_glUniformMatrix4fv(const GLint handle, const GLfloat data[]);
      void createObject(const string &meshName);
};

#endif
