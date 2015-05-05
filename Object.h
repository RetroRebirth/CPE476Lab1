#ifndef __Object__
#define __Object__

#include "includes.h"

class Object {
public:
    // constructors & destructors
    Object(vector<tinyobj::shape_t> &_shapes,
           vector<tinyobj::material_t> &_materials,
           GLuint ShadeProg);
    virtual ~Object();
    
    // methods
    void init();
    void load(const string &meshName);
    void load(const string &meshName, const string &matName);
    void resize_obj();
    void step(float dt);
    void draw();
    
    // collision detection
    bool collidedWithPlayer(glm::vec3 camPos, float dt);
    bool collidedWithWall(float dt);
    bool collidedWithObj(Object o, float dt);
    bool collision(Object* o);
    
    // setters
    void setPos(glm::vec3 position)     { pos = position;    }
    void setDir(glm::vec3 direction)    { dir = direction;   }
    void setSpeed(float speed)          { vel = speed;       }
    void setTexture(int tex)            { texture_id = tex;  }
    void setDirectional(bool dir)       { directional = dir; }
    
    // getters
    glm::vec3 getPos()        { return pos; }
    glm::vec3 getDir()        { return dir; }
    glm::vec3 calculateNewPos(float dt) { return pos + dir * vel * dt; }
    int getTexture()          { return texture_id; }
    float getRadius()         { return radius; }
    float getSpeed()          { return vel; }
    bool isDirectional()      { return directional; }
    void getBounds(struct bound_box *bounds);
    float getXRadius();
    vector<tinyobj::shape_t> getShapes() { return shapes; }
    vector<tinyobj::material_t> getMaterials() { return materials; }
    glm::mat4 getModelMatrix();
    
    // modifiers
    void scale(glm::vec3 scalar);
    void rotate(float angle, glm::vec3 axis);
    void translate(glm::vec3 trans);

private:
    // Object information
    glm::vec3 pos, dimensions, dir;
    float vel, shine, radius;
    bool collected, directional;
    vector<tinyobj::shape_t> shapes;
    vector<tinyobj::material_t> materials;
    
    // Model matrices
    glm::mat4 modelMat;
    glm::mat4 scalerMat, boundBoxScalerMat;
    glm::mat4 rotateMat;
    glm::mat4 transMat;
    
    // Shader information
    GLuint ShadeProg;
    GLuint posBufID, indBufID, norBufID, texBufID;
    GLint h_aPos, h_aNor, h_uM;
    
    // Texture information
    int texture_id;
    
    // Helper functions
    vector<float> computeNormals(vector<float> posBuf, vector<unsigned int> indBuf);
    float calcYFacingAngle();
};

#endif
