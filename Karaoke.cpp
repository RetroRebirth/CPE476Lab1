#include "Karaoke.h"
#include "ParticleSorter.h"

#define DEBUGGING_BLOOM 0

bool removeFireworks(vector<Particle*> p) {
    if (p[0]->cycles > 0) {
        return true;
    }
    return false;
}

Karaoke::Karaoke(GLuint _ShadeProg, Sound* _sound, Camera* _camera, Program* _particleProg) {
    // Inititalize the game
    ShadeProg = _ShadeProg;
    sound = _sound;
    camera = _camera;
    particleProg = _particleProg;
    gameOver = false;
    gameStart = false;
    songChosen = false;
    timeStart = timeArrow = 0.0;
    score = 0;
    curSong = 0;
    numGood = 0;
    numBad = 0;
    numPerfect = 0;
    curTarget = -1;
    speed = 1;
    messageTime = 0.0;
    
    t = 0.0f;
    t0_disp = 0.0f;
    t_disp = 0.0f;
    h = 1.0f;
    g = glm::vec3(0.0f, -0.01f, 0.0f);
    fireworks.clear();
    numFireworks = 0;
    
    charaPos[MIKU] = MIKU_POS;
    charaPos[RIN] = RIN_POS;
    charaPos[LEN] = LEN_POS;
    charaPos[KAITO] = KAITO_POS;
    sound->playKaraokeMusic(curSong);
    
    // Set up the booth
    setUp();
}

Karaoke::~Karaoke() {
    for (int i = 0; i < characters.size(); i++)
        delete characters[i];
    delete arrow;
    delete screen;
    
    sound->playBackgroundMusic();
}

void Karaoke::setUp() {
    // Create a "screen" in the back for projecting the video
    screen = new Object(shapes, materials, ShadeProg);
    screen->load((char *)"objs/screen.obj");
    screen->setTexture(textures[curSong + TEX_SONGS]);
    screen->setPos(glm::vec3(0.0, 2.8, 2.0));
    screen->scale(glm::vec3(5.0, 7.0, 5.0));
    screen->setShadows(false, 0.0, 0.0);
    
    // Create the arrow
    arrow = new Object(shapes, materials, ShadeProg);
    arrow->load((char *)"objs/arrow.obj");
    arrow->setTexture(textures[curTarget + CHARA_TEX]);
    arrow->scale(glm::vec3(1.0, 1.0, 1.0));
    arrow->setPos(glm::vec3(2.0, 3.5, 1.0));
    arrow->setShadows(false, 0.0, 0.0);
    
    // Load countdown textures beforehand for performance
    Texture countdownTex;
    countdownTex.loadTexture((char *)"textures/countdown5.bmp", 105, true);
    countdownTex.loadTexture((char *)"textures/countdown4.bmp", 104, true);
    countdownTex.loadTexture((char *)"textures/countdown3.bmp", 103, true);
    countdownTex.loadTexture((char *)"textures/countdown2.bmp", 102, true);
    countdownTex.loadTexture((char *)"textures/countdown1.bmp", 101, true);
}

void Karaoke::addCharacter(char *file, int tex, float xPos) {
    Object *chara = new Object(shapes, materials, ShadeProg);
    chara->load(file);
    chara->setTexture(textures[tex]);
    chara->setPos(glm::vec3(xPos, 1.5, 1.0));
    chara->scale(glm::vec3(1.0, 1.0, 1.0));
    chara->rotate(180.0f, glm::vec3(0.0, 1.0, 0.0));
    chara->setShadows(false, 0.0, 0.0);
    chara->updateRadius();
    characters.push_back(chara);
}

void Karaoke::addArrow() {
    // Choose a target position
    curTarget = Util::randF() * 4;
    characters[curTarget]->setTexture(textures[curTarget + CHARA_TEX]);
    arrow->translate(glm::vec3(0.0, 0.0, 0.0));
    
    // Decide where to place the arrow
    if (curTarget == MIKU) {
        arrow->setTexture(textures[curTarget + CHARA_TEX]);
        arrow->setPos(MIKU_POS);
        arrow->rotate(90.0f, glm::vec3(0.0, 0.0, 1.0));
    }
    else if (curTarget == RIN) {
        arrow->setTexture(textures[curTarget + CHARA_TEX]);
        arrow->setPos(RIN_POS);
        arrow->rotate(-180.0f, glm::vec3(1.0, 0.0, 0.0));
    }
    else if (curTarget == LEN) {
        arrow->setTexture(textures[curTarget + CHARA_TEX]);
        arrow->setPos(LEN_POS);
        arrow->rotate(0.0f, glm::vec3(1.0, 0.0, 0.0));
    }
    else if (curTarget == KAITO) {
        arrow->setTexture(textures[curTarget + CHARA_TEX]);
        arrow->setPos(KAITO_POS);
        arrow->rotate(-90.0f, glm::vec3(0.0, 0.0, 1.0));
    }
    arrowPos = 0.0;
}

void Karaoke::initVideo() {
    // Create frames for the video
    cap = VideoCapture(sound->getSongInfo(curSong).vid_file);
    
    if (cap.isOpened()) {
        // Get the fps
        fps = cap.get(CV_CAP_PROP_FPS);
        // set video "frame speed"
        frameStep = (1 / fps) * speed;
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
        screen->setTexture(textures[texture_id]);
    }
}

void Karaoke::drawArrow() {
    GLint texLoc = GLSL::getUniformLocation(ShadeProg, "uSampler1");
    glUniform1i(texLoc, 1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, arrow->getTexture());
    
    // Draw arrow with bloom
    float bloom = (arrowPos * -1) / 2.0;
    int blend = (arrowPos <= -1.6) ? 1 : 1;
    glUniform1f(GLSL::getUniformLocation(ShadeProg, "BloomAmount"), bloom);
    glUniform1i(GLSL::getUniformLocation(ShadeProg, "BlendMode"), blend);
    arrow->scale(glm::vec3(1.0, 1.0, 1.0));
    arrow->draw();
    
    // Turn bloom off
    glUniform1i(GLSL::getUniformLocation(ShadeProg, "BlendMode"), 0);
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

void Karaoke::printScore() {
    float yPos = .5;
    float yInc;
    float percent;
    
    // Display game over message
    char ln1[15];
    if (score <= -10 * speed)
        sprintf(ln1, "LOSE...");
    else if (numBad == 0)
        sprintf(ln1, "PERFECT!!!");
    else
        sprintf(ln1, "FINISH!");
    fontEngine->useFont("amatic", 52);
    fontEngine->display(glm::vec4(1.0, 1.0, 1.0, 1.0), ln1, 0-fontEngine->getTextWidth(ln1)/2.0, yPos);
    yInc = fontEngine->getTextHeight(ln1) * 1.3;
    yPos -= (yInc * 2);
    
    // [Song Name]
    char ln2[40];
    sprintf(ln2, "%s", sound->getSongInfo(curSong).song_name);
    fontEngine->display(glm::vec4(1.0, 1.0, 1.0, 1.0), ln2, 0-fontEngine->getTextWidth(ln2)/2.0, yPos);
    yPos -= yInc;
    // Your score is: score
    char ln3[30];
    sprintf(ln3, "Your score is: %d", score);
    fontEngine->display(glm::vec4(1.0, 1.0, 1.0, 1.0), ln3, 0-fontEngine->getTextWidth(ln3)/2.0, yPos);
    yPos -= (yInc * 2);
    
    // PERFECTS = numPerfect GOODS = numGood BADS = numBad
    char ln4[50];
    sprintf(ln4, "PERFECTS: %d     GOODS: %d     BADS: %d", numPerfect, numGood, numBad);
    fontEngine->display(glm::vec4(1.0, 1.0, 1.0, 1.0), ln4, 0-fontEngine->getTextWidth(ln4)/2.0, yPos);
    yPos -= (yInc * 2);
    
    // Percent = 100 * ((numGood + numPerfect) / (numGood + numPerfect + numBad))
    percent = 100 * (((numGood + numPerfect) * 1.0) / ((numGood + numPerfect + numBad) * 1.0));
    char ln5[20];
    sprintf(ln5, "FINAL GRADE: %d", (int)percent);
    fontEngine->useFont("capture", 70);
    fontEngine->display(glm::vec4(1.0, 1.0, 1.0, 1.0), ln5, 0-fontEngine->getTextWidth(ln5)/2.0, yPos);
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
            sound->setVolume(1.0 - (window->time - timeStart) / 5.0);
            if (window->time - timeStart >= 5.0) {
                bpm = sound->playKaraokeMusic(curSong);
                songDuration = sound->getSongDuration() / 1000.0;
                beat = songDuration / ((songDuration / 60.0) * bpm);
                beat *= 3 / speed;
                
                totsec = ((sound->getSongDuration() + 500) / 1000);
                mins = totsec / 60;
                secs = totsec % 60;
                
                for (int i = 0; i < characters.size(); i++)
                    characters[i]->setTexture(textures[TEX_MISC]);
                screen->scale(glm::vec3(8.0, 11.0, 8.0));
                screen->rotate(180.0f, glm::vec3(1.0, 0.0, 0.0));
                screen->setTexture(textures[0]);
                gameStart = true;
                timeStart = timeFrame = window->time;
                texture_id = 100;
                sound->setVolume(1.0);
            }
            else if (window->time - timeStart >= 4.0 && characters.size() < 4) {
                addCharacter((char *)"objs/kaito.obj", TEX_KAITO, -2.0);
                texture_id = 101;
            }
            else if (window->time - timeStart >= 3.0 && characters.size() < 3) {
                addCharacter((char *)"objs/len.obj", TEX_LEN, -0.7);
                texture_id = 102;
            }
            else if (window->time - timeStart >= 2.0 && characters.size() < 2) {
                addCharacter((char *)"objs/rin.obj", TEX_RIN, 0.7);
                texture_id = 103;
            }
            else if (window->time - timeStart >= 1.0 && characters.size() < 1) {
                addCharacter((char *)"objs/miku.obj", TEX_MIKU, 2.0);
                texture_id = 104;
            }
            screen->setTexture(textures[texture_id]);
        }
        else {
            // Check whether the game has ended
            etotsec = window->time - timeStart;
            emins = etotsec / 60;
            esecs = etotsec % 60;
            
            messageTime -= window->dt;
            
            if (etotsec >= songDuration) {
                // Bonus points for all perfects
                if (numGood == 0 && numBad == 0)
                    score += 250 * (speed - 1);
                // Bonus points for no missed arrows
                if (numBad == 0)
                    score += 100 * (speed - 1);
                gameOver = true;
                global_points += score;
                sound->pauseSong();
            }
            // Add a new arrow
            else if (curTarget == -1) {
                addArrow();
                timeArrow = window->time;
            }
            // Arrow time is up; remove the arrow
            else if (window->time - timeArrow >= beat) {
                sound->playBuzzerSound();
                if (curTarget != -1)
                    characters[curTarget]->setTexture(textures[TEX_MISC]);
                score -= speed;
                numBad++;
                curTarget = -1;
                
                messageTime = 1.0;
                message = BAD_STR;
                messageColor = glm::vec3(1.0, 0.0, 0.0);
            }
            // Move the arrow down the screen
            else {
                arrowPos = (-2.0 / beat) * (window->time - timeArrow);
                arrow->translate(glm::vec3(0.0, arrowPos, 0.0));
            }
        }
    }
}

void Karaoke::particleStep() {
    // Display every 60 Hz
	   t += h;
    
    // Create matrix stacks
	   MatrixStack P, MV;
	   // Apply camera transforms
	   P.pushMatrix();
	   camera->applyProjectionMatrix(&P);
	   MV.pushMatrix();
	   camera->applyViewMatrix(&MV);
    
	   // Bind the program
	   particleProg->bind();
	   ParticleSorter sorter;
    
	   glUniformMatrix4fv(particleProg->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P.topMatrix()));
	   
	   for(int i=0; i<fireworks.size(); ++i) {
           // sort the explosions' Particles from back to front
           MatrixStack temp;
           camera->applyViewMatrix(&temp);
           glm::mat4 V = temp.topMatrix();
           
           sorter.C = glm::transpose(glm::inverse(V)); // glm is transposed!
           std::sort(fireworks[i].begin(), fireworks[i].end(), sorter);
           
           for (int j=0; j<fireworks[i].size(); ++j) {
               fireworks[i][j]->update(t, h, g);
               fireworks[i][j]->draw(&MV);
           }
       }
	   fireworks.erase(std::remove_if(fireworks.begin(),
                                      fireworks.end(),
                                      &removeFireworks),
                       fireworks.end());
	   // Unbind the program
	   particleProg->unbind();
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
        char msg2[40], lock[10];
        if (!sound->getSongInfo(curSong).unlocked)
            sprintf(lock, "(%d)", sound->getSongInfo(curSong).price);
        else
            sprintf(lock, " ");
        sprintf(msg2, "%s %s", sound->getSongInfo(curSong).song_name, lock);
        fontEngine->display(glm::vec4(1.0, 1.0, 1.0, 1.0), msg2, 0-fontEngine->getTextWidth(msg2)/2.0, 0.53);
        
        printInstructions();
        return;
    }
    // Game has ended; display the score
    if (gameOver) {
        printScore();
        return;
    }
    checkTime(window);
    
    // Draw the video
    if (window->time - timeFrame >= frameStep) {
        drawVideo(window);
        timeFrame = window->time;
    }
    // Draw the scene
    for (int i = 0; i < characters.size(); i++)
        characters[i]->draw();
    if (!gameStart)
        return;
    if (curTarget != -1)
        drawArrow();
    
    // Check for game over
    if (score < -10 * speed) {
        gameOver = true;
        sound->pauseSong();
    }
    textStep();
    particleStep();
}

void Karaoke::textStep() {
    float yPos = .9;
    float yInc = .1;
    float alpha = 0.0;
    
    // Display the score
    char time[60];
    sprintf(time, "Time remaining: %d:%02d / %d:%02d", emins, esecs, mins, secs);
    fontEngine->useFont("amatic", 40);
    fontEngine->display(glm::vec4(1.0, 1.0, 1.0, 1.0), time, 0-fontEngine->getTextWidth(time)/2.0, yPos);
    yInc = fontEngine->getTextHeight(time) * 1.3;
    
    char scrStr[20];
    sprintf(scrStr, "Score: %d", score);
    fontEngine->display(glm::vec4(1.0, 1.0, 1.0, 1.0), scrStr, 1-fontEngine->getTextWidth(scrStr)-.07, yPos);
    
    char bpmStr[20];
    sprintf(bpmStr, "BPM: %.2lf", bpm);
    fontEngine->display(glm::vec4(1.0, 1.0, 1.0, 1.0), bpmStr, 1-fontEngine->getTextWidth(bpmStr)-.07, yPos-yInc);
    
    fontEngine->useFont("oswald", 75);
    
    if (messageTime > 0.0) {
        yPos = .3 + (abs(messageTime - 1.0)/2.0);
        alpha = 1.0 - (abs(messageTime - 1.0)*2.0);
        fontEngine->display(glm::vec4(messageColor, alpha), message, 0-fontEngine->getTextWidth(message)/2.0, yPos);
    }
}

// Chooses a song on song selection menu
void Karaoke::nextSong() {
    if (songChosen)
        return;
    curSong = (curSong + 1) % sound->getSongs().size();
    screen->setTexture(textures[curSong + TEX_SONGS]);
    sound->playKaraokeMusic(curSong);
}
void Karaoke::prevSong() {
    if (songChosen)
        return;
    curSong = curSong - 1;
    if (curSong == -1)
        curSong = sound->getSongs().size() - 1;
    screen->setTexture(textures[curSong + TEX_SONGS]);
    sound->playKaraokeMusic(curSong);
}
void Karaoke::selectSong() {
    if (!sound->getSongInfo(curSong).unlocked) {
        sound->playIncorrectSound();
        return;
    }
    if (songChosen)
        return;
    sound->playContactSound();
    
    initVideo();
    // Decide how many fireworks to make
    if (speed == 3)
        numParticles = 5;
    else
        numParticles = 10;
    texture_id = 105;
    screen->setTexture(textures[texture_id]);

    songChosen = true;
}

// Changes the difficulty setting
void Karaoke::increaseDifficulty() {
    if (speed < 3) {
        speed++;
        sound->playThwackSound();
    }
}
void Karaoke::decreaseDifficulty() {
    if (speed > 1) {
        speed--;
        sound->playThwackSound();
    }
}

void Karaoke::addNewFirework(int target) {
    glm::vec3 pos = charaPos[target];
    vector<Particle*> firework;
    firework.clear();
    for (int i = 0; i < numParticles; ++i) {
        Particle* particle = new Particle();
        particle->load();
        particle->setTexture(textures[TEX_PARTICLE]);
        particle->setStartPos(pos);
        particle->setStartVel(glm::vec3(randFloat(-0.1f, 0.1f), randFloat(-0.1f, 0.0f), randFloat(-0.1f, 0.1f)));
        particle->setStartCol(glm::vec3(randFloat(0.0f, 1.0f), randFloat(0.0f, 1.0f), randFloat(0.0f, 1.0f)));
        particle->setStartTTL(40.0f);
        particle->setOneCycle(true);
        particle->setOpacityTaper(true);
        particle->init(particleProg);
        firework.push_back(particle);
    }
    fireworks.push_back(firework);
    
    numFireworks++;
}

void Karaoke::selectCharacter(int target) {
    // Hit the right arrow
    if (target == curTarget) {
        // Check whether a "PERFECT" score was earned
        if (arrowPos <= -1.6) {
            addNewFirework(target);
            score += speed * 2;
            numPerfect++;
            
            messageTime = 1.0;
            message = PERF_STR;
            messageColor = glm::vec3(1.0, 1.0, 0.0);
        }
        // Otherwise, "GOOD" score
        else {
            score += speed;
            numGood++;
            
            messageTime = 1.0;
            message = GOOD_STR;
            messageColor = glm::vec3(0.0, 0.0, 1.0);
        }
    }
    // "BAD" score; wrong arrow
    else {
        sound->playBuzzerSound();
        score -= speed;
        numBad++;
        
        messageTime = 1.0;
        message = BAD_STR;
        messageColor = glm::vec3(1.0, 0.0, 0.0);
    }
    
    // Make the next arrow
    if (curTarget != -1)
        characters[curTarget]->setTexture(textures[TEX_MISC]);
    curTarget = -1;
}