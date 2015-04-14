#ifndef __World__
#define __World__

#include "includes.h"
#include "Object.h"
#include "Window.h"

class World {
   public:
      World(
         GLint _h_uAClr,
         GLint _h_uDClr,
         GLint _h_uSClr,
         GLint _h_uS,
         GLint _h_uM,
         GLint _h_aPos,
         GLint _h_aNor);
      virtual ~World();

      void step(Window* window);
      void draw();
      void initGround();
      void drawGround();
   private:
      vector<tinyobj::shape_t> shapes; // TODO map of mesh data
      vector<tinyobj::material_t> materials;
      GLint h_aPos, h_aNor;
      GLint h_uM;
      GLint h_uAClr, h_uDClr, h_uSClr, h_uS;
      bufID_t groundBufIDs;
      vector<Object> objects;
      double objStartTime;
      int objCount;

      float randF();
      inline void safe_glUniformMatrix4fv(const GLint handle, const GLfloat data[]);
      void createObject();
      void resize_obj(std::vector<tinyobj::shape_t> &shapes);
      void loadShapes(const string &objFile);
};

#endif
