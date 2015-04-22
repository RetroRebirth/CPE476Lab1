#include "SkyBox.h"
using namespace std;

SkyBox::SkyBox(GLuint _ShadeProg)
{
   ShadeProg = _ShadeProg;

   // Defined attribute values
   h_aPos = GLSL::getAttribLocation(ShadeProg, "aPos");
   h_aNor = GLSL::getAttribLocation(ShadeProg, "aNor");
   h_uM = GLSL::getUniformLocation(ShadeProg, "uM");
   h_aTexCoord = GLSL::getAttribLocation(ShadeProg,  "aTexCoord");
   h_uTexUnit = GLSL::getUniformLocation(ShadeProg, "uTexUnit");

   init();
   loadTexture((char *)"cloud.bmp", TEXTURE_SKY);
}

SkyBox::~SkyBox()
{
   // Delete stuff
}

void SkyBox::init()
{
   skydome = new Object(shapes, materials, ShadeProg);
   skydome->load("sphere.obj");
   skydome->scale(glm::vec3(SKY_SIZE, SKY_SIZE, SKY_SIZE));
   skydome->setTexture((char *)"cloud.bmp", TEXTURE_SKY);

   // Texture coordinate mapping of sky box
   static GLfloat CubeTex[] = {
      0, 0, // back 
      0, 1,
      1, 1,
      1, 0,
      0, 0, //right 
      0, 1,
      1, 1,
      1, 0,
      0, 0, //front 
      0, 1,
      1, 1,
      1, 0,
      0, 0, // left 
      0, 1,
      1, 1,
      1, 0
   };

   glGenBuffers(1, &boxBufIDs.nor);
   glBindBuffer(GL_ARRAY_BUFFER, boxBufIDs.nor);
   glBufferData(GL_ARRAY_BUFFER, sizeof(CubeTex), CubeTex, GL_STATIC_DRAW);
}

void SkyBox::draw(Camera* camera, Window* window)
{
   glEnable(GL_TEXTURE_2D);
   glActiveTexture(GL_TEXTURE0);
   glUniform1i(h_uTexUnit, 0);

   // Bind the sky texture
   glBindTexture(GL_TEXTURE_2D, TEXTURE_SKY);

   // Bind texture buffer
   GLSL::enableVertexAttribArray(h_aTexCoord);
   glBindBuffer(GL_ARRAY_BUFFER, boxBufIDs.nor);
   glVertexAttribPointer(h_aTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0); 

   // Apply translation
   glm::vec3 trans = camera->pos;
   glm::mat4 T = glm::translate(glm::mat4(1.0f), trans);
   safe_glUniformMatrix4fv(h_uM, glm::value_ptr(T));

   skydome->draw();
}
