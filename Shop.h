#ifndef __SHOP__
#define __SHOP__

#include "includes.h"
#include "Clicks.h"
#include "Texture.h"
#include "Window.h"
#include "Sound.h"
#include "FontEngine.h"

#define OUTFIT_TYPE "outfit"
#define CHARACTER_TYPE "character"
#define SONG_TYPE "song"

/* Represents a shop item */
class Item {

    // global shader programs
    map<string, Program*> shaders;

public:
    // constructor & destructor
    Item(char* _name, int _price, char* model, int texID, char* _type) {
        // Initialize the item
        name = _name;
        price = _price;
        type = _type;
        
        // Make the object
        object = new Object(shapes, materials, shaders[SHADER_DEFAULT]->getPID());
        object->setPos(glm::vec3(0.0, 1.0, 3.0));
        object->setTexture(texID);
        object->setShadows(false);
    }
    
    // getters and setters
    Object *object;
    char *name, *type;
    int price;

private:
    vector<tinyobj::shape_t> shapes;
    vector<tinyobj::material_t> materials;
};

class Shop {
public:
    // constructor & deconstructor
    Shop(GLuint _ShadeProg, Sound* _sound);
    virtual ~Shop();
    
    // called by session
    void mouseClick(glm::vec3 direction, glm::vec4 point);
    void step(Window* window);
    
    // called by main (uses keyboard input)
    void selectItem();
    void nextItem();
    void prevItem();
    void nextCategory();
    void prevCategory();

    // getters and setters
    bool gameOver, gameStart;
    
private:
    // Variables
    vector<Item> items;
    vector<Object*> misc_objects;
    vector<tinyobj::shape_t> shapes;
    vector<tinyobj::material_t> materials;
    GLuint ShadeProg;
    Sound* sound;
    
    // Shop variables
    int curItem;
    
    // Shop functions
    void setUp();
    void buyItem();
    
    // Text functions
    void printInstructions();
};

#endif