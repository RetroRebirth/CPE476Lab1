#include "includes.h"
#include "Camera.h"
#include "Object.h"

class SkyBox
{
   public:
      SkyBox(GLuint ShadeProg);
      virtual ~SkyBox();

      void init();
      void draw(Camera* camera, Window* window);

   private:
      GLuint ShadeProg;
      GLint h_aPos, h_aNor, h_uM, h_uTexUnit;

      Object* skydome;
      vector<tinyobj::shape_t> shapes;
      vector<tinyobj::material_t> materials;
};


