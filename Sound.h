#ifndef __Sound__
#define __Sound__

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "includes.h"
#include "Texture.h"

using namespace cv;
using namespace irrklang;

class Song {
public:
    char song_name[50]; // name of the song
    char song_file[50]; // name of the .wav sound file
    char vid_file[50];  // name of the .avi video file
    float bpm;          // # of beats per minute
    int price;          // price of the song in the shop
    bool unlocked;      // whether or not the song can be played
};

class Sound {
public:
    Sound();
    virtual ~Sound();
    
    ISoundEngine* engine;
    
    void initSound();
    void playBackgroundMusic();
    float playKaraokeMusic(int song);
    void pauseSong();
    
    int getSongDuration();
    Song getSongInfo(int song) { return karaoke_songs[song]; }
    vector<Song> getSongs()    { return karaoke_songs;       }
    void unlockSong(int song)  { karaoke_songs[song].unlocked = true; }

    void playContactSound();
    void playBuzzerSound();
    void playThwackSound();
    void playSplatSound();
    void playJumpSound();
    void playCorrectSound();
    void playIncorrectSound();
    void playSoftDing();
    void playLoudDing();
    
private:
    vector<Song> karaoke_songs;
    ISound* curSound;
};

#endif