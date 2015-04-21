#include "includes.h"
#include "Camera.h"

class SkyBox
{
   public:
      SkyBox(GLuint ShadeProg);
      virtual ~SkyBox();

      void init();
      void draw(Camera* camera, Window* window);

   private:
      GLint h_aPos, h_aNor, h_uM;
      GLint h_uTexUnit, h_aTexCoord;
      bufID_t boxBufIDs; // pos = pos, ind = ind, nor = textures
};


