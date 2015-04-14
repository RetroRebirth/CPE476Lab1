#include "World.h"

World::World(
      vector<tinyobj::shape_t> &_shapes,
      vector<tinyobj::material_t> &_materials,
      GLint _h_uAClr,
      GLint _h_uDClr,
      GLint _h_uSClr,
      GLint _h_uS,
      GLint _h_uM,
      GLint _h_aPos,
      GLint _h_aNor) {
   // Default attribute values

   // Defined attribute values
   shapes = _shapes;
   materials = _materials;
   h_uAClr = _h_uAClr;
   h_uDClr = _h_uDClr;
   h_uSClr = _h_uSClr;
   h_uS = _h_uS;
   h_uM = _h_uM;
   h_aPos = _h_aPos;
   h_aNor = _h_aNor;

   initGround();
}

World::~World() {}

void World::step(float dt) {
   drawGround();
}

void World::draw() {
}

float World::randF() {
   return (float)rand() / RAND_MAX;
}

inline void World::safe_glUniformMatrix4fv(const GLint handle, const GLfloat data[]) {
   if (handle >= 0)
      glUniformMatrix4fv(handle, 1, GL_FALSE, data);
}

void World::initGround() {
   // Position array of ground
   GLfloat vertices[] = {
      -1.0f, 0.0, -1.0f, //0
      -1.0f, 0.0, +1.0f, //1
      +1.0f, 0.0, -1.0f, //2
      +1.0f, 0.0, +1.0f, //3
   };
   // Amplify ground size
   for (int i = 0, last = sizeof(vertices)/sizeof(GLfloat); i < last; i++) {
      // Don't amplify the floor height
      if (i % 3 != 1)
         vertices[i] *= SIZE;
   }
   glGenBuffers(1, &(groundBufIDs.pos));
   glBindBuffer(GL_ARRAY_BUFFER, groundBufIDs.pos);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

   // Index array of ground
   GLuint indices[] = {
      0, 1, 2,
      1, 3, 2,
   };
   glGenBuffers(1, &(groundBufIDs.ind));
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, groundBufIDs.ind);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

   // Normal array of ground
   vector<float> posBuf(vertices, vertices+sizeof(vertices)/sizeof(vertices[0]));
   vector<unsigned int> indBuf(indices, indices+sizeof(indices)/sizeof(indices[0]));

   glGenBuffers(1, &(groundBufIDs.nor));
   glBindBuffer(GL_ARRAY_BUFFER, groundBufIDs.nor);
}

void World::drawGround() {
   // Bind position buffer
   glBindBuffer(GL_ARRAY_BUFFER, groundBufIDs.pos);
   glVertexAttribPointer(h_aPos, 3, GL_FLOAT, GL_FALSE, 0, 0);
   // Bind index buffer
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, groundBufIDs.ind);
   // Bind normal buffer
   glVertexAttribPointer(h_aNor, 3, GL_FLOAT, GL_FALSE, 0, 0);
   // Set color
   glUniform3f(h_uAClr, 0.13, 0.13, 0.14);
   glUniform3f(h_uDClr, 0.0, 0.0, 0.0);
   glUniform3f(h_uSClr, 0.0, 0.0, 0.0);
   glUniform1f(h_uS, 1.0);
   // Apply translation
   glm::vec3 trans = glm::vec3(0.0, 0.0, 0.0);
   glm::mat4 T = glm::translate(glm::mat4(1.0f), trans);
   safe_glUniformMatrix4fv(h_uM, glm::value_ptr(T));

   glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

