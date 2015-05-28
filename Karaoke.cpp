#include "Karaoke.h"

Karaoke::Karaoke(GLuint _ShadeProg, Sound* _sound) {
    
    // Inititalize the game
    ShadeProg = _ShadeProg;
    sound = _sound;
    gameOver = gameStart = songChosen = false;
    timeStart = timeArrow = 0.0;
    score = curSong = numGood = numBad = numPerfect = 0;
    curTarget = -1;
    speed = 1;
    
    // Set up the booth
    setUp();
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
    chara->rotate(180.0f, glm::vec3(0.0, 1.0, 0.0));
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

void Karaoke::initVideo() {
    // Create frames for the video
    cap = VideoCapture(sound->getSongInfo(curSong).vid_file);
    if (cap.isOpened()) {
        // Get the fps
        fps = cap.get(CV_CAP_PROP_FPS);
    }
}

void Karaoke::drawVideo(Window* window) {
    // Set the latest frame based off the time
    double curFrame = (window->time - timeStart) * fps;
    cap.set(CV_CAP_PROP_POS_FRAMES, curFrame);
    
    // Read a new frame
    Mat frame;
    bool success = cap.read(frame);
    
    // Make a new texture for the frame and draw it on the screen
    if (success) {
        Texture newTex;
        newTex.loadTexture(frame, 100);
        screen->setTexture(100);
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
                for (int i = 0; i < characters.size(); i++)
                    characters[i]->setTexture(TEX_MISC);
                screen->scale(glm::vec3(8.0, 11.0, 8.0));
                screen->rotate(180.0f, glm::vec3(1.0, 0.0, 0.0));
                gameStart = true;
                timeStart = timeFrame = window->time;
            }
            else if (window->time - timeStart >= 3.0 && characters.size() < 4)
                addCharacter((char *)"objs/kaito.obj", TEX_KAITO, -2.0);
            else if (window->time - timeStart >= 2.0 && characters.size() < 3)
                addCharacter((char *)"objs/len.obj", TEX_LEN, -0.7);
            else if (window->time - timeStart >= 1.0 && characters.size() < 2)
                addCharacter((char *)"objs/rin.obj", TEX_RIN, 0.7);
            else if (window->time - timeStart >= 0.0 && characters.size() < 1)
                addCharacter((char *)"objs/miku.obj", TEX_MIKU, 2.0);
        }
        else {
            // Check whether the game has ended
            if (window->time - timeStart >= songDuration)
                gameOver = true;
            // Add a new arrow
            else if (curTarget == -1) {
                addArrow();
                timeArrow = window->time;
            }
            // Arrow time is up; remove the arrow
            else if (window->time - timeArrow >= beat) {
                sound->playBuzzerSound();
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

void Karaoke::printInstructions() {
   ifstream instrFile;
   instrFile.open("krkinstr.txt");
   string line;
   float yPos = .5;
   float yInc;
   
   fontEngine->useFont("amatic", 30);
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
      printf("file 'wminstr.txt' was not available or could not be opened\n");
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
        fontEngine->useFont("amatic", 48);
        fontEngine->display(glm::vec4(1.0, 1.0, 1.0, 1.0), msg1, 0-fontEngine->getTextWidth(msg1)/2.0, -0.48);
        
        // Display the song name
        char msg2[30], lock[6];
        if (!sound->getSongInfo(curSong).unlocked)
            sprintf(lock, "(%d)", sound->getSongInfo(curSong).price);
        else
            sprintf(lock, " ");
        sprintf(msg2, "%s %s", sound->getSongInfo(curSong).song_name, lock);
        fontEngine->display(glm::vec4(1.0, 1.0, 1.0, 1.0), msg2, 0-fontEngine->getTextWidth(msg2)/2.0, 0.53);
        
        printInstructions();
        
        // Don't do anything; wait for the song to start
        return;
    }
    // Game has ended; display the score
    if (gameOver) {
        /* DISPLAY GAME OVER INFO HERE: */
        // FINISH!
        // ------------
        // [Song Name]
        // Your score is: score
        //
        // PERFECTS = numPerfect
        // GOODS = numGood
        // BADS = numBad
        // Percent = 100 * ((numGood + numPerfect) / (numGood + numPerfect + numBad))
        return;
    }
    checkTime(window);
    
    // Draw the video
    if (window->time - timeFrame >= (1 / fps) * speed) {
        drawVideo(window);
        timeFrame = window->time;
    }
    
    // Draw the scene
    for (int i = 0; i < characters.size(); i++)
        characters[i]->draw();
    if (!gameStart)
        return;
    if (curTarget != -1)
        arrow->draw();
    
    // Check for game over
    if (score < -10 * speed) {
        gameOver = true;
    }
    
    // Display the score
    char scrStr[20];
    sprintf(scrStr, "Score: %d", score);
    fontEngine->useFont("amatic", 40);
    fontEngine->display(glm::vec4(1.0, 1.0, 1.0, 1.0), scrStr, 1-fontEngine->getTextWidth(scrStr)-.07, .9);
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
    initVideo();
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
            //sound->playContactSound();
            score += speed * 2;
            numPerfect++;
        }
        // Otherwise, "GOOD" score
        else {
            //sound->playJumpSound();
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
    
    // Make the next arrow
    if (curTarget != -1)
        characters[curTarget]->setTexture(TEX_MISC);
    curTarget = -1;
}

/* DO NOTHING -- game does not use the mouse clicks */
void Karaoke::mouseClick(glm::vec3 direction, glm::vec4 point) {}
