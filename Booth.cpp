#include "Booth.h"

using namespace std;

Booth::Booth(
    Object *_object,
    const string *_message){

  object = _object;
  interactMessage = _message;
  //can't go past this
  bumpRadius = 1.0f;
  //can only interact within this
  interactRadius = 4.0f;
  //What should be shown to screen when inside the interact radius
  //this should go away when the player leaves the interact radius
}

Booth::~Booth(){}

//Shows the message to the screen through whatever magic we're going
//to use
void Booth::showMessage(){
  //TODO magic to show messages on the screen without glut.
}

//Checks if anything is colliding with the booth so it stops them.
bool Booth::checkCollision(Object* _otherObject){
//TODO magic collision work
}

//Checks for if the player is within the interact radius
bool Booth::checkInteract(Player *_player){
//TODO magic collision work again
  while(true /*The actual interact check would go here*/ ){
    showMessage();
    //TODO check for whatever interaction
    //if(interaction is used)
    //startMinigame();
  }
}

//Starts the minigame
void Booth::startMinigame(){
  //Need minigame things
}

//Sets the position for the booth
void Booth::setPosition(glm::vec3 position){
  object->pos = position;

}
