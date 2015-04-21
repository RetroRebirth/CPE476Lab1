#include "SkyBox.h"
using namespace std;

SkyBox::SkyBox(GLint _h_aPos, GLint _h_aNor, GLint _h_uM, GLint _h_uTexUnit, GLint _h_aTexCoord)
{
   // Defined attribute values
   h_aPos = _h_aPos;
   h_aNor = _h_aNor;
   h_uM = _h_uM;
   h_uTexUnit = _h_uTexUnit;
   h_aTexCoord = _h_aTexCoord;

   init();
   loadTexture((char *)"cloud5.bmp", TEXTURE_SKY);
}

SkyBox::~SkyBox()
{
   // Delete stuff
}

void SkyBox::init()
{
   // Position array of sky box
   GLfloat CubePos[] = {
      -1.5, -1.5, -1.5, //back face 5 verts :0 - clockwise 
      -1.5,  1.5, -1.5,
       1.5,  1.5, -1.5,
       1.5, -1.5, -1.5,
       1.5, -1.5,  1.5, //right face 5 verts :4
       1.5,  1.5,  1.5,
       1.5,  1.5, -1.5,
       1.5, -1.5, -1.5,
      -1.5, -1.5,  1.5, //front face 4 verts :8
      -1.5,  1.5,  1.5,
       1.5,  1.5,  1.5,
       1.5, -1.5,  1.5,
      -1.5, -1.5, -1.5, //left face 4 verts :12
      -1.5,  1.5, -1.5,
      -1.5,  1.5,  1.5,
      -1.5, -1.5,  1.5
   };
   // Amplify sky box size
   for (int i = 0, last = sizeof(CubePos)/sizeof(GLfloat); i < last; i++) {
      CubePos[i] *= SKY_SIZE;
   }

   // Index array of sky box   
   unsigned short idx[] = {0, 1, 2,  2, 3, 0,  4, 5, 6, 6, 7, 4,  8, 9, 10, 10, 11, 8,  12, 13, 14, 14, 15, 12};

   // Texture index mapping of sky box
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

   glGenBuffers(1, &boxBufIDs.pos);
   glBindBuffer(GL_ARRAY_BUFFER, boxBufIDs.pos);
   glBufferData(GL_ARRAY_BUFFER, sizeof(CubePos), CubePos, GL_STATIC_DRAW);

   glGenBuffers(1, &boxBufIDs.ind);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, boxBufIDs.ind);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idx), idx, GL_STATIC_DRAW);

   glGenBuffers(1, &boxBufIDs.nor);
   glBindBuffer(GL_ARRAY_BUFFER, boxBufIDs.nor);
   glBufferData(GL_ARRAY_BUFFER, sizeof(CubeTex), CubeTex, GL_STATIC_DRAW);
}

void SkyBox::draw(Camera* camera, Window* window)
{
   camera->step(window);

   glEnable(GL_TEXTURE_2D);
   glActiveTexture(GL_TEXTURE0);
   glUniform1i(h_uTexUnit, 0);

   // Bind the sky texture
   glBindTexture(GL_TEXTURE_2D, TEXTURE_SKY);

   // Bind position buffer
   glBindBuffer(GL_ARRAY_BUFFER, boxBufIDs.pos);
   glVertexAttribPointer(h_aPos, 3, GL_FLOAT, GL_FALSE, 0, 0);
   // Bind index buffer
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, boxBufIDs.ind);
   // Bind texture buffer
   GLSL::enableVertexAttribArray(h_aTexCoord);
   glBindBuffer(GL_ARRAY_BUFFER, boxBufIDs.nor);
   glVertexAttribPointer(h_aTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0); 

   // Apply translation
   glm::vec3 trans = camera->pos;
   glm::mat4 T = glm::translate(glm::mat4(1.0f), trans);
   safe_glUniformMatrix4fv(h_uM, glm::value_ptr(T));

   glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_SHORT, 0);
   glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
}
