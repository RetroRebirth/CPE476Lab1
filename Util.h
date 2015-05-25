#ifndef Util
#define Util

#include <cmath> 
//#include "includes.h"

using namespace std;

namespace Util
{
    //Returns a random float 
    float randF()
    {
      return (float) rand() / RAND_MAX;
    }
    
    float randFloat(float l, float h)
    {
	   float r = rand() / (float)RAND_MAX;
	   return (1.0f - r) * l + r * h;
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
    
    bool cmpVec3(glm::vec3 a, glm::vec3 b) {
      if (a.x == b.x && a.y == b.y && a.z == b.z) {
         return true;
      }
      return false;
    }

    /* Clicking stuff */
    glm::vec3 mouse_click(int mouse_x, int mouse_y, int height, int width, glm::mat4 Position, glm::mat4 View, glm::vec3 o){
      float x = (2.0f * mouse_x) / width - 1.0f;
      float y = 1.0f - (2.0f * mouse_y) / height;
      glm::vec4 ray_clip = glm::vec4 (x, y, -1.0f, 1.0);
      glm::vec4 ray_eye = glm::inverse(Position) * ray_clip; 
      ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);
      glm::vec4 ray_hol = (glm::inverse(View) * ray_eye);
      glm::vec3 ray_wor = glm::vec3 (ray_hol.x, ray_hol.y, ray_hol.z);
      
      //THE DIRECTION OF THE CLICK
      ray_wor = glm::normalize (ray_wor);
      return ray_wor; 
      //How to check: use checkAgainstSphere (the area the center is, radius, ray_wor, o);
    }

    bool checkAgainstSphere(glm::vec3 center, float radius, glm::vec3 direction, glm::vec3 o){
      glm::vec3 infoHold = glm::vec3(o.x - center.x, o.y - center.y, o.z - center.z);
      float b = glm::dot(direction, infoHold);
      float c = glm::dot(infoHold, infoHold) - (radius * radius);
      if(b * b - c >= 0){
	      return true;
      }
      else{
	      return false;
      }
    }
}

#endif
