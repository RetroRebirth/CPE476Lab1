#ifndef __Booth__
#define __Booth__

#include "includes.h"
#include "Object.h"
#include "Texture.h"

class Booth {
   public:
    
    Booth(string data,
          vector<tinyobj::shape_t> shapes,
          vector<tinyobj::material_t> materials,
          GLuint ShadeProg);
    virtual ~Booth();
    
    struct bound_box bounds;
    struct bound_box influence_bounds;
    
    //Checks if anything is colliding with the booth so it stops them. Returns true if a hit is registered with given object
    bool checkCollision(Object* _otherObject);
    //Checks to see if camera is colliding with bounding box
    bool checkCameraCollision(glm::vec3 cam_pos, glm::vec3 *colPlane);
    //Checks for if the player is within the interact radius. Returns true if hit is registered within interaction box
    bool checkInteract(glm::vec3 player_pos);
    //Check to see if booth is active (has been interacted with)
    bool isActive() { return active; }
    //Set the position for the booth
    void setPosition(glm::vec3 position);
    //Calculate the bounding box for the booth... should only be called once after first draw
    void calculateBoundingBox();
    // Return the minigame associated with this booth
    char* getMinigame();
    // Draws the booth
    void draw();
    // Get the booths
    Object **getBooths() { return booth; }

  private:
    // Object information
    Object *booth[3];       // booth object
    float bumpRadius;       // can't go past this (collision)
    float interactRadius;   // can enter within this range
    bool active;            // whether or not booth can be interacted with
    
    // Minigame information
    char* interactMessage;  // message shown when within interaction
    char* minigame;         // minigame associated with this booth
    
    // Methods
    void showMessage();     // displays message to screen
    void startMinigame();   // starts the minigame
    void initBooth(string data);
};

#endif