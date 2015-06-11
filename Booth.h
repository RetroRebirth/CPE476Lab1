#ifndef __Booth__
#define __Booth__

#include "includes.h"
#include "Object.h"
#include "Texture.h"
#include "FontEngine.h"

class Booth {
   public:
    
    Booth(glm::vec3 _pos, glm::vec3 _scalar, float _angle, char* _minigame,
          vector<tinyobj::shape_t> shapes,
          vector<tinyobj::material_t> materials,
          GLuint ShadeProg);
    virtual ~Booth();
    
    struct bound_box bounds;
    struct bound_box influence_bounds;
    
    Object *booth[3];       // booth object
    
    //Checks if anything is colliding with the booth so it stops them. Returns true if a hit is registered with given object
    bool checkCollision(Object* _otherObject);
    //Checks to see if player is colliding with bounding box
    bool checkPlayerCollision(Object* player, glm::vec3* colPlane);
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
    
    void getCollisionAxis(glm::vec3 pos, glm::vec3* colPlane);
    const string getMinigameDisplayName();
    const string getMinigameSplash();

  private:
    // Object information
    float bumpRadius;       // can't go past this (collision)
    float interactRadius;   // can enter within this range
    bool active;            // whether or not booth can be interacted with
    
    Object* decoration;
    
    // Minigame information
    char* interactMessage;  // message shown when within interaction
    char minigame[20];         // minigame associated with this booth
    
    // Methods
    void showMessage();     // displays message to screen
    void startMinigame();   // starts the minigame
    void initBooth(glm::vec3 _pos, glm::vec3 _scalar, float _angle, char* _minigame);
};

#endif