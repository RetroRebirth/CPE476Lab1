#include "includes.h"
#include "Camera.h"

class SkyBox
{
   public:
      SkyBox(GLint _h_aPos,
             GLint _h_aNor,
             GLint _h_uM,
             GLint _h_uTexUnit,
             GLint _h_aTexCoord);
      virtual ~SkyBox();

      void init();
      void draw(Camera* camera, Window* window);

   private:
      GLint h_aPos, h_aNor, h_uM;
      bufID_t boxBufIDs; // pos = pos, ind = ind, nor = textures
      GLint h_uTexUnit, h_aTexCoord;
};


