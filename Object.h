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
    
    // position
    glm::vec3 pos;
    
    // bounding box
    struct bound_box bounds;
    
    // methods
    void init();
    void load(const string &meshName);
    void load(const string &meshName, const string &matName);
    void resize_obj();
    void step(float dt);
    void draw();
    
    // collision detection
    // circular bounds
    bool collidedWithPlayer(glm::vec3 camPos, float dt);
    bool collidedWithWall(float dt);
    bool collidedWithObj(Object o, float dt);
    bool collision(Object* o);
    // boxed bounds
    bool checkCollision(Object* _otherObject);
    void getCollisionAxis(glm::vec3 pos, glm::vec3* colPlane);
    bool checkCameraCollision(glm::vec3 cam_pos, glm::vec3 *colPlane);
    bool checkPlayerCollision(Object* player, glm::vec3* colPlane);
    
    // setters
    void setPos(glm::vec3 position)     { pos = position;    }
    void setDir(glm::vec3 direction)    { dir = direction;   }
    void setSpeed(float speed)          { vel = speed;       }
    void setTexture(int tex)            { texture_id = tex;  }
    void setDirectional(bool dir)       { directional = dir; }
    void setShadows(bool shadows)       { castShadows = shadows; }
    
    // getters
    glm::vec3 getPos()        { return pos; }
    glm::vec3 getDir()        { return dir; }
    glm::vec3 calculateNewPos(float dt) { return pos + dir * vel * dt; }
    int getTexture()          { return texture_id; }
    float getRadius()         { return radius; }
    float getSpeed()          { return vel; }
    bool isDirectional()      { return directional; }
    void getBounds(struct bound_box *bounds);
    float getXZRadius();
    vector<tinyobj::shape_t> getShapes() { return shapes; }
    vector<tinyobj::material_t> getMaterials() { return materials; }
    glm::mat4 getModelMatrix();
    
    // modifiers
    void scale(glm::vec3 scalar);
    void rotate(float angle, glm::vec3 axis);
    void translate(glm::vec3 trans);
    
    // Draw Bounding boxes?
    bool drawBounds;

private:
    // Object information
    glm::vec3 dimensions, dir, col;
    
    // bounding box debug
    void drawBox();
    
    float vel, shine, radius;
    bool collected, directional, castShadows;
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
    
    // Get the shadow projection matrix (for drawing a shadow)
    glm::mat4 ShadowMatrix()
    {
        glm::mat4 ShadowMatrix;
        float ground[4] = {0.0, 1.0, 0.0, 0.0};
        float light[4] = {-1.0, 1.0, 0.278, 0.0};
        float dot = ground[0] * light[0] +
            ground[1] * light[1] +
            ground[2] * light[2] +
            ground[3] * light[2];
        ShadowMatrix[0][0] = dot - light[0] * ground[0];
        ShadowMatrix[1][0] = 0.0 - light[0] * ground[1];
        ShadowMatrix[2][0] = 0.0 - light[0] * ground[2];
        ShadowMatrix[3][0] = 0.0 - light[0] * ground[3];
        ShadowMatrix[0][1] = 0.0 - light[1] * ground[0];
        ShadowMatrix[1][1] = dot - light[1] * ground[1];
        ShadowMatrix[2][1] = 0.0 - light[1] * ground[2];
        ShadowMatrix[3][1] = 0.0 - light[1] * ground[3];
        ShadowMatrix[0][2] = 0.0 - light[2] * ground[0];
        ShadowMatrix[1][2] = 0.0 - light[2] * ground[1];
        ShadowMatrix[2][2] = dot - light[2] * ground[2];
        ShadowMatrix[3][2] = 0.0 - light[2] * ground[3];
        ShadowMatrix[0][3] = 0.0 - light[3] * ground[0];
        ShadowMatrix[1][3] = 0.0 - light[3] * ground[1];
        ShadowMatrix[2][3] = 0.0 - light[3] * ground[2];
        ShadowMatrix[3][3] = dot - light[3] * ground[3];
        return ShadowMatrix;
    }
};

#endif
