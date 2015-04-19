#ifndef Util
#define Util

#include <cmath> 
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

}

#endif
