#ifndef __Clicks__
#define __Clicks__

#include "includes.h"
#include "Object.h"

class Clicks {
  public:
    Clicks();
    virtual ~Clicks();

    //This gets the clicked objects and clears the current set of clicked objects
    vector<Object*> getClickedObjects();
    //Sets the objects for clicks to check against
    void setObjects(vector<Object*>* bob);

  private:
    vector<Object*>* objects;
    vector<Object*> clickedObjects;
};
#endif
