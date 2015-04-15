#include "Object.h"

Object::Object(
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
   collided = false;
   col = glm::vec3(0.313, 0.784, 0.470);
   shine = 800.0;
   radius = 1.0;

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

   // Place the object randomly in the world
   pos = glm::vec3(2*randF()*SIZE - SIZE, 1.0, 2*randF()*SIZE - SIZE);
   dir = glm::normalize(glm::vec3(randF()-0.5, 0.0, randF()-0.5));
   vel = INITIAL_OBJ_SPEED;
   
   // Position array of object
   const vector<float> &posBuf = shapes[0].mesh.positions;
   glGenBuffers(1, &bufIDs.pos);
   glBindBuffer(GL_ARRAY_BUFFER, bufIDs.pos);
   glBufferData(GL_ARRAY_BUFFER, posBuf.size()*sizeof(float), &posBuf[0], GL_STATIC_DRAW);

   // Index array of object
   const vector<unsigned int> &indBuf = shapes[0].mesh.indices;
   glGenBuffers(1, &bufIDs.ind);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIDs.ind);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, indBuf.size()*sizeof(unsigned int), &indBuf[0], GL_STATIC_DRAW);

   // Normal array of ground
   vector<float> norBuf = computeNormals(posBuf, indBuf);
   glGenBuffers(1, &bufIDs.nor);
   glBindBuffer(GL_ARRAY_BUFFER, bufIDs.nor);
   glBufferData(GL_ARRAY_BUFFER, norBuf.size()*sizeof(float), &norBuf[0], GL_STATIC_DRAW);
}

Object::~Object() {}

glm::vec3 Object::calculateNewPos(float dt) {
   return pos + dir * vel * dt;
}

bool Object::collidedWithPlayer(glm::vec3 camPos, float dt, int *objCollected) {
   glm::vec3 testPos = calculateNewPos(dt);
   
   if (glm::distance(testPos, camPos) <= radius) {
      vel = 0;
      collided = true;
      col = glm::vec3(1.0, 0.68, 0.0);
      (*objCollected)++;
      
      return true;
   }
   else {
      return false;
   }
}

bool Object::collidedWithWall(float dt) {
   bool collided = false;
   glm::vec3 testPos = calculateNewPos(dt);
   
   // Check boundaries
   if (testPos.x < -SIZE
      || testPos.x > SIZE) {
      dir.x = -dir.x;
      collided = true;
   }
   if (testPos.z < -SIZE 
      || testPos.z > SIZE) {
      dir.z = -dir.z;
      collided = true;
   }

   return collided;
}

bool Object::collidedWithObj(Object o, float dt) {
   glm::vec3 testPos = calculateNewPos(dt);
   glm::vec3 testPosO = o.calculateNewPos(dt);

   if (glm::distance(testPos, testPosO) <= (radius + o.radius)) {
      dir = -dir;
      
      return true;
   }
   else {
      return false;
   }
}

void Object::step(float dt) {
   pos = calculateNewPos(dt);

   draw();
}

void Object::draw() {
   /*GLuint posBufID = bufIDs.pos;
   GLuint indBufID = bufIDs.ind;
   GLuint norBufID = bufIDs.nor;

   // Bind position buffer
   glBindBuffer(GL_ARRAY_BUFFER, posBufID);
   glVertexAttribPointer(h_aPos, 3, GL_FLOAT, GL_FALSE, 0, 0);

   // Bind index buffer
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBufID);

   // Bind normal buffer
	glBindBuffer(GL_ARRAY_BUFFER, norBufID);
   glVertexAttribPointer(h_aNor, 3, GL_FLOAT, GL_FALSE, 0, 0);
   
   // Send data to the GPU and draw
   // Set the color
   */glUniform3f(h_uAClr, col.x/5.0, col.y/5.0, col.z/5.0);
   glUniform3f(h_uDClr, col.x/3.0, col.y/3.0, col.z/3.0);
   glUniform3f(h_uSClr, col.x/3.0, col.y/3.0, col.z/3.0);
   glUniform1f(h_uS, shine);
   // Set the model transformation
   glm::mat4 S = glm::scale(glm::mat4(1.0f), glm::vec3(radius, radius, radius));
   glm::mat4 T = glm::translate(glm::mat4(1.0f), pos);
   glm::mat4 RX = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(1.0, 0.0, 0.0));
   glm::mat4 RY = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(0.0, 1.0, 0.0));
   glm::mat4 RZ = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(0.0, 0.0, 1.0));

   glm::mat4 MV = T*RX*RY*RZ*S;

   safe_glUniformMatrix4fv(h_uM, glm::value_ptr(MV));


   int nIndices = (int)shapes[0].mesh.indices.size();
   //glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);
}

float Object::randF() {
   return (float)rand() / RAND_MAX;
}

inline void Object::safe_glUniformMatrix4fv(const GLint handle, const GLfloat data[]) {
   if (handle >= 0)
      glUniformMatrix4fv(handle, 1, GL_FALSE, data);
}

vector<float> Object::computeNormals(vector<float> posBuf, vector<unsigned int> indBuf) {
   vector<float> norBuf;
   vector<glm::vec3> crossBuf;
   int idx1, idx2, idx3;
   glm::vec3 v1, v2, v3;
   glm::vec3 vec1, vec2, vec3;
   glm::vec3 cross1, cross2, cross3;

   // For every vertex initialize a normal to 0
   for (int j = 0; j < posBuf.size()/3; j++) {
      norBuf.push_back(0);
      norBuf.push_back(0);
      norBuf.push_back(0);

      crossBuf.push_back(glm::vec3(0, 0, 0));
   }

   // Compute normals for every face then add them to every associated vertex
   for (int i = 0; i < indBuf.size()/3; i++) {
      idx1 = indBuf[3*i+0];
      idx2 = indBuf[3*i+1];
      idx3 = indBuf[3*i+2];
      v1 = glm::vec3(posBuf[3*idx1 +0], posBuf[3*idx1 +1], posBuf[3*idx1 +2]); 
      v2 = glm::vec3(posBuf[3*idx2 +0], posBuf[3*idx2 +1], posBuf[3*idx2 +2]); 
      v3 = glm::vec3(posBuf[3*idx3 +0], posBuf[3*idx3 +1], posBuf[3*idx3 +2]); 

      vec1 = glm::normalize(v1 - v2);
      vec2 = glm::normalize(v2 - v3);
      vec3 = glm::normalize(v3 - v1);

      cross1 = glm::cross(vec1, vec2);
      cross2 = glm::cross(vec2, vec3);
      cross3 = glm::cross(vec3, vec1);

      crossBuf[idx1] += cross1;
      crossBuf[idx2] += cross2;
      crossBuf[idx3] += cross3;
   }

   // Cross products have been added together, normalize them and add to normal buffer
   for (int i = 0; i < indBuf.size()/3; i++) {
      idx1 = indBuf[3*i+0];
      idx2 = indBuf[3*i+1];
      idx3 = indBuf[3*i+2];

      cross1 = glm::normalize(crossBuf[idx1]);
      cross2 = glm::normalize(crossBuf[idx2]);
      cross3 = glm::normalize(crossBuf[idx3]);

      norBuf[3*idx1+0] = cross1.x;
      norBuf[3*idx1+1] = cross1.y;
      norBuf[3*idx1+2] = cross1.z;
      norBuf[3*idx2+0] = cross2.x;
      norBuf[3*idx2+1] = cross2.y;
      norBuf[3*idx2+2] = cross2.z;
      norBuf[3*idx3+0] = cross3.x;
      norBuf[3*idx3+1] = cross3.y;
      norBuf[3*idx3+2] = cross3.z;
   }

   return norBuf;
}
