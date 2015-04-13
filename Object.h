#ifndef __Object__
#define __Object__

#include "includes.h"

class Object {
   public:
      glm::vec3 pos;
      glm::vec3 dir;
      float vel;
      bool hit;
      glm::vec3 col;
      float shine;

      Object(
         vector<tinyobj::shape_t> &_shapes,
         vector<tinyobj::material_t> &_materials,
         GLint _h_uAClr,
         GLint _h_uDClr,
         GLint _h_uSClr,
         GLint _h_uS,
         GLint _h_uM,
         GLint _h_aPos,
         GLint _h_aNor);
      virtual ~Object();

      void step(float dt);
      void draw();
   private:
      vector<tinyobj::shape_t> shapes;
      vector<tinyobj::material_t> materials;
      bufID_t bufIDs;
      GLint h_aPos, h_aNor;
      GLint h_uM;
      GLint h_uAClr, h_uDClr, h_uSClr, h_uS;

      vector<float> computeNormals(vector<float> posBuf, vector<unsigned int> indBuf);
      float randF();
      inline void safe_glUniformMatrix4fv(const GLint handle, const GLfloat data[]);
};

#endif
