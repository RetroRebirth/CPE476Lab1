#include "SkyBox.h"

using namespace std;

SkyBox::SkyBox(GLuint _ShadeProg)
{
    // Defined attribute values
    ShadeProg = _ShadeProg;
    h_aPos = GLSL::getAttribLocation(ShadeProg, "aPos");
    h_aNor = GLSL::getAttribLocation(ShadeProg, "aNor");
    h_uM = GLSL::getUniformLocation(ShadeProg, "uM");
    h_uTexUnit = GLSL::getUniformLocation(ShadeProg, "uTexUnit");
    
#if ASSIMP == 1
    printf("Hello world\n");
    Texture *skytexture = new Texture();
    skytexture->loadTexture("objs/SkyDome-Cloud-Wispy-MidMorning.bmp", true, h_uTexUnit);
    printf("loaded texture\n");
    
    printf("Trying to make a new Model\n");
    Model *skydome = new Model();
    skydome->loadModel((char *)"objs/skydome_day.obj", h_uTexUnit);
    printf("loaded model\n");
    skydome->finalizeModel(ShadeProg);
    printf("finalized model\n");
    skydome->bindModel();
    printf("bound model\n");
#else
   skydome = new Object(shapes, materials, ShadeProg);
   skydome->load("objs/skydome_day.obj", "objs/SkyDome-Cloud-Wispy-MidMorning.bmp");
   skydome->scale(glm::vec3(SKY_SIZE, SKY_SIZE, SKY_SIZE));
   skydome->setTexture(TEXTURE_SKY);
    /*
    printf("Sky texcoords:\n");
    for (int i = 0; i < skydome->shapes[0].mesh.texcoords.size() / 2; i++) {
        printf("%lf %lf\n", skydome->shapes[0].mesh.texcoords[2*i+0], skydome->shapes[0].mesh.texcoords[2*i+1]);
    }
     */
#endif
}

SkyBox::~SkyBox() {}

void SkyBox::draw(Camera* camera, Window* window)
{
#if ASSIMP == 1
    skydome->drawModel();
#else
    skydome->setPos(camera->pos);
    skydome->draw();
#endif
}