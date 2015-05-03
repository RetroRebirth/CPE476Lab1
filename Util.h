#ifndef Util
#define Util

#include <cmath> 
#include "includes.h"

using namespace std;

namespace Util
{
    
    //Returns a random float 
    float randF()
    {
      return (float) rand() / RAND_MAX;
    }
    
    //Safely uses the glUniformMatrix4fv function
    void safe_glUniformMatrix4fv(const GLint handle, const GLfloat data[])
    {
      if(handle >= 0)
	glUniformMatrix4fv(handle, 1, GL_FALSE, data);
    }
    
    //Changes Degrees to radians
    float degreesToRadians(float degrees)
    {
      return degrees * 0.0174532925;
    }

    //changes radians to degrees.
    float radiansToDegrees(float radians)
    {
      return radians * 57.2957795;
    }
    
    void printVec3(glm::vec3 v)
    {
      printf("(%f, %f, %f)\n",v.x,v.y,v.z);
    }

    void printMat4(glm::mat4 m)
    {
      double dArray[16] = {0.0};

      const float *pSource = (const float*)glm::value_ptr(m);
      for (int i = 0; i < 16; ++i)
         dArray[i] = pSource[i];
   
      for (int i = 0; i < 16; ++i) {
         printf("%f ", dArray[i]);
         if ((i+1)%4 == 0) {
            printf("\n");
         }
      }
    }
}

#endif
