#include "includes.h"
#include "Camera.h"
#include "Object.h"
#include "Model.h"

#define ASSIMP 0

class SkyBox
{
public:
    SkyBox(GLuint ShadeProg);
    virtual ~SkyBox();

    void init();
    void draw(Camera* camera, Window* window);

private:
#if ASSIMP == 1
    Model *skydome;
#else
    Object *skydome;
#endif
    vector<tinyobj::shape_t> shapes;
    vector<tinyobj::material_t> materials;
    
    GLuint ShadeProg;
    GLint h_aPos, h_aNor, h_uM, h_uTexUnit;
};