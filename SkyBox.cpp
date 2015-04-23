#include "SkyBox.h"
using namespace std;

SkyBox::SkyBox(GLuint _ShadeProg)
{
   ShadeProg = _ShadeProg;

   // Defined attribute values
   h_aPos = GLSL::getAttribLocation(ShadeProg, "aPos");
   h_aNor = GLSL::getAttribLocation(ShadeProg, "aNor");
   h_uM = GLSL::getUniformLocation(ShadeProg, "uM");
   h_uTexUnit = GLSL::getUniformLocation(ShadeProg, "uTexUnit");

   loadTexture((char *)"cloud.bmp", TEXTURE_SKY);
}

SkyBox::~SkyBox() {}

void SkyBox::draw(Camera* camera, Window* window)
{
   // Set to sphere mapping
   GLUquadricObj *sphere = NULL;
   sphere = gluNewQuadric();
   gluQuadricDrawStyle(sphere, GLU_FILL);
   gluQuadricTexture(sphere, 1);
   gluQuadricNormals(sphere, GLU_SMOOTH);

   //Making a display list
   GLuint mysphereID = glGenLists(1);
   glNewList(mysphereID, GL_COMPILE);
   gluSphere(sphere, SKY_SIZE, 20, 20);
   glEndList();
   gluDeleteQuadric(sphere);

   // Enable textures
   glEnable(GL_TEXTURE_2D);
   glActiveTexture(GL_TEXTURE0);
   glUniform1i(h_uTexUnit, 0);

   // Bind the sky texture
   glBindTexture(GL_TEXTURE_2D, TEXTURE_SKY);

   // Apply translation
   glm::vec3 trans = camera->pos;
   glm::mat4 T = glm::translate(glm::mat4(1.0f), trans);
   safe_glUniformMatrix4fv(h_uM, glm::value_ptr(T));

   // Draw the skydome
   glCallList(mysphereID);
   glDeleteLists(mysphereID, 1);
}
