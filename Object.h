#ifndef __Object__
#define __Object__

#include "includes.h"

class Object {
   public:
      Object();
      virtual ~Object();

      float randF();
      inline void safe_glUniformMatrix4fv(const GLint handle, const GLfloat data[]);
      void resize_obj(std::vector<tinyobj::shape_t> &shapes);
      void loadShapes(const string &objFile);
      vector<float> computeNormals(vector<float> posBuf, vector<unsigned int> indBuf);
      void randomTransform(transform_t &transform);
      void initModel(int objType, transform_t &transform);
      void initObj(int objType);
      void setMaterial(int i, GLint h_uAClr, GLint h_uDClr, GLint h_uSClr, GLint h_uS);
      void setModel(GLint h_uM);
      void drawObj(GLint h_uAClr, GLint h_uDClr, GLint h_uSClr, GLint h_uS, GLint h_uM, GLint h_aPos, GLint h_aNor);
};

#endif
