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
    //This is what main calls for the click callback
    void mouse_click(int mouse_x, int mouse_y, int height, int width, glm::mat4 position, glm::mat4 view, glm::vec3 o);
    //Gets the direction of the last click so it can be drawn in minigames that want that kind of thing. It also clears out direction afterwards so the same line won't be drawn twice.
    glm::vec3 getDirection();
    //Switches between the sphere check and the plane check. True is for spheres and false is for planes 
    void changeCheck(bool which);

  private:
    vector<Object*>* objects;
    vector<Object*> clickedObjects;
    glm::vec3 direction;
    bool checkSphere;

    //Used to check against spheres. Give in the object and the o that is provided into mouse_click. 
    bool checkAgainstSphere(Object *object, glm::vec3 o);
    
};
#endif
