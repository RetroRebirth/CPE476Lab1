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
    
    printf("\t\tPress the arrow keys to choose the target\n");
    printf("\t\tIf you hit the wrong target you lose points, so be careful!\n");
}

Karaoke::~Karaoke() {
    for (int i = 0; i < characters.size(); i++) {
        delete characters[i];
    }
    delete arrow;
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
    
    // TEMP: add an arrow
    arrow = new Object(shapes, materials, ShadeProg);
    arrow->load("objs/arrow.obj");
    arrow->setTexture(curTarget + CHARA_TEX);
    arrow->scale(glm::vec3(1.0, 1.0, 1.0));
    arrow->setPos(glm::vec3(2.0, 3.5, 1.0));
    arrow->setShadows(false);
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

void Karaoke::addArrow() {
    // Choose a target position
    curTarget = Util::randF() * 4;
    characters[curTarget]->setTexture(curTarget + CHARA_TEX);
    
    // Decide where to place the arrow
    if (curTarget == MIKU) {
        arrow->setTexture(curTarget + CHARA_TEX);
        arrow->setPos(glm::vec3(2.0, 3.5, 1.0));
        arrow->rotate(90.0f, glm::vec3(0.0, 0.0, 1.0));
    }
    else if (curTarget == RIN) {
        arrow->setTexture(curTarget + CHARA_TEX);
        arrow->setPos(glm::vec3(0.7, 3.5, 1.0));
        arrow->rotate(-180.0f, glm::vec3(1.0, 0.0, 0.0));
    }
    else if (curTarget == LEN) {
        arrow->setTexture(curTarget + CHARA_TEX);
        arrow->setPos(glm::vec3(-0.7, 3.5, 1.0));
        arrow->rotate(0.0f, glm::vec3(1.0, 0.0, 0.0));
    }
    else if (curTarget == KAITO) {
        arrow->setTexture(curTarget + CHARA_TEX);
        arrow->setPos(glm::vec3(-2.0, 3.5, 1.0));
        arrow->rotate(-90.0f, glm::vec3(0.0, 0.0, 1.0));
    }
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
            // Add a new arrow
            if (curTarget == -1) {
                addArrow();
            }
        }
        
    }
}

void Karaoke::step(Window* window) {
    // Draw the booth
    screen->draw();
    
    // Check how much time has passed and whether game is playing
    if (gameOver || !songChosen)
        return;
    checkTime(window);
    
    // Draw the characters
    for (int i = 0; i < characters.size(); i++)
        characters[i]->draw();

    // Draw the arrows
    if (!gameStart)
        return;
    arrow->draw();
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
    
}