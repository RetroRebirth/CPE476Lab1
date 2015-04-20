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
      GLint _h_aNor)  :
      
	      posBufID(0),
	      norBufID(0),
	      indBufID(0) {
	      
   // Default attribute values
   collected = false;
   col = glm::vec3(0.313, 0.784, 0.470);
   shine = 800.0;
   radius = OBJ_SIZE;

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
   pos = glm::vec3(2*Util::randF()*SIZE - SIZE, 1.0, 2*Util::randF()*SIZE - SIZE);
   dir = glm::normalize(glm::vec3(Util::randF()-0.5, 0.0, Util::randF()-0.5));
   vel = OBJ_SPEED;fferData(GL_ARRAY_BUFFER, norBuf.size()*sizeof(float), &norBuf[0], GL_STATIC_DRAW);
  
}

Object::~Object() {}

glm::vec3 Object::calculateNewPos(float dt) {
   return pos + dir * vel * dt;
}

bool Object::collidedWithPlayer(glm::vec3 camPos, float dt) {
   glm::vec3 testPos = calculateNewPos(dt);
   
   if (glm::distance(testPos, camPos) <= radius) {
      vel = 0;
      collected = true;
      col = glm::vec3(1.0, 0.68, 0.0);
      
      return true;
   }
   else {
      return false;
   }
}

bool Object::collidedWithWall(float dt) {
   bool collidedWithWall = false;
   glm::vec3 testPos = calculateNewPos(dt);
   
   // Check boundaries
   if (testPos.x < -SIZE
      || testPos.x > SIZE) {
      dir.x = -dir.x;
      collidedWithWall = true;
   }
   if (testPos.z < -SIZE 
      || testPos.z > SIZE) {
      dir.z = -dir.z;
      collidedWithWall = true;
   }

   return collidedWithWall;
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
   if (collected) {
      radius -= dt * OBJ_SHRINK_RATE;
   }

   pos = calculateNewPos(dt);
   draw(h_aPos, h_aNor);
}

/**
 * @return Rotation angle to face in degrees (0-360) along Y-axis.
 */
float Object::calcYFacingAngle() {
   float radians = atan(dir.z/dir.x);

   if (dir.x < 0) {
      radians = 180 - radians;
   }

   if (dir.x > 0 && dir.z < 0) {
      radians = 360 - radians;
   }

   return radians * (180.0 / M_PI); // Convert to degrees
}

void Object::load(const string &meshName)
{
   printf("loading\n");
	// Load geometry
	// Some obj files contain material information.
	// We'll ignore them for this assignment.
	std::vector<tinyobj::material_t> objMaterials;
	string err = tinyobj::LoadObj(shapes, objMaterials, meshName.c_str());
	if(!err.empty()) {
		cerr << err << endl;
	}
	
	// Scale the vertex positions so that they fit within [-1, +1] in all three dimensions.
	vector<float> &posBuf = shapes[0].mesh.positions;
	
	glm::vec3 vmin(posBuf[0], posBuf[1], posBuf[2]);
	glm::vec3 vmax(posBuf[0], posBuf[1], posBuf[2]);
	for(int i = 3; i < (int)posBuf.size(); i += 3) {
		glm::vec3 v(posBuf[i], posBuf[i+1], posBuf[i+2]);
		vmin.x = min(vmin.x, v.x);
		vmin.y = min(vmin.y, v.y);
		vmin.z = min(vmin.z, v.z);
		vmax.x = max(vmax.x, v.x);
		vmax.y = max(vmax.y, v.y);
		vmax.z = max(vmax.z, v.z);
	}
	glm::vec3 center = 0.5f*(vmin + vmax);
	glm::vec3 diff = vmax - vmin;
	float diffmax = diff.x;
	diffmax = max(diffmax, diff.y);
	diffmax = max(diffmax, diff.z);
	float scale = 1.0f / diffmax;
	
	//transBuf.resize((int)(posBuf.size()/3));
	//int j = 0;
	for(int i = 0; i < (int)posBuf.size(); i += 3) {
		posBuf[i  ] = (posBuf[i  ] - center.x) * scale;
		posBuf[i+1] = (posBuf[i+1] - center.y) * scale;
		posBuf[i+2] = (posBuf[i+2] - center.z) * scale;
   }/*
		// populate transBuf
		transBuf[j] = glm::vec3(posBuf[i], posBuf[i+1], posBuf[i+2]);
	}
	
	// edit: init the transformation and collision stuff
	transMat = glm::mat4();
	
	update();*/
}

/* initialize a new shape */
void Object::init()
{
   printf("initing\n");
	// Send the position array to the GPU
	const vector<float> &posBuf = shapes[0].mesh.positions;
	glGenBuffers(1, &posBufID);
	glBindBuffer(GL_ARRAY_BUFFER, posBufID);
	glBufferData(GL_ARRAY_BUFFER, posBuf.size()*sizeof(float), &posBuf[0], GL_STATIC_DRAW);
	
	// Send the normal array (if it exists) to the GPU
	const vector<float> &norBuf = shapes[0].mesh.normals;
	if(!norBuf.empty()) {
		glGenBuffers(1, &norBufID);
		glBindBuffer(GL_ARRAY_BUFFER, norBufID);
		glBufferData(GL_ARRAY_BUFFER, norBuf.size()*sizeof(float), &norBuf[0], GL_STATIC_DRAW);
	} else {
		norBufID = 0;
	}
	
	// Send the index array to the GPU
	const vector<unsigned int> &indBuf = shapes[0].mesh.indices;
	glGenBuffers(1, &indBufID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBufID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indBuf.size()*sizeof(unsigned int), &indBuf[0], GL_STATIC_DRAW);
	
	// Unbind the arrays
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	assert(glGetError() == GL_NO_ERROR);
}

void Object::draw(GLint h_pos, GLint h_nor)
{
	// Enable and bind position array for drawing
	GLSL::enableVertexAttribArray(h_pos);
	glBindBuffer(GL_ARRAY_BUFFER, posBufID);
	glVertexAttribPointer(h_pos, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	// Enable and bind normal array (if it exists) for drawing
	if(norBufID) {
		GLSL::enableVertexAttribArray(h_nor);
		glBindBuffer(GL_ARRAY_BUFFER, norBufID);
		glVertexAttribPointer(h_nor, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}
	
	// Bind index array for drawing
	int nIndices = (int)shapes[0].mesh.indices.size();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBufID);
	
	// Send data to the GPU and draw
   // Set the color
   glUniform3f(h_uAClr, col.x/5.0, col.y/5.0, col.z/5.0);
   glUniform3f(h_uDClr, col.x/3.0, col.y/3.0, col.z/3.0);
   glUniform3f(h_uSClr, col.x/3.0, col.y/3.0, col.z/3.0);
   glUniform1f(h_uS, shine);
   // Set the model transformation
   glm::mat4 S = glm::scale(glm::mat4(1.0f), glm::vec3(radius, radius, radius));
   glm::mat4 T = glm::translate(glm::mat4(1.0f), pos);
   glm::mat4 RX = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(1.0, 0.0, 0.0));
   glm::mat4 RY = glm::rotate(glm::mat4(1.0f), calcYFacingAngle(), glm::vec3(0.0, 1.0, 0.0));
   glm::mat4 RZ = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(0.0, 0.0, 1.0));

   glm::mat4 MV = T*RX*RY*RZ*S;

   safe_glUniformMatrix4fv(h_uM, glm::value_ptr(MV));
	
	
	// Draw
	glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);
}
/*
void Object::draw() {
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
   glUniform3f(h_uAClr, col.x/5.0, col.y/5.0, col.z/5.0);
   glUniform3f(h_uDClr, col.x/3.0, col.y/3.0, col.z/3.0);
   glUniform3f(h_uSClr, col.x/3.0, col.y/3.0, col.z/3.0);
   glUniform1f(h_uS, shine);
   // Set the model transformation
   glm::mat4 S = glm::scale(glm::mat4(1.0f), glm::vec3(radius, radius, radius));
   glm::mat4 T = glm::translate(glm::mat4(1.0f), pos);
   glm::mat4 RX = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(1.0, 0.0, 0.0));
   glm::mat4 RY = glm::rotate(glm::mat4(1.0f), calcYFacingAngle(), glm::vec3(0.0, 1.0, 0.0));
   glm::mat4 RZ = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(0.0, 0.0, 1.0));

   glm::mat4 MV = T*RX*RY*RZ*S;

   safe_glUniformMatrix4fv(h_uM, glm::value_ptr(MV));

   int nIndices = (int)shapes[0].mesh.indices.size();
   glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);
}*/

inline void Object::safe_glUniformMatrix4fv(const GLint handle, const GLfloat data[]) {
   //if (handle >= 0)
     // glUniformMatrix4fv(handle, 1, GL_FALSE, data);
  Util::safe_glUniformMatrix4fv(handle, data);
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

void Object::resize_obj() {
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

