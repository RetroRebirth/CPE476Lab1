#ifndef __Object__
#define __Object__

#include "includes.h"

class Object {
   public:
      glm::vec3 pos;
      glm::vec3 dir;
      float vel;
      bool collected;
      glm::vec3 col;
      float shine;
      float radius;

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

      void init();
      void load(const string &meshName);
      void resize_obj();
      
      void setPos(glm::vec3 position);
      void setDir(glm::vec3 direction);
      void setSpeed(float speed);
      
      /* functions dealing with matrix stack transformations */
      void scale(glm::vec3 scaler); // change the size relative to the normal size =)
      void rotate(float angle, glm::vec3 axis); // rotations other than the directional rotations
      void translate(glm::vec3 trans); // translations other than position change... be careful with this one
      
      void step(float dt);
      void draw();
      bool collidedWithPlayer(glm::vec3 camPos, float dt);
      bool collidedWithWall(float dt);
      bool collidedWithObj(Object o, float dt);
      glm::vec3 calculateNewPos(float dt);
   private:
      glm::mat4 modelMatrix;
      GLuint posBufID;
      GLuint indBufID;
      GLuint norBufID;
      vector<tinyobj::shape_t> shapes;
      vector<tinyobj::material_t> materials;
      bufID_t bufIDs;
      GLint h_aPos, h_aNor;
      GLint h_uM;
      GLint h_uAClr, h_uDClr, h_uSClr, h_uS;

      vector<float> computeNormals(vector<float> posBuf, vector<unsigned int> indBuf);
      inline void safe_glUniformMatrix4fv(const GLint handle, const GLfloat data[]);
      float calcYFacingAngle();
};

#endif
