#include "Object.h"

// Model vectors
vector<tinyobj::shape_t> shapes;
vector<tinyobj::material_t> materials;
transform_t transform;
// Obj buffer ids
bufID_t bufIDs;

Object::Object() {}

Object::~Object() {}

float Object::randF() {
   return (float)rand() / RAND_MAX;
}

inline void Object::safe_glUniformMatrix4fv(const GLint handle, const GLfloat data[]) {
   if (handle >= 0)
      glUniformMatrix4fv(handle, 1, GL_FALSE, data);
}

/** INITIALIZING FOR DRAW **/
void Object::resize_obj(std::vector<tinyobj::shape_t> &shapes) {
   float minX, minY, minZ;
   float maxX, maxY, maxZ;
   float scaleX, scaleY, scaleZ;
   float shiftX, shiftY, shiftZ;
   float epsilon = 0.001;

   minX = minY = minZ = 1.1754E+38F;
   maxX = maxY = maxZ = -1.1754E+38F;

   //Go through all vertices to determine min and max of each dimension
   for (size_t i = 0; i < shapes.size(); i++) {
      for (size_t v = 0; v < shapes[i].mesh.positions.size() / 3; v++) {
         if(shapes[i].mesh.positions[3*v+0] < minX)
            minX = shapes[i].mesh.positions[3*v+0];
         if(shapes[i].mesh.positions[3*v+0] > maxX)
            maxX = shapes[i].mesh.positions[3*v+0];

         if(shapes[i].mesh.positions[3*v+1] < minY)
            minY = shapes[i].mesh.positions[3*v+1];
         if(shapes[i].mesh.positions[3*v+1] > maxY)
            maxY = shapes[i].mesh.positions[3*v+1];

         if(shapes[i].mesh.positions[3*v+2] < minZ)
            minZ = shapes[i].mesh.positions[3*v+2];
         if(shapes[i].mesh.positions[3*v+2] > maxZ)
            maxZ = shapes[i].mesh.positions[3*v+2];
      }
   }
   //From min and max compute necessary scale and shift for each dimension
   float maxExtent, xExtent, yExtent, zExtent;
   xExtent = maxX-minX;
   yExtent = maxY-minY;
   zExtent = maxZ-minZ;
   if (xExtent >= yExtent && xExtent >= zExtent) {
      maxExtent = xExtent;
   }
   if (yExtent >= xExtent && yExtent >= zExtent) {
      maxExtent = yExtent;
   }
   if (zExtent >= xExtent && zExtent >= yExtent) {
      maxExtent = zExtent;
   }
   scaleX = 2.0 /maxExtent;
   shiftX = minX + (xExtent/ 2.0);
   scaleY = 2.0 / maxExtent;
   shiftY = minY + (yExtent / 2.0);
   scaleZ = 2.0/ maxExtent;
   shiftZ = minZ + (zExtent)/2.0;

   // Go through all verticies shift and scale them
   for (size_t i = 0; i < shapes.size(); i++) {
      for (size_t v = 0; v < shapes[i].mesh.positions.size() / 3; v++) {
         shapes[i].mesh.positions[3*v+0] = (shapes[i].mesh.positions[3*v+0] - shiftX) * scaleX;
         assert(shapes[i].mesh.positions[3*v+0] >= -1.0 - epsilon);
         assert(shapes[i].mesh.positions[3*v+0] <= 1.0 + epsilon);
         shapes[i].mesh.positions[3*v+1] = (shapes[i].mesh.positions[3*v+1] - shiftY) * scaleY;
         assert(shapes[i].mesh.positions[3*v+1] >= -1.0 - epsilon);
         assert(shapes[i].mesh.positions[3*v+1] <= 1.0 + epsilon);
         shapes[i].mesh.positions[3*v+2] = (shapes[i].mesh.positions[3*v+2] - shiftZ) * scaleZ;
         assert(shapes[i].mesh.positions[3*v+2] >= -1.0 - epsilon);
         assert(shapes[i].mesh.positions[3*v+2] <= 1.0 + epsilon);
      }
   }
}

void Object::loadShapes(const string &objFile) {
   string err = tinyobj::LoadObj(shapes, materials, objFile.c_str());
   if(!err.empty()) {
      cerr << err << endl;
   }
   resize_obj(shapes);
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

void Object::randomTransform(transform_t &transform) {
   float s = 1.0;
   float x = 2*randF()*SIZE - SIZE;
   float y = 0.0;
   float z = 2*randF()*SIZE - SIZE;
   float rotX = 0;
   float rotY = 0;
   float rotZ = 0;

   transform.trans = glm::vec3(x, y, z);
   transform.scale = glm::vec3(s, s, s);
   transform.rotX = rotX;
   transform.rotY = rotY;
   transform.rotZ = rotZ;
}

void Object::initModel(int objType, transform_t &transform) {
   randomTransform(transform);
}

void Object::initObj(int objType) {
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

   // Initialize transforms for each object
   initModel(objType, transform);
}

/** DRAWING **/
void Object::setMaterial(int i, GLint h_uAClr, GLint h_uDClr, GLint h_uSClr, GLint h_uS) {
   glm::vec3 col;

   switch (i) {
      case -1: // flat grey (ground)
         glUniform3f(h_uAClr, 0.13, 0.13, 0.14);
         glUniform3f(h_uDClr, 0.0, 0.0, 0.0);
         glUniform3f(h_uSClr, 0.0, 0.0, 0.0);
         glUniform1f(h_uS, 1.0);
         break;

      case 0: // emerald
         col = glm::vec3(0.313, 0.784, 0.470);
         glUniform3f(h_uAClr, col.x/5.0, col.y/5.0, col.z/5.0);
         glUniform3f(h_uDClr, col.x/3.0, col.y/3.0, col.z/3.0);
         glUniform3f(h_uSClr, col.x/3.0, col.y/3.0, col.z/3.0);
         glUniform1f(h_uS, 800.0);
         break;
      case 1: // gold
         glUniform3f(h_uAClr, 0.09, 0.07, 0.08);
         glUniform3f(h_uDClr, 0.91, 0.782, 0.82);
         glUniform3f(h_uSClr, 1.0, 0.913, 0.8);
         glUniform1f(h_uS, 200.0);
         break;
      case 2: // shiny blue plastic
         glUniform3f(h_uAClr, 0.02, 0.02, 0.1);
         glUniform3f(h_uDClr, 0.0, 0.08, 0.5);
         glUniform3f(h_uSClr, 0.14, 0.14, 0.4);
         glUniform1f(h_uS, 120.0);
         break;
      case 3: //ruby jewel
         col = glm::vec3(0.607, 0.066, 0.117);
         glUniform3f(h_uAClr, col.x/10.0, col.y/10.0, col.z/10.0);
         glUniform3f(h_uDClr, col.x/3.0, col.y/3.0, col.z/3.0);
         glUniform3f(h_uSClr, col.x/1.0, col.y/1.0, col.z/1.0);
         glUniform1f(h_uS, 300.0);
         break;
      case 4: // flat grey
         glUniform3f(h_uAClr, 0.13, 0.13, 0.14);
         glUniform3f(h_uDClr, 0.3, 0.3, 0.4);
         glUniform3f(h_uSClr, 0.3, 0.3, 0.4);
         glUniform1f(h_uS, 4.0);

      default: // black
         glUniform3f(h_uAClr, 0.0, 0.0, 0.0);
         glUniform3f(h_uDClr, 0.0, 0.0, 0.0);
         glUniform3f(h_uSClr, 0.0, 0.0, 0.0);
         glUniform1f(h_uS, 1.0);
         break;
   }
}

void Object::setModel(GLint h_uM) {
   // Calculate each matrix
   glm::mat4 S = glm::scale(glm::mat4(1.0f), transform.scale);
   glm::mat4 T = glm::translate(glm::mat4(1.0f), transform.trans);
   glm::mat4 RX = glm::rotate(glm::mat4(1.0f), transform.rotX, glm::vec3(1.0, 0.0, 0.0));
   glm::mat4 RY = glm::rotate(glm::mat4(1.0f), transform.rotY, glm::vec3(0.0, 1.0, 0.0));
   glm::mat4 RZ = glm::rotate(glm::mat4(1.0f), transform.rotZ, glm::vec3(0.0, 0.0, 1.0));

   glm::mat4 MV = T*RX*RY*RZ*S;

   safe_glUniformMatrix4fv(h_uM, glm::value_ptr(MV));
}

void Object::drawObj(GLint h_uAClr, GLint h_uDClr, GLint h_uSClr, GLint h_uS, GLint h_uM, GLint h_aPos, GLint h_aNor) {
   GLuint posBufID = bufIDs.pos;
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
   setMaterial(0, h_uAClr, h_uDClr, h_uSClr, h_uS);
   // Set the model transformation
   setModel(h_uM);

   int nIndices = (int)shapes[0].mesh.indices.size();
   glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);
}
