#include "Clicks.h"

using namespace std;

Clicks::Clicks()
{
  objects = NULL;
}

Clicks::~Clicks(){
}

//This gets the clicked objects and clears the current set of clicked objects
vector<Object*> Clicks::getClickedObjects(){
  vector<Object*> forReturning = clickedObjects;
  clickedObjects.clear();
  return forReturning;
}

//Sets the objects for clicks to check against
void Clicks::setObjects(vector<Object*>* bob){
  objects = bob;
}
