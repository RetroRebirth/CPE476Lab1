#include "Karaoke.h"

Karaoke::Karaoke(GLuint _ShadeProg, Sound* _sound) {
    
    // Inititalize the game
    ShadeProg = _ShadeProg;
    sound = _sound;
    gameOver = gameStart = songChosen = false;
    timeStart = timeSong = 0.0;
    curTarget = -1;
    score = 0;
    curSong = 0;
    
    // Set up the booth
    setUp();
    
    // Display the game description and rules
    printf("\t\t----- Welcome to the KARAOKE MACHINE-----\n\n");
    printf("\t\tHere you can relax and listen to your favorite anime songs\n");
    printf("\t\tUse the LEFT and RIGHT arrow keys to choose a song\n");
    printf("\t\tPress ENTER to select your song\n\n");
    
    printf("\t\tClick on the colored character to earn points\n");
    printf("\t\tYou can also press the arrow keys to choose the target\n");
    printf("\t\tIf you hit the wrong target you lose points, so be careful!\n");
}

Karaoke::~Karaoke() {
    for (int i = 0; i < bullets.size(); ++i) {
        delete bullets[i];
    }
    for (int i = 0; i < characters.size(); i++) {
        delete characters[i];
    }
    for (int i = 0; i < misc_objects.size(); i++) {
        delete misc_objects[i];
    }
    sound->playBackgroundMusic();
}

void Karaoke::setUp() {
    // Create a "screen" in the back for projecting the video
    screen = new Object(shapes, materials, ShadeProg);
    screen->load((char *)"objs/screen.obj");
    screen->setTexture(curSong + NUM_TEXTURES);
    screen->setPos(glm::vec3(0.0, 2.7, 2.0));
    screen->scale(glm::vec3(7.0, 10.0, 7.0));
    screen->setShadows(false);
}

void Karaoke::addCharacter(char *file, int tex, float xPos) {
    Object *chara = new Object(shapes, materials, ShadeProg);
    chara->load(file);
    chara->setTexture(tex);
    chara->setPos(glm::vec3(xPos, 1.5, 1.0));
    chara->scale(glm::vec3(1.0, 1.0, 1.0));
    chara->setShadows(false);
    chara->updateRadius();
    characters.push_back(chara);
}

void Karaoke::checkTime(Window *window) {
    // Initialize the time if not done so already
    if (timeStart == 0.0) {
        timeStart = window->time;
        srand(timeStart);
    }
    else {
        // Begin the timer countdown for the song to start
        if (!gameStart) {
            if (window->time - timeStart >= 4.0) {
                timeSong = sound->playKaraokeMusic(curSong) / 1000.0;
                timeStart = window->time;
                for (int i = 0; i < characters.size(); i++)
                    characters[i]->setTexture(TEX_MISC);
                gameStart = true;
            }
            if (window->time - timeStart >= 3.0 && characters.size() < 4) {
                addCharacter((char *)"objs/kaito.obj", TEX_KAITO, -2.0);
            }
            else if (window->time - timeStart >= 2.0 && characters.size() < 3) {
                addCharacter((char *)"objs/len.obj", TEX_LEN, -0.7);
            }
            else if (window->time - timeStart >= 1.0 && characters.size() < 2) {
                addCharacter((char *)"objs/rin.obj", TEX_RIN, 0.7);
            }
            else if (window->time - timeStart >= 0.0 && characters.size() < 1) {
                addCharacter((char *)"objs/miku.obj", TEX_MIKU, 2.0);
            }
        }
        else {
            // Check whether the game has ended
            if (window->time - timeStart >= timeSong) {
                printf("Time's up! Your score is: %d\n", score);
                gameOver = true;
            }
            // Add a new "target"
            if (curTarget == -1) {
                curTarget = Util::randF() * 4;
                characters[curTarget]->setTexture(curTarget + CHARA_TEX);
            }
        }
        
    }
}

void Karaoke::step(Window* window) {
    // Draw the booth
    //for (int i = 0; i < misc_objects.size(); i++)
    //    misc_objects[i]->draw();
    screen->draw();
    
    // Check how much time has passed and whether game is playing
    if (gameOver || !songChosen)
        return;
    checkTime(window);
    
    // Draw the characters
    for (int i = 0; i < characters.size(); i++)
        characters[i]->draw();
    
    // Fire the bullets
    /*
    for (int i = 0; i < bullets.size(); i++){
        if (bullets[i]->getPos().z <= 1.0 && bullets[i]->getPos().z >= -1.0) {
            if (bullets[i] != NULL) {
                bullets[i]->setPos(bullets[i]->calculateNewPos(window->dt));
                
                // Check collision against the current target
                if (bullets[i]->collidedWithObj(*characters[curTarget], window->dt)) {
                    //sound->playContactSound();
                    characters[curTarget]->setTexture(TEX_MISC);
                    score++;
                    curTarget = -1;
                }
                else {
                    score--;
                }
                printf("score is: %d\n", score);
            }
        }
        // Remove the bullet if it has gone past the target
        else
            bullets.erase(bullets.begin() + i--);
    }
     */
}

void Karaoke::selectCharacter(int target) {
    if (target == curTarget) {
        characters[curTarget]->setTexture(TEX_MISC);
        score++;
        curTarget = -1;
    }
    else {
        score--;
    }
    printf("score is: %d\n", score);
}

void Karaoke::nextSong() {
    curSong = (curSong + 1) % NUM_SONGS;
    screen->setTexture(curSong + NUM_TEXTURES);
}

void Karaoke::prevSong() {
    curSong = (curSong - 1) % NUM_SONGS;
    screen->setTexture(curSong + NUM_TEXTURES);
}

void Karaoke::selectSong() {
    songChosen = true;
}

void Karaoke::mouseClick(glm::vec3 direction, glm::vec4 point) {
    /*
    // Shoot a "bullet"
    Object* bullet = new Object(shapes, materials, ShadeProg);
    bullet->load("sphere.obj");
    bullet->setPos(glm::vec3(point.x, point.y - 7.5, 0));
    bullet->setDir(direction);
    bullet->setSpeed(1.0f);
    bullet->setTexture(TEX_WOOD_WALL);
    bullet->setShadows(false);
    bullet->setSpeed(30.0);
    bullet->scale(glm::vec3(0.2, 0.2, 0.2));
    bullets.push_back(bullet);
     */
}