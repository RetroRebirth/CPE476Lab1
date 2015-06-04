#ifndef __KARAOKE__
#define __KARAOKE__

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <algorithm>
#include <functional>

#include "includes.h"
#include "Clicks.h"
#include "Texture.h"
#include "Window.h"
#include "Sound.h"
#include "FontEngine.h"
#include "Program.h"
#include "Particle.h"
#include "Camera.h"

#define MIKU  0
#define MIKU_POS glm::vec3(2.0, 3.5, 1.0)
#define RIN   1
#define RIN_POS glm::vec3(0.7, 3.5, 1.0)
#define LEN   2
#define LEN_POS glm::vec3(-0.7, 3.5, 1.0)
#define KAITO 3
#define KAITO_POS glm::vec3(-2.0, 3.5, 1.0)
#define CHARA_TEX 14
#define PERF_STR "Perfect!!!"
#define GOOD_STR "Good!"
#define BAD_STR "Bad..."

using namespace cv;

class Karaoke {
public:
    // constructor & deconstructor
    Karaoke(GLuint _ShadeProg, Sound* _sound, Camera* _camera, Program* _particleProg);
    virtual ~Karaoke();
    
    // called by session
    void step(Window* window);
    
    // called by main (uses keyboard input)
    void selectCharacter(int target);
    void nextSong();
    void prevSong();
    void increaseDifficulty();
    void decreaseDifficulty();
    
    // called by minigame
    void selectSong();
    void addNewFirework(int target);
    void particleStep();
    
    // getters and setters
    bool gameOver, gameStart, songChosen;
    
private:
    // Variables
    vector<Object*> characters;
    vector<tinyobj::shape_t> shapes;
    vector<tinyobj::material_t> materials;
    GLuint ShadeProg;
    Camera* camera;
    Program* particleProg;
    Object *screen, *arrow;
    Sound* sound;
    
    // Paticles
    vector<vector<Particle*> > fireworks;
    int numFireworks, numParticles;
    // time info for particles
    float t, t0_disp, t_disp;
    float h;
    glm::vec3 g;

    // Minigame variables
    int score, curTarget, curSong, speed;
    int totsec, mins, secs, etotsec, emins, esecs;
    int numGood, numBad, numPerfect;
    float timeStart, beat, bpm, songDuration;
    float perfTime, goodTime, badTime;
    
    // Animation variables
    float timeArrow, arrowPos;
    
    // Video variables
    VideoCapture cap;
    float fps;
    float timeFrame, frameStep;
    int texture_id;
    
    // Minigame functions
    void checkTime(Window *window);
    void setUp();
    void addCharacter(char *file, int tex, float xPos);
    void addArrow();
    void drawArrow();
    
    // Video functions
    void initVideo();
    void drawVideo(Window *window);
    
    // Text functions
    void printInstructions();
    void printScore();
    void textStep();
};

#endif
