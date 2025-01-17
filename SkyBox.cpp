#include "SkyBox.h"

using namespace std;

SkyBox::SkyBox(GLuint _ShadeProg)
{
    // Defined attribute values
    ShadeProg = _ShadeProg;

    skydome = new Object(shapes, materials, ShadeProg);
    skydome->load("objs/skydome_day.obj", "textures/skydome.bmp");
    skydome->scale(glm::vec3(SKY_SIZE, SKY_SIZE, SKY_SIZE));
    skydome->setTexture(textures[TEX_SKY_DAY]);
    skydome->setShadows(false, 0.0, 0.0);
}

SkyBox::~SkyBox() {}

void SkyBox::draw(Camera* camera, Window* window)
{
    skydome->setPos(camera->pos);
    skydome->draw();
}
