#include "Karaoke.h"

Karaoke::Karaoke(GLuint _ShadeProg, Sound* _sound) {
    
    // Inititalize the game
    ShadeProg = _ShadeProg;
    sound = _sound;
    gameOver = gameStart = songChosen = false;
    timeStart = timeArrow = timeCur = 0.0;
    curTarget = -1;
    score = curSong = numGood = numBad = numPerfect = 0;
    speed = 1;
    
    // Set up the booth
    setUp();
    
    // Display the game description and rules
    printf("\t\t----- Welcome to the KARAOKE MACHINE-----\n\n");
    printf("\t\tHere you can relax and listen to your favorite anime songs\n");
    printf("\t\tUse the LEFT and RIGHT arrow keys to choose a song\n");
    printf("\t\tUse the UP and DOWN arrow keys to choose a difficulty\n");
    printf("\t\tPress ENTER to start playing the song\n\n");
    
    printf("\t\tPress the arrow keys as they appear to earn points\n");
    printf("\t\tEarn more points for hitting the arrows at the bottom\n");
    printf("\t\tIf you hit the wrong arrow or miss an arrow you lose points\n");
    printf("\t\tIf you miss too many arrows, it's GAME OVER!\n\n");
    
    printf("\t\tYou can purchase additional songs at the SHOP BOOTH\n");
}

Karaoke::~Karaoke() {
    for (int i = 0; i < characters.size(); i++) {
        delete characters[i];
    }
    delete arrow;
    delete screen;
    
    sound->playBackgroundMusic();
}

void Karaoke::setUp() {
    // Create a "screen" in the back for projecting the video
    screen = new Object(shapes, materials, ShadeProg);
    screen->load((char *)"objs/screen.obj");
    screen->setTexture(curSong + NUM_TEXTURES);
    screen->setPos(glm::vec3(0.0, 2.7, 2.0));
    screen->scale(glm::vec3(5.0, 7.0, 5.0));
    screen->setShadows(false);
    
    // Create the arrow
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
    arrow->translate(glm::vec3(0.0, 0.0, 0.0));
    
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
                bpm = sound->playKaraokeMusic(curSong);
                songDuration = sound->getSongDuration() / 1000.0;
                beat = songDuration / ((songDuration / 60.0) * bpm);
                beat *= (3 - speed) + 1;
                timeStart = window->time;
                for (int i = 0; i < characters.size(); i++)
                    characters[i]->setTexture(TEX_MISC);
                gameStart = true;
            }
            if (window->time - timeStart >= 3.0 && characters.size() < 4)
                addCharacter((char *)"objs/kaito.obj", TEX_KAITO, -2.0);
            else if (window->time - timeStart >= 2.0 && characters.size() < 3)
                addCharacter((char *)"objs/len.obj", TEX_LEN, -0.7);
            else if (window->time - timeStart >= 1.0 && characters.size() < 2)
                addCharacter((char *)"objs/rin.obj", TEX_RIN, 0.7);
            else if (window->time - timeStart >= 0.0 && characters.size() < 1)
                addCharacter((char *)"objs/miku.obj", TEX_MIKU, 2.0);
        }
        else {
            timeCur = window->time;
            // Check whether the game has ended
            if (window->time - timeStart >= songDuration) {
                printf("Time's up! Your score is: %d\n", score);
                printf("BAD: %d\nGOOD: %d\nPERFECT: %d\n", numBad, numGood, numPerfect);
                printf("Percentage: %.2f\n", (double)(numGood + numPerfect) / (numGood + numPerfect + numBad) * 100.0);
                gameOver = true;
            }
            // Add a new arrow
            if (curTarget == -1) {
                addArrow();
                timeArrow = window->time;
            }
            // Arrow time is up; remove the arrow
            else if (window->time - timeArrow >= beat) {
                sound->playBuzzerSound();
                /* There seems to be a bug where if the arrow disappears at the
                 * Same time you hit it, it segfaults because curTarget = -1
                 */
                if (curTarget != -1)
                    characters[curTarget]->setTexture(TEX_MISC);
                score -= speed;
                numBad++;
                curTarget = -1;
            }
            // Move the arrow down the screen
            else {
                arrowPos = (-2.0 / beat) * (window->time - timeArrow);
                arrow->translate(glm::vec3(0.0, arrowPos, 0.0));
            }
        }
    }
}

void Karaoke::step(Window* window) {
    // Draw the booth
    screen->draw();

    // Player is still choosing a song; display song info
    if (!songChosen) {
        
        // Display the difficulty
        char msg1[30];
        char* diff = (char *)"EASY";
        if (speed == 2)         diff = (char *)"MEDIUM";
        else if (speed == 3)    diff = (char *)"HARD";
        sprintf(msg1, "Difficulty: %s", diff);
        fontEngine->display(glm::vec4(1.0, 1.0, 1.0, 1.0), 48, msg1, -0.6, -0.5);
        
        // Display the song name
        char msg2[30], lock[6];
        if (!sound->getSongInfo(curSong).unlocked)
            sprintf(lock, "(%d)", sound->getSongInfo(curSong).price);
        else
            sprintf(lock, "");
        sprintf(msg2, "%s %s", sound->getSongInfo(curSong).song_name, lock);
        fontEngine->display(glm::vec4(1.0, 1.0, 1.0, 1.0), 48, msg2, -0.6, 0.55);
        
        return;
    }
    checkTime(window);
    
    // Draw the characters and arrows
    for (int i = 0; i < characters.size(); i++)
        characters[i]->draw();
    if (!gameStart)
        return;
    arrow->draw();
    
    // Check for game over
    if (score < -10) {
        gameOver = true;
    }
    
    // Display the score
    char scrStr[15];
    sprintf(scrStr, "score: %d", score);
    fontEngine->display(glm::vec4(1.0, 0.0, 0.0, 1.0), 48, scrStr, -0.1, 0.0);
}

// Chooses a song on song selection menu
void Karaoke::nextSong() {
    curSong = (curSong + 1) % sound->getSongs().size();
    screen->setTexture(curSong + NUM_TEXTURES);
}
void Karaoke::prevSong() {
    curSong = curSong - 1;
    if (curSong == -1)
        curSong = sound->getSongs().size() - 1;
    screen->setTexture(curSong + NUM_TEXTURES);
}
void Karaoke::selectSong() {
    if (!sound->getSongInfo(curSong).unlocked) {
        sound->unlockSong(curSong);
        return;
    }
    songChosen = true;
}

// Changes the difficulty setting
void Karaoke::increaseDifficulty() {
    if (speed < 3)
        speed++;
}
void Karaoke::decreaseDifficulty() {
    if (speed > 1)
        speed--;
}

void Karaoke::selectCharacter(int target) {
    // Hit the right arrow
    if (target == curTarget) {
        // Check whether a "PERFECT" score was earned
        if (arrowPos <= -1.80) {
            sound->playContactSound();
            score += speed * 2;
            numPerfect++;
        }
        // Otherwise, "GOOD" score
        else {
            sound->playJumpSound();
            score += speed;
            numGood++;
        }
    }
    // "BAD" score; wrong arrow
    else {
        sound->playBuzzerSound();
        score -= speed;
        numBad++;
    }
    /* There seems to be a bug where if the arrow disappears at the
     * Same time you hit it, it segfaults because curTarget = -1
     */
    if (curTarget != -1)
        characters[curTarget]->setTexture(TEX_MISC);
    curTarget = -1;
}

/* DO NOTHING -- game does not use the mouse clicks */
void Karaoke::mouseClick(glm::vec3 direction, glm::vec4 point) {}