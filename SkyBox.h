#include "includes.h"
#include "Camera.h"
#include "Object.h"
#include "Model.h"
#include "Texture.h"

#define ASSIMP 0

class SkyBox
{
public:
    SkyBox(GLuint ShadeProg);
    virtual ~SkyBox();

    void init();
    void draw(Camera* camera, Window* window);

private:
    Object *skydome;
    vector<tinyobj::shape_t> shapes;
    vector<tinyobj::material_t> materials;
    
    GLuint ShadeProg;
};