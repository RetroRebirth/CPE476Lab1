#include "Shop.h"

Shop::Shop(GLuint _ShadeProg, Sound* _sound) {
    ShadeProg = _ShadeProg;
    sound = _sound;
    curItem = 0;
    gameStart = false;
    
    setUp();
}

Shop::~Shop() {
    for (int i = 0; i < items.size(); i++)
        delete items[i].object;
    for (int i = 0; i < misc_objects.size(); i++)
        delete misc_objects[i];
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
    
    // Load the songs
    vector<Song> karaoke_songs = sound->getSongs();
    for (int i = 0; i < karaoke_songs.size(); i++) {
        // Check that the song is not already unlocked
        if (!karaoke_songs[i].unlocked) {
            Item newItem;
            newItem.name = karaoke_songs[i].song_name;
            newItem.price = karaoke_songs[i].price;
            newItem.type = (char *)"song";
            newItem.index = i;
            
            Object *object = new Object(shapes, materials, ShadeProg);
            object->load((char *)"objs/screen.obj");
            object->setTexture(i + TEX_SONGS);
            object->setShadows(false);
            object->scale(glm::vec3(5.0, 7.0, 5.0));
            object->setPos(glm::vec3(0.0, 2.7, 3.0));
            newItem.object = object;
            
            items.push_back(newItem);
        }
    }
}

void Shop::buyItem() {
    // Not enough points to buy the item
    if (global_points < items[curItem].price) {
        printf("You just remembered you're poor...\n");
        printf("Play some minigames to earn more cash!\n");
        sound->playBuzzerSound();
    }
    // Item was already unlocked
    else if (items[curItem].price == 0) {
        printf("You've already purchased this item.");
        sound->playBuzzerSound();
    }
    // Buy the item
    else {
        printf("Bought %s\n", items[curItem].name);
        items[curItem].price = 0;
        sound->playContactSound();
        
        // Item is a song; unlock it
        if (strcmp(items[curItem].type, SONG_TYPE) == 0) {
            printf("You can now play this song in the karaoke booth!\n");
            sound->unlockSong(items[curItem].index);
        }
    }
}

void Shop::printInstructions() {
    ifstream instrFile;
    instrFile.open("shopinstr.txt");
    string line;
    float yPos = .5;
    float yInc;
    
    fontEngine->useFont("ostrich", 30);
    yInc = fontEngine->getTextHeight("blank") * 1.3;
    
    if (instrFile.is_open()) {
        while (getline(instrFile, line)) {
            if (line[0] == '\n') {
                yPos -= yInc;
            }
            yPos -= yInc;
            fontEngine->display(glm::vec4(1.0, 1.0, 1.0, 1.0), line.c_str(), 0-fontEngine->getTextWidth(line.c_str())/2.0, yPos);
        }
    }
    else {
        printf("file 'shopinstr.txt' was not available or could not be opened\n");
    }
}

void Shop::step(Window* window) {
    // Draw the booth
    for (int i = 0; i < misc_objects.size(); i++)
        misc_objects[i]->draw();
    
    // Draw the instructions
    if (!gameStart) {
        printInstructions();
        return;
    }
    
    float yInc, yTop = .53, yBot = -.48; 

    fontEngine->useFont("ostrich", 30);
    yInc = fontEngine->getTextHeight("blank") * 1.3;

    fontEngine->display(glm::vec4(1.0, 1.0, 1.0, 1.0), items[curItem].name, 0-fontEngine->getTextWidth(items[curItem].name)/2.0, yTop);

    char price[15];
    sprintf(price, "Price: %d", items[curItem].price);
    fontEngine->display(glm::vec4(1.0, 1.0, 1.0, 1.0), price, 0-fontEngine->getTextWidth(price)/2.0, yBot);
    
    // Draw the item
    items[curItem].draw();
}

void Shop::prevItem() {
    if (gameStart) {
        curItem = (curItem - 1) % items.size();
        sound->playJumpSound();
    }
}
void Shop::nextItem() {
    if (gameStart) {
        curItem = (curItem + 1) % items.size();
        sound->playJumpSound();
    }
}
void Shop::prevCategory() {
    
}
void Shop::nextCategory() {
    
}
void Shop::selectItem() {
    buyItem();
}

/* DO NOTHING -- game does not use the mouse clicks */
void Shop::mouseClick(glm::vec3 direction, glm::vec4 point) {}
