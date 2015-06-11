#include "Object.h"

Object::Object(
      vector<tinyobj::shape_t> &_shapes,
      vector<tinyobj::material_t> &_materials,
      GLuint _ShadeProg)  :
	      posBufID(0),
	      norBufID(0),
	      indBufID(0) {
	      
   // Default attribute values
   collected = false;
   shine = 800.0;
   radius = OBJ_SIZE/2.0f;
   
   // initialize transform matrices
   modelMat = glm::mat4(1.0f);
   scalerMat = glm::mat4(1.0f);
   rotateMat = glm::mat4(1.0f);
   shearMat = glm::mat4(1.0f);
   directionalMat = glm::mat4(1.0f);
   transMat = glm::mat4(1.0f);
   transRot = glm::mat4(1.0f);
   
   dir_angle = 0.0f;
   boundBoxScalerMat = glm::scale(glm::mat4(1.0f), glm::vec3(1.1f,1.1f,1.1f));

   // Defined attribute values
   shapes = _shapes;
   materials = _materials;
   ShadeProg = _ShadeProg;
   h_uM = GLSL::getUniformLocation(ShadeProg, "uM");
   h_aPos = GLSL::getAttribLocation(ShadeProg, "aPos");
   h_aNor = GLSL::getAttribLocation(ShadeProg, "aNor");
              
   reflective = bumpy = directional = drawBounds = false;
   screenRender = screenRender = castShadows = changeDir = true;

   pos = glm::vec3(0.0f, 0.0f, 0.0f);
   dimensions = glm::vec3(1.0f, 1.0f, 1.0f);
   dir = glm::vec3(0.0f, 0.0f, 1.0f);
   vel = 0.0f;
   accel = 1.0f;
   planes.reserve(4*sizeof(plane));
   
   shadowHeight = 0;
   shadowDarkness = 1.0;
   setTexture(textures[MISC_TYPE]);
   xzRadius = -1.0f;
   
   drawBounds = false;
   isTree = false;
}

Object::~Object() {}

// scale object by flat amount, applied on top of scaling done by the radius
void Object::scale(glm::vec3 scaler) {
   scalerMat = glm::scale(glm::mat4(1.0f), scaler);
   radius = glm::max(glm::max(scaler.x, scaler.y), scaler.z);
}

void Object::setDir(glm::vec3 direction) {
   float dot1 = glm::dot(direction, glm::vec3(0.0f, 0.0f, 1.0f));//, direction);
   float dot2 = glm::dot(glm::vec3(0.0f, 0.0f, 1.0f), direction);
   //dir_angle = acos(fmin(dot1,dot2));
   float angle1 = acos(dot1);
   float angle2 = acos(dot2);
   dir_angle = fmin(angle1,angle2);
   directionalMat = glm::rotate(glm::mat4(1.0f), -1.0f*(float)(dir_angle*180.0f/M_PI), glm::vec3(0.0f, 1.0f, 0.0f));
   dir = direction;
}

// rotate object by flat amount, this will be applied on top of directional rotations
void Object::rotate(float angle, glm::vec3 axis) {
   rotateMat = glm::rotate(glm::mat4(1.0f), angle, axis);
}
void Object::iterativeRotate(float angle, glm::vec3 axis) {
   if (angle == 0.0) {
      rotateMat = glm::mat4(1.0f);
   } else {
      rotateMat *= glm::rotate(glm::mat4(1.0f), angle, axis);
   }
}

// translate object by flat amount... this will be applied on top of positional translations
void Object::translate(glm::vec3 trans) {
   transMat = glm::translate(glm::mat4(1.0f), trans);
   // TODO update pos
}
void Object::setTransRot(glm::vec3 trans) {
   transRot = glm::translate(glm::mat4(1.0f), trans);
}

bool Object::collidedWithPlayer(glm::vec3 camPos, float dt) {
   glm::vec3 testPos = calculateNewPos(dt);
   
   if (glm::distance(testPos, camPos) <= radius) {
      vel = 0;
      collected = true;
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

bool Object::collision(Object* o) {
   if (glm::distance(o->pos, pos) <= (radius + o->radius)) {
      return true;
   }
   return false;
}

float Object::updateRadius() {
        const vector<float> &posBuf = shapes[0].mesh.positions;
        
        float rad;
        for (int i = 0; i < (int)posBuf.size(); i += 3) {
            glm::vec4 v;
            v = glm::vec4(posBuf[i], posBuf[i+1], posBuf[i+2], 1.0f) * scalerMat;
            if (i == 0) {
                // initialize radius on first pass
                rad = abs(v.x);
            }
            else {
                if (abs(v.x) > rad) {
                    rad = abs(v.x);
                }
                if (abs(v.y) > rad) {
                    rad = abs(v.y);
                }
                if (abs(v.z) > rad) {
                    rad = abs(v.z);
                }
            }
        }
        radius = rad;
        return rad;
    //return xzRadius;
}

float Object::getXZRadius() {
   if (xzRadius == -1.0f) {
      const vector<float> &posBuf = shapes[0].mesh.positions;
      
      float xz_rad;
      for (int i = 0; i < (int)posBuf.size(); i += 3) {
         glm::vec4 v;
         v = glm::vec4(posBuf[i], posBuf[i+1], posBuf[i+2], 1.0f) * scalerMat;
         if (i == 0) {
            // initialize radius on first pass
            xz_rad = abs(v.x);
         }
         else {
            if (abs(v.x) > xz_rad) {
               xz_rad = abs(v.x);
            }
            if (abs(v.z) > xz_rad) {
               xz_rad = abs(v.z);
            }
         }
      }
      xzRadius = xz_rad;
   }
   return xzRadius;
}

//Checks if anything is colliding with the object so it stops them.
bool Object::checkCollision(Object* _otherObject){
   glm::vec3 max = glm::vec3(_otherObject->getPos().x + _otherObject->getRadius(), _otherObject->getPos().y + _otherObject->getRadius(), _otherObject->getPos().z + _otherObject->getRadius());
   glm::vec3 min = glm::vec3(_otherObject->getPos().x - _otherObject->getRadius(), _otherObject->getPos().y - _otherObject->getRadius(), _otherObject->getPos().z - _otherObject->getRadius());
   
   if ((max.x < bounds.x_min) || (min.x > bounds.x_max)) {
      return false;
   }
   if ((max.y < bounds.y_min) || (min.y > bounds.y_max)) {
      return false;
   }
   if ((max.z < bounds.z_min) || (min.z > bounds.z_max)) {
      return false;
   }
   
   return true;
}

void Object::getCollisionAxis(glm::vec3 pos, glm::vec3* colPlane) {
   // going to probably have to get the plane equation f
   float x_diff, y_diff, z_diff;
   x_diff = bounds.x_min - pos.x;
   if (abs(bounds.x_max - pos.x) < abs(x_diff)) {
      x_diff = bounds.x_max - pos.x;
   }
   y_diff = bounds.y_min - pos.y;
   if (abs(bounds.y_max - pos.y) < abs(y_diff)) {
      y_diff = bounds.y_max - pos.y;
   }
   z_diff = bounds.z_min - pos.z;
   if (abs(bounds.z_max - pos.z) < abs(z_diff)) {
      z_diff = bounds.z_max - pos.z;
   }
   
   float diffs[3] = {x_diff, y_diff, z_diff};
   float min = abs(diffs[0]);
   glm::vec3 plane = glm::vec3(1.0f, 0.0f, 0.0f);
   for (int i=0; i<3; ++i) {
      if (abs(diffs[i]) < min) {
         min = abs(diffs[i]);
         plane = glm::vec3(0.0f);
         plane[i] = 1.0f;
      }
   }
   glm::vec3 temp_vec = glm::vec3(pos.x + x_diff, pos.y + y_diff, pos.z + z_diff);
   plane *= temp_vec;
   colPlane->x = plane.x;
   colPlane->y = plane.y;
   colPlane->z = plane.z;
}

bool Object::planarCollisionCheck(Object* o, glm::vec3* colPlane) {
   if (!checkCollision(o)) {
      return false;
   }
   glm::vec3 position = o->pos;
   // go through each plane and plug in position to find distance
   for (int i=0; i<planes.size(); ++i) {
      // distance = (a(x) + b(y) + c(z) + d)/plane norm
      float d = planes[i].a*position.x + planes[i].b*position.y + planes[i].c*position.z + planes[i].d;
      float distance = d / (float)sqrt(planes[i].a*planes[i].a + planes[i].b*planes[i].b + planes[i].c*planes[i].c);
      if (fabs(distance) < o->getXZRadius()) {
         // which plane? planes = {z1, z2, x1, x2}
         if (i == 0) {
            // collided with minimum z plane
            colPlane->z = -1.0f;
         }
         else if (i == 1) {
            // collided with maximum z plane
            colPlane->z = 1.0f;
         }
         else if (i == 2) {
            // collided with minimum x plane
            colPlane->x = -1.0f;
         }
         else if (i == 3) {
            // collided with maximum x plane
            colPlane->x = 1.0f;
         }
         return true;
      }
   }
   return false;
}

// check for camera collision set colPlane equal to the bound that has been passed through
bool Object::checkCameraCollision(glm::vec3 cam_pos, glm::vec3 *colPlane) {
   if ((cam_pos.x < bounds.x_min) || (cam_pos.x > bounds.x_max)) {
      return false;
   }
   if ((cam_pos.y < bounds.y_min) || (cam_pos.y > bounds.y_max)) {
      return false;
   }
   if ((cam_pos.z < bounds.z_min) || (cam_pos.z > bounds.z_max)) {
      return false;
   }
   
   getCollisionAxis(cam_pos, colPlane);
   
   return true;
}

// check for player collision
bool Object::checkPlayerCollision(Object* player, glm::vec3* colPlane) {
   if (!checkCollision(player)) {
      return false;
   }
   
   getCollisionAxis(player->getPos(), colPlane);
   return true;
}

bool Object::existsInVoxel(int index) {
   for (int i = 0; i < spatialGrid[index].members.size(); ++i) {
      if (spatialGrid[index].members[i] == this) {
         return true;
      }
   }
   return false;
}

void Object::checkForPlayer() { 
   const vector<float> &posBuf = shapes[0].mesh.positions;
	for(int i = 0; i < (int)posBuf.size(); i += 3) {
      glm::vec4 v;
      v = glm::vec4(posBuf[i], posBuf[i+1], posBuf[i+2], 1.0f);
      v = modelMat * v;
      
      // check vertex against spatial data structure to add object where it belongs
      for (int j = 0; j < UNIFORM_GRID_SIZE; ++j) {
         spatialGrid[j].hasPlayer = false;
         // only do check if we no its not in there yet
         //if (!existsInVoxel(j)) {
            if (v.x > spatialGrid[j].x_min && v.x < spatialGrid[j].x_max && v.z > spatialGrid[j].z_min && v.z < spatialGrid[j].z_max) {
               spatialGrid[j].hasPlayer = true;
            }
        // }
      }     
   }
}

bool Object::voxelOverlap(int index) {
   if ((spatialGrid[index].x_max < bounds.x_min) || (spatialGrid[index].x_min > bounds.x_max)) {
      return false;
   }
   if ((spatialGrid[index].z_max < bounds.z_min) || (spatialGrid[index].z_min > bounds.z_max)) {
      return false;
   }
   return true;
}

void Object::getBounds(struct bound_box *_bounds) {
   
   float x_min, x_max, y_min, y_max, z_min, z_max; 
   
   if (!isTree) {
      const vector<float> &posBuf = shapes[0].mesh.positions;
	   for(int i = 0; i < (int)posBuf.size(); i += 3) {
         glm::vec4 v;
         v = glm::vec4(posBuf[i], posBuf[i+1], posBuf[i+2], 1.0f);
         v = modelMat * v;
         
         
         
         // bound box stuff 
         if (i == 0) { 
            // initialize _bounds on first pass
            x_min = x_max = v.x;
            y_min = y_max = v.y;
            z_min = z_max = v.z;
         }
         else {
            // establish spatial relationships
            if (v.x < x_min) {
               x_min = v.x;
            }
            else if (v.x > x_max) {
               x_max = v.x;
            }
            if (v.y < y_min) {
               y_min = v.y;
            }
            else if (v.y > y_max) {
               y_max = v.y;
            }
            if (v.z < z_min) {
               z_min = v.z;
            }
            else if (v.z > z_max) {
               z_max = v.z;
            }
         }
      }
   }
   else {
      x_min = pos.x - TREE_WIDTH;
      x_max = pos.x + TREE_WIDTH;
      y_min = pos.y - TREE_WIDTH;
      y_max = pos.y + TREE_WIDTH;
      z_min = pos.z - TREE_WIDTH;
      z_max = pos.z + TREE_WIDTH;
   }
      
   _bounds->x_min = x_min;
   _bounds->x_max = x_max;
   _bounds->y_min = y_min;
   _bounds->y_max = y_max;
   _bounds->z_min = z_min;
   _bounds->z_max = z_max;
      
   
   // check vertex against spatial data structure to add object where it belongs
   for (int j = 0; j < UNIFORM_GRID_SIZE; ++j) {
      // only do check if we no its not in there yet
      if (!existsInVoxel(j)) {
         
         if (voxelOverlap(j)) {
            spatialGrid[j].members.push_back(this);
         }
         //if (v.x > spatialGrid[j].x_min && v.x < spatialGrid[j].x_max && v.z > spatialGrid[j].z_min && v.z < spatialGrid[j].z_max) {
            //spatialGrid[j].members.push_back(this);
         //}
      }
   }
   
   // ok set up the planes just x and z unless we need this for y too...
   struct plane x1, x2, z1, z2;
   planes.clear();
   // z1 
   glm::vec3 z1p = glm::vec3(x_min, y_min, z_min);
   glm::vec3 z1q = glm::vec3(x_max, y_min, z_min);
   glm::vec3 z1r = glm::vec3(x_max, y_max, z_min);
   setupPlane(z1p, z1q, z1r, &z1);
   planes.push_back(z1);
   
   // z2
   glm::vec3 z2p = glm::vec3(x_min, y_min, z_max);
   glm::vec3 z2q = glm::vec3(x_max, y_min, z_max);
   glm::vec3 z2r = glm::vec3(x_max, y_max, z_max);
   setupPlane(z2p, z2q, z2r, &z2);
   planes.push_back(z2);
   
   // x1
   glm::vec3 x1p = glm::vec3(x_min, y_min, z_max);
   glm::vec3 x1q = glm::vec3(x_min, y_min, z_min);
   glm::vec3 x1r = glm::vec3(x_min, y_max, z_min);
   setupPlane(x1p, x1q, x1r, &x1);
   planes.push_back(x1);
  
   // x2
   glm::vec3 x2p = glm::vec3(x_max, y_min, z_max);
   glm::vec3 x2q = glm::vec3(x_max, y_min, z_min);
   glm::vec3 x2r = glm::vec3(x_max, y_max, z_min); 
   setupPlane(x2p, x2q, x2r, &x2);
   planes.push_back(x2);
   
   if (&bounds != _bounds) {
      memcpy(&bounds, _bounds, sizeof(struct bound_box));
   }
}

void Object::setupPlane(glm::vec3 p, glm::vec3 q, glm::vec3 r, struct plane* plane) {
   /*glm::vec3 p = glm::vec3(x_min, y_min, z_min);
   glm::vec3 q = glm::vec3(x_max, y_min, z_min);
   glm::vec3 r = glm::vec3(x_max, y_max, z_min);*/
   glm::vec3 pq = q - p;
   glm::vec3 pr = r - p;
   
   glm::vec3 n = glm::cross(pq, pr);
   
   plane->a = n.x;
   plane->b = n.y;
   plane->c = n.z;
   plane->d = n.x*(-p.x) + n.y*(-p.y) + n.z*(-p.z);
}

void Object::step(float dt) {}

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
    }
    
    init();
}
 
void Object::load(const string &meshName, const string &matName)
{
	std::vector<tinyobj::material_t> objMaterials;
	string err = tinyobj::LoadObj(shapes, objMaterials, meshName.c_str(), matName.c_str());
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
	
	for(int i = 0; i < (int)posBuf.size(); i += 3) {
		posBuf[i  ] = (posBuf[i  ] - center.x) * scale;
		posBuf[i+1] = (posBuf[i+1] - center.y) * scale;
		posBuf[i+2] = (posBuf[i+2] - center.z) * scale;
   }
	
   init();
}

/* initialize a new shape */
void Object::init()
{
   vector<float> &posBuf = shapes[0].mesh.positions;
   vector<unsigned int> &indBuf = shapes[0].mesh.indices;
   vector<float> norBuf = computeNormals(posBuf, indBuf);
   vector<float> texBuf = shapes[0].mesh.texcoords;
   
   vector<unsigned short> indices;
   vector<glm::vec3> tangents;
   vector<glm::vec3> indexed_vertices, indexed_normals, indexed_tangents;
   vector<glm::vec2> indexed_uvs;
    
    if (bumpy) {
        /*
        // Get the tangents and bitangents (for bump mapping)
        computeTangentBasis(posBuf, texBuf, norBuf, tangents);
        indices = indexVBO_TBN(posBuf, texBuf, norBuf, tangents, indices, indexed_vertices, indexed_uvs, indexed_normals, indexed_tangents);
        
        // Send the tangents (for bump mapping)
        glGenBuffers(1, &tanBufID);
        glBindBuffer(GL_ARRAY_BUFFER, tanBufID);
        glBufferData(GL_ARRAY_BUFFER, indexed_tangents.size() * sizeof(glm::vec3), &indexed_tangents[0], GL_STATIC_DRAW);
         */
    }
    // Send the position array to the GPU
    glGenBuffers(1, &posBufID);
    glBindBuffer(GL_ARRAY_BUFFER, posBufID);
    glBufferData(GL_ARRAY_BUFFER, posBuf.size()*sizeof(float), &posBuf[0], GL_STATIC_DRAW);
    // Send the index array to the GPU
    glGenBuffers(1, &indBufID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBufID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indBuf.size()*sizeof(unsigned int) , &indBuf[0], GL_STATIC_DRAW);
    // Send the normal array (if it exists) to the GPU
    if(!norBuf.empty()) {
        glGenBuffers(1, &norBufID);
        glBindBuffer(GL_ARRAY_BUFFER, norBufID);
        glBufferData(GL_ARRAY_BUFFER, norBuf.size()*sizeof(float), &norBuf[0], GL_STATIC_DRAW);
    } else
        norBufID = 0;
    // Send the texture coordinate array (if it exists) to the GPU
    if (!texBuf.empty()) {
        glGenBuffers(1, &texBufID);
        glBindBuffer(GL_ARRAY_BUFFER, texBufID);
        glBufferData(GL_ARRAY_BUFFER, texBuf.size()*sizeof(float), &texBuf[0], GL_STATIC_DRAW);
    } else
        texBufID = 0;
    
	// Unbind the arrays
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

int Object::bind()
{
   glEnable(GL_TEXTURE_2D);
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

   // Enable and bind position array for drawing
   GLSL::enableVertexAttribArray(h_aPos);
   glBindBuffer(GL_ARRAY_BUFFER, posBufID);
   glVertexAttribPointer(h_aPos, 3, GL_FLOAT, GL_FALSE, 0, 0);

   // Enable and bind normal array (if it exists) for drawing
   if(norBufID) {
      GLSL::enableVertexAttribArray(h_aNor);
      glBindBuffer(GL_ARRAY_BUFFER, norBufID);
      glVertexAttribPointer(h_aNor, 3, GL_FLOAT, GL_FALSE, 0, 0);
   }
   
   // Bind index array for drawing
   int nIndices = (int)shapes[0].mesh.indices.size();
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBufID);
   
   // Enable and bind texture coordinate array (if it exists) for drawing
   if (texBufID) {
      GLint h_tex = GLSL::getAttribLocation(ShadeProg, "aTexCoord");
      GLSL::enableVertexAttribArray(h_tex);
      glBindBuffer(GL_ARRAY_BUFFER, texBufID);
      glVertexAttribPointer(h_tex, 2, GL_FLOAT, GL_FALSE, 0, 0);
   }
   // Enable and bind the tangent coordinates for norma mapping
   if (bumpy) {
       /*
      GLint h_tangent = GLSL::getAttribLocation(ShadeProg, "aTangent");
      GLSL::enableVertexAttribArray(h_tangent);
      glBindBuffer(GL_ARRAY_BUFFER, tanBufID);
      glVertexAttribPointer(h_tangent, 3, GL_FLOAT, GL_FALSE, 0, 0);
        */
   }
  
   glm::mat4 R = rotateMat * directionalMat;
   // Send the matrix information
   GLint h_rot = GLSL::getUniformLocation(ShadeProg, "uRot");
   Util::safe_glUniformMatrix4fv(h_rot, glm::value_ptr(R));
   getModelMatrix();
   Util::safe_glUniformMatrix4fv(h_uM, glm::value_ptr(modelMat));
   
   return nIndices;
}

void Object::draw()
{
    // Bind the object information to the shader
    int nIndices = bind();
    GLint texLoc = GLSL::getUniformLocation(ShadeProg, "uSampler0");
    glUniform1i(texLoc, 0);
    texLoc = GLSL::getUniformLocation(ShadeProg, "uSampler2");
    glUniform1i(texLoc, 2);
    
    // Enable basic FBO
    glBindFramebuffer(GL_FRAMEBUFFER, FBO_Basic);
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    
    // Draw the object to FBO
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);
    // Draw the shadow projection to FBO
    if (castShadows) {
        glUniform1f(GLSL::getUniformLocation(ShadeProg, "uTrans"), shadowDarkness);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TEX_MISC);
        glm::mat4 sT = glm::translate(glm::mat4(1.0f), glm::vec3(0, -.45 + shadowHeight, 0)) * ShadowMatrix();
        Util::safe_glUniformMatrix4fv(h_uM, glm::value_ptr(sT*modelMat));
        glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);
        glDisable(GL_CULL_FACE);
        glUniform1f(GLSL::getUniformLocation(ShadeProg, "uTrans"), 1.0);
    }
    
    // Draw the object to scene
    if (screenRender) {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glActiveTexture(GL_TEXTURE0);
        if (reflective) {
            
        }
        glBindTexture(GL_TEXTURE_2D, reflective ? FBO_TBasic : texture_id);
        glUniform1i(GLSL::getUniformLocation(ShadeProg, "BumpMode"), 0);
        if (bumpy) {
            glUniform1i(GLSL::getUniformLocation(ShadeProg, "BumpMode"), 1);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, norm_id);
        }
        Util::safe_glUniformMatrix4fv(h_uM, glm::value_ptr(modelMat));
        glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);
        glUniform1i(GLSL::getUniformLocation(ShadeProg, "BumpMode"), 0);
       
        // Draw the shadow projection to scene
        if (castShadows) {
            glUniform1f(GLSL::getUniformLocation(ShadeProg, "uTrans"), shadowDarkness);
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, TEX_MISC);
            glm::mat4 sT = glm::translate(glm::mat4(1.0f), glm::vec3(0, -.45 + shadowHeight, 0)) * ShadowMatrix();
            Util::safe_glUniformMatrix4fv(h_uM, glm::value_ptr(sT*modelMat));
            glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);
            glDisable(GL_CULL_FACE);
            glUniform1f(GLSL::getUniformLocation(ShadeProg, "uTrans"), 1.0);
        }
    }
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Object::computeTangentBasis(vector<float> &vertices,
                                 vector<float> &uvs,
                                 vector<float> &normals,
                                 vector<glm::vec3> &tangents) {
   
   for (int i = 0; i < vertices.size()/3; i+=3) {
      // shortcuts for vertices
      glm::vec3 v0 = glm::vec3(vertices[3*i+0], vertices[3*i+1], vertices[3*i+2]);
      glm::vec3 v1 = glm::vec3(vertices[3*(i+1)+0], vertices[3*(i+1)+1], vertices[3*(i+1)+2]);
      glm::vec3 v2 = glm::vec3(vertices[3*(i+2)+0], vertices[3*(i+2)+1], vertices[3*(i+2)+2]);
      
      // Shortcuts for UVs
      glm::vec2 uv0 = glm::vec2(uvs[3*i+0], uvs[3*i+1]);
      glm::vec2 uv1 = glm::vec2(uvs[3*(i+1)+0], uvs[3*(i+1)+1]);
      glm::vec2 uv2 = glm::vec2(uvs[3*(i+2)+0], uvs[3*(i+2)+1]);
      
      // Edges of the triangle : position delta
      glm::vec3 deltaPos1 = v1 - v0;
      glm::vec3 deltaPos2 = v2 - v0;
      
      // UV delta
      glm::vec2 deltaUV1 = uv1 - uv0;
      glm::vec2 deltaUV2 = uv2 - uv0;
      
      float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
      glm::vec3 tangent = r*(deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y);
      glm::vec3 bitangent = r*(deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x);
      
      // Set the same tangent for all three vertices of the triangle.
      tangents.push_back(tangent);
      tangents.push_back(tangent);
      tangents.push_back(tangent);
   }
}

// Returns true iif v1 can be considered equal to v2
bool is_near(float v1, float v2){
   return fabs( v1-v2 ) < 0.01f;
}
// Searches through all already-exported vertices for a similar one
bool getSimilarVertexIndex(glm::vec3 in_vertex,
                           glm::vec2 in_uv,
                           glm::vec3 in_normal,
                           std::vector<glm::vec3> out_vertices,
                           std::vector<glm::vec2> out_uvs,
                           std::vector<glm::vec3> out_normals,
                           unsigned short result)
{
   // Lame linear search
   for ( unsigned int i = 0; i < out_vertices.size(); i++ ){
      if (is_near( in_vertex.x , out_vertices[i].x ) &&
          is_near( in_vertex.y , out_vertices[i].y ) &&
          is_near( in_vertex.z , out_vertices[i].z ) &&
          is_near( in_uv.x     , out_uvs     [i].x ) &&
          is_near( in_uv.y     , out_uvs     [i].y ) &&
          is_near( in_normal.x , out_normals [i].x ) &&
          is_near( in_normal.y , out_normals [i].y ) &&
          is_near( in_normal.z , out_normals [i].z )
          ) {
         result = i;
         return true;
      }
   }
   // No other vertex could be used instead.
   // Looks like we'll have to add it to the VBO.
   return false;
}
vector<unsigned short> Object::indexVBO_TBN(vector<float> &in_vertices,
                  vector<float> &in_uvs,
                  vector<float> &in_normals,
                  vector<glm::vec3> &in_tangents,
                  vector<unsigned short> out_indices,
                  vector<glm::vec3> &out_vertices,
                  vector<glm::vec2> &out_uvs,
                  vector<glm::vec3> &out_normals,
                  vector<glm::vec3> &out_tangents) {
   
   for (int i = 0; i < in_vertices.size()/3; i++) {
      // Try to find a similar vertex in out
      unsigned short index;
      bool found = getSimilarVertexIndex(glm::vec3(in_vertices[3*i+0], in_vertices[3*i+1], in_vertices[3*i+2]),
          glm::vec2(in_uvs[3*i+0], in_uvs[3*i+1]),
          glm::vec3(in_normals[3*i+0], in_normals[3*i+1], in_normals[3*i+2]),
          out_vertices, out_uvs, out_normals, index);
       
      if (found) {
         out_indices.push_back(index);
         out_tangents[index] += in_tangents[i];
      } else {
         out_vertices.push_back(glm::vec3(in_vertices[3*i+0], in_vertices[3*i+1], in_vertices[3*i+2]));
         out_uvs.push_back(glm::vec2(in_uvs[3*i+0], in_uvs[3*i+1]));
         out_normals.push_back(glm::vec3(in_normals[3*i+0], in_normals[3*i+1], in_normals[3*i+2]));
         out_tangents.push_back(in_tangents[i]);
         out_indices.push_back((unsigned short)out_vertices.size() - 1);
      }
   }
   return out_indices;
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

glm::mat4 Object::getModelMatrix() {
   // Set the model transformation
   glm::vec3 position = pos + glm::vec3(0.0f, -0.5f, 0.0f);
   glm::mat4 T = glm::translate(glm::mat4(1.0f), position) * transMat;
   glm::mat4 R = rotateMat * directionalMat;
   if (directional) {
   glm::mat4 RX = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(1.0, 0.0, 0.0));
   glm::mat4 RY = glm::rotate(glm::mat4(1.0f), calcYFacingAngle(), glm::vec3(0.0, 1.0, 0.0));
   glm::mat4 RZ = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(0.0, 0.0, 1.0));
   R *= RX*RY*RZ;
   }
   modelMat = T*shearMat*R*scalerMat;
 
   return modelMat;
}

void Object::drawBox() {
	glLineWidth(3);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	
	// draw top rectangle
	glVertex3f(bounds.x_min, bounds.y_max, bounds.z_min); // 1
	glVertex3f(bounds.x_max, bounds.y_max, bounds.z_min); // 1
	glVertex3f(bounds.x_max, bounds.y_max, bounds.z_min); // 2
	glVertex3f(bounds.x_max, bounds.y_max, bounds.z_max); // 2
	glVertex3f(bounds.x_max, bounds.y_max, bounds.z_max); // 3
	glVertex3f(bounds.x_min, bounds.y_max, bounds.z_max); // 3
	glVertex3f(bounds.x_min, bounds.y_max, bounds.z_max); // 4
	glVertex3f(bounds.x_min, bounds.y_max, bounds.z_min); // 4
	
	// draw bottom rectangle
	glVertex3f(bounds.x_min, bounds.y_min, bounds.z_min); // 1
	glVertex3f(bounds.x_max, bounds.y_min, bounds.z_min); // 1
	glVertex3f(bounds.x_max, bounds.y_min, bounds.z_min); // 2
	glVertex3f(bounds.x_max, bounds.y_min, bounds.z_max); // 2
	glVertex3f(bounds.x_max, bounds.y_min, bounds.z_max); // 3
	glVertex3f(bounds.x_min, bounds.y_min, bounds.z_max); // 3
	glVertex3f(bounds.x_min, bounds.y_min, bounds.z_max); // 4
	glVertex3f(bounds.x_min, bounds.y_min, bounds.z_min); // 4
	
	// connect the top and bottom rectangles
	glVertex3f(bounds.x_min, bounds.y_min, bounds.z_min); // 1
	glVertex3f(bounds.x_min, bounds.y_max, bounds.z_min); // 1
	glVertex3f(bounds.x_min, bounds.y_min, bounds.z_max); // 2
	glVertex3f(bounds.x_min, bounds.y_max, bounds.z_max); // 2
	glVertex3f(bounds.x_max, bounds.y_min, bounds.z_min); // 3
	glVertex3f(bounds.x_max, bounds.y_max, bounds.z_min); // 3
	glVertex3f(bounds.x_max, bounds.y_min, bounds.z_max); // 4
	glVertex3f(bounds.x_max, bounds.y_max, bounds.z_max); // 4
	
	glEnd();
}

glm::vec3 Object::calculateNewPos(float dt) {
   if (vel > 0.0 || changeDir)
      vel += accel * dt;
   pos += dir * vel * dt;
   return pos;
}

void Object::setShadows(bool shadows, float height, float dark) {
   castShadows = shadows;
   shadowHeight = height;
   shadowDarkness = dark;
}
void Object::setTexture(int tex) {
   texture_id = tex;

   // reference for fresnel values: http://forums.cgsociety.org/archive/index.php/t-513458.html
   // Set cook torrance values depending on texture
   switch (tex) {
   case TEX_SKY_DAY:
      roughness = 1.0;
      fresnel = 1.0;
      geometric = 1.0;
      break;
   case TEX_WOOD_LIGHT:
   case TEX_WOOD_DARK:
   case TEX_WOOD_RED:
   case TEX_WOOD_WALL:
   case TEX_TARGET:
   case TEX_HAMMER:
      roughness = 0.8;
      fresnel = 1.3;
      geometric = 0.9;
      break;
   case TEX_LANTERN:
      roughness = 1.0;
      fresnel = 1.6;
      geometric = 1.0;
      break;
   case TEX_MELON_OUT:
      roughness = 0.2;
      fresnel = 1.6;
      geometric = 0.3;
      break;
   case TEX_MELON_IN:
      roughness = 0.5;
      fresnel = 1.6;
      geometric = 0.1;
      break;
   case TEX_PARTICLE:
      roughness = 0.0;
      fresnel = 1.0;
      geometric = 0.1;
      break;
   case TEX_MIKU:
   case TEX_RIN:
   case TEX_LEN:
   case TEX_KAITO:
   case TEX_SQUISH_BLUE:
   case TEX_SQUISH_PURPLE:
   case TEX_GIRL_RED:
   case TEX_GIRL_BLUE:
   case TEX_GIRL_GREEN:
   case TEX_GIRL_PINK:
      roughness = 0.6;
      fresnel = 1.6;
      geometric = 0.8;
      break;
   case TEX_STEEL:
      roughness = 0.1;
      fresnel = 2.5;
      geometric = 0.2;
      break;
   case TEX_GROUND_SAKURA:
   case TEX_GROUND_GRASS:
   case TEX_MISC:
   default:
      roughness = 1.0;
      fresnel = 1.6;
      geometric = 1.0;
      break;
   }
}
void Object::setNormalmap(int normal) {
   norm_id = normal;
}

void Object::shear(float shearX, float shearZ) {
   shearMat = glm::mat4(1.0f);

   shearMat[0][0] = 1; shearMat[0][1] = 0; shearMat[0][2] = shearX; shearMat[0][3] = 0;
   shearMat[1][0] = 0; shearMat[1][1] = 1; shearMat[1][2] = shearZ; shearMat[1][3] = 0;
   shearMat[2][0] = 0; shearMat[2][1] = 0; shearMat[2][2] = 1; shearMat[2][3] = 0;
   shearMat[3][0] = 0; shearMat[3][1] = 0; shearMat[3][2] = 0; shearMat[3][3] = 1;
}
