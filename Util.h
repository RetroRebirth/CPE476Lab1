#ifndef Util
#define Util

#include <cmath> 
namespace Util
{
    
    float randF()
    {
      return (float) rand() / RAND_MAX;
    }

    void safe_glUniformMatrix4fv(const GLint handle, const GLfloat data[])
    {
      if(handle >= 0)
	glUniformMatrix4fv(handle, 1, GL_FALSE, data);
    }
}

#endif
