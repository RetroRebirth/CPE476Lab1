#ifndef __KARAOKE__
#define __KARAOKE__

#include "includes.h"
#include "Clicks.h"
#include "Texture.h"
#include "Window.h"
#include "Sound.h"

#define MIKU  0
#define RIN   1
#define LEN   2
#define KAITO 3
#define CHARA_TEX 14


class Karaoke {
public:
    // constructor & deconstructor
    Karaoke(GLuint _ShadeProg, Sound* _sound);
    virtual ~Karaoke();
    
    // called by session
    void mouseClick(glm::vec3 direction, glm::vec4 point);
    void step(Window* window);
    
    // called by main
    void selectCharacter(int target);
    void nextSong();
    void prevSong();
    void selectSong();
    void increaseDifficulty();
    void decreaseDifficulty();
    
    // getters and setters
    bool gameOver, gameStart, songChosen;
    
private:
    // Variables
    vector<Object*> characters;
    vector<tinyobj::shape_t> shapes;
    vector<tinyobj::material_t> materials;
    GLuint ShadeProg;
    Sound* sound;
    Object* screen;
    Object* arrow;
    
    // Minigame variables
    int score;
    int curTarget, curSong, speed;
    float timeStart, timeArrow;
    float beat, bpm, songDuration;
    float arrowPos;
    
    // Minigame functions
    void checkTime(Window *window);
    void setUp();
    void addCharacter(char *file, int tex, float xPos);
    void addArrow();
};

#endif