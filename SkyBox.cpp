#include "SkyBox.h"

using namespace std;

SkyBox::SkyBox(GLuint _ShadeProg)
{
    // Defined attribute values
    ShadeProg = _ShadeProg;

    skydome = new Object(shapes, materials, ShadeProg);
    skydome->load("objs/skydome_day.obj", "objs/SkyDome-Cloud-Wispy-MidMorning.bmp");
    skydome->scale(glm::vec3(SKY_SIZE, SKY_SIZE, SKY_SIZE));
    skydome->setTexture(TEX_SKY_DAY);
    skydome->setShadows(false);
}

SkyBox::~SkyBox() {}

void SkyBox::draw(Camera* camera, Window* window)
{
    skydome->setPos(camera->pos);
    skydome->draw();
}