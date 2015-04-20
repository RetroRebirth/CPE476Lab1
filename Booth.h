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

    //Checks if anything is colliding with the booth so it stops them.
    void checkCollision(Object* _otherObject);
    //Checks for if the player is within the interact radius
    void checkInteract(Player *_player);
    //Set the position for the booth
    void setPosition(glm::vec3 position);

  private:
    Object* object;
    //can't go past the bump radius
    float bumpRadius;
    //Can only interact within this radius
    float interactRadius;
    //What should be shown to screen when inside the interact radius
    //this should go away when the player leaves the interact radius
    const string* interactMessage;

    //Shows the message to the screen through whatever magic we're going
    //to use
    void showMessage();
    //Starts the minigame
    void startMinigame();
};
#endif
