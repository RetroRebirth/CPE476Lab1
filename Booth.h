#ifndef __Booth__
#define __Booth__

#include "includes.h"
#include "Object.h"
#include "Player.h"

class Booth {
  public:
    
    Booth(
	   Object *_object,
	   const string *_message);
    virtual ~Booth();
    
    Object* object;
    //bounding box for hit detection
    struct bound_box bounds;
    struct bound_box influence_bounds;

    //Checks if anything is colliding with the booth so it stops them. Returns true if a hit is registered with given object
    bool checkCollision(Object* _otherObject);
    //Checks to see if camera is colliding with bounding box
    bool checkCameraCollision(glm::vec3 cam_pos, glm::vec3 *colPlane);
    //Checks for if the player is within the interact radius. Returns true if hit is registered within interaction box
    bool checkInteract(glm::vec3 player_pos);
    //Set the position for the booth
    void setPosition(glm::vec3 position);
    //Calculate the bounding box for the booth... should only be called once after first draw
    void calculateBoundingBox();
    //Set the booth/structure type. Supported parameters are defined in include.h
    void setType(int t) { type = t; };
    //Get the booth/structure type. Defined in include.h
    int getType() { return type; };

  private:
    //type of booth/structure - default type is a wall
    int type;
    //can't go past the bump radius
    float bumpRadius;
    //Can only interact within this radius
    float interactRadius;
    //What should be shown to screen when inside the interact radius
    //this should go away when the player leaves the interact radius
    const string* interactMessage;
    //Boolean describing if the booth is able to be interacted with.
    bool active;

    //Shows the message to the screen through whatever magic we're going
    //to use
    void showMessage();
    //Starts the minigame
    void startMinigame();
};
#endif
