#include "Shop.h"

Shop::Shop(GLuint _ShadeProg, Sound* _sound) {
    // Initialize the shop
    ShadeProg = _ShadeProg;
    sound = _sound;
    curItem = 0;
}

Shop::~Shop() {
    for (int i = 0; i < items.size(); i++)
        delete items[i].object;
    for (int i = 0; i < misc_objects.size(); i++)
        delete misc_objects[i];
}

void Shop::buyItem() {
    // Item has already been purchased
    if (items[curItem].price == 0) {
        // Item is an outfit; equip it
        if (strcmp(items[curItem].type, OUTFIT_TYPE) == 0) {
            printf("Now wearing %s!\n", items[curItem].name);
        }
        // Item is a character; change player models
        else if (strcmp(items[curItem].type, CHARACTER_TYPE) == 0) {
            printf("Now playing as %s\n", items[curItem].name);
        }
        // Item is a song; play it
        else if (strcmp(items[curItem].type, SONG_TYPE) == 0) {
            printf("Now playing %s\n", items[curItem].name);
        }
    }
    // Not enough points to buy the item
    /*
    else if (global_points < items[curItem].price) {
        printf("You just remembered you're poor...\n");
        printf("Play some minigames to earn more cash!\n");
    }
     */
    // Buy the item
    else {
        printf("Bought %s\n", items[curItem].name);
        items[curItem].price = 0;
    }
}

void Shop::setUp() {
    // Make the booth
    Object *table = new Object(shapes, materials, ShadeProg);
    table->load("cube.obj");
    table->setPos(glm::vec3(0.0, 0.0, 3.0));
    table->scale(glm::vec3(10.0f, 1.0f, 1.0f));
    table->setTexture(TEX_WOOD_DARK);
    table->setShadows(false);
    misc_objects.push_back(table);
    
    // Make the wall
    Object *wall = new Object(shapes, materials, ShadeProg);
    wall->load("cube.obj");
    wall->setPos(glm::vec3(0.0, 0.0, 30.0));
    wall->scale(glm::vec3(100.0f, 100.0f, 1.0f));
    wall->setTexture(TEX_WOOD_LIGHT);
    wall->setShadows(false);
    misc_objects.push_back(wall);
}

/* LEFT: Switches to the previous item in the shop */
void Shop::prevItem() {
    
}
/* RIGHT: Switches to the next item in the shop */
void Shop::nextItem() {
    
}
/* DOWN: Jumps to the next category (all/clothes/models/songs) */
void Shop::prevCategory() {
    
}
/* UP: Jumps to the previous category (all/clothes/models/songs) */
void Shop::nextCategory() {
    
}
/* Attempts to buy the selected item (else equips it) */
void Shop::selectItem() {
    buyItem();
}

/* DO NOTHING -- game does not use the mouse clicks */
void Shop::mouseClick(glm::vec3 direction, glm::vec4 point) {}