#include <math.h>
#include <fstream>
#include <map>
#include <string>
#include <vector>

#include "includes.h"
#include "Session.h"
#include "Window.h"
#include "Camera.h"
#include "Texture.h"
#include "Karaoke.h"

Session* session;

// TODO get these callback functions out of main
void window_size_callback(GLFWwindow* window, int w, int h) {
   glViewport(0, 0, (GLsizei)w, (GLsizei)h);
} 
void mouse_callback(GLFWwindow* win, double xpos, double ypos) {
   Camera* camera = session->getCamera();
   Window* window = session->getWindow();

   camera->mouse_callback(win, xpos, ypos, window->width, window->height);
}

void enter_callback(GLFWwindow* win, int entered) {
   Camera* camera = session->getCamera();
   Window* window = session->getWindow();

   camera->enter_callback(win, entered, window->width, window->height);
}

void key_callback(GLFWwindow* win, int key, int scancode, int action, int mods) {
   Camera* camera = session->getCamera();

   // If we aren't pressing any button for the first time OR holding down a button, don't do anything.
   if (action != GLFW_PRESS && action != GLFW_REPEAT) {
      return;
   }

   switch(key) {
   case GLFW_KEY_C:
      camera->debug = !camera->debug;
      if (camera->debug) {
         camera->debug_pos = camera->pos;
      }
      break;
   case GLFW_KEY_L:
      session->toggleDrawWorld();
      break;
   case GLFW_KEY_P:
      camera->pov = !camera->pov;
      break;
   case GLFW_KEY_UP:
      if (camera->speed < MAX_SPEED) {
         camera->speed += DELTA_SPEED;
      }
      if (session->getMinigame()->karaoke != NULL) {
         if (!session->getMinigame()->karaoke->songChosen)
            session->getMinigame()->karaoke->increaseDifficulty();
         else if (!session->getMinigame()->getGameOver())
            session->getMinigame()->karaoke->selectCharacter(1);
      }
      break;
   case GLFW_KEY_DOWN:
      if (camera->speed > MIN_SPEED) {
         camera->speed -= DELTA_SPEED;
      }
      if (session->getMinigame()->karaoke != NULL) {
         if (!session->getMinigame()->karaoke->songChosen)
            session->getMinigame()->karaoke->decreaseDifficulty();
         else if (!session->getMinigame()->getGameOver())
            session->getMinigame()->karaoke->selectCharacter(2);
      }
      break;
   case GLFW_KEY_LEFT:
      if (session->getMinigame()->karaoke != NULL) {
         if (!session->getMinigame()->karaoke->songChosen)
            session->getMinigame()->karaoke->prevSong();
         else if (!session->getMinigame()->getGameOver())
            session->getMinigame()->karaoke->selectCharacter(0);
      }
      break;
   case GLFW_KEY_RIGHT:
      if (session->getMinigame()->karaoke != NULL) {
         if (!session->getMinigame()->karaoke->songChosen)
            session->getMinigame()->karaoke->nextSong();
         else if (!session->getMinigame()->getGameOver())
            session->getMinigame()->karaoke->selectCharacter(3);
      }
      break;
   case GLFW_KEY_ENTER:
      // Enter the minigame if in the world
      if (session->getGameState() == WORLD_STATE) {
         session->enterMinigame();
      }
      // Start the minigame if in the minigame
      else if (session->getGameState() == MINIGAME_STATE) {
         if (!session->gameStarted()) {
            session->startMinigame();
         }
      }
      break;
   case GLFW_KEY_SPACE:
      if (session->getGameState() == MINIGAME_STATE) {
         session->leaveMinigame();
      }
      break;
   }
}

void mouse_click_callback(GLFWwindow *window, int button, int action, int mods){
   // Only read the user releasing the mouse button
   if (action == GLFW_PRESS) {
      return;
   }
   double mouse_x;
   double mouse_y;
   glfwGetCursorPos(window, &mouse_x, &mouse_y);

   int height;
   int width;
   glfwGetWindowSize(window, &width, &height);
   
   // convert window to world coordinates
   double x = 2.0 * mouse_x / width - 1;
   double y = -2.0 * mouse_y / height + 1;
   glm::mat4 Projection = session->getCamera()->Projection;
   glm::mat4 View = session->getCamera()->View;
   glm::mat4 viewProjInv = glm::inverse(Projection * View);
   glm::vec4 point = glm::vec4(x, y, 0, 1);
   glm::vec4 newPoint = viewProjInv * point;
   
   session->getClicks()->mouse_click(mouse_x, mouse_y, height, width, Projection, View, session->getCamera()->pos);
   glm::vec3 direction = session->getClicks()->getDirection();
   session->mouseClick(direction, newPoint);
}


void setInputCallbacks() {
   GLFWwindow* win = session->getWindow()->glfw_window;
   // Set input callbacks
   glfwSetWindowSizeCallback(win, window_size_callback);
   glfwSetCursorPosCallback(win, mouse_callback);
   glfwSetMouseButtonCallback(win, mouse_click_callback);
   glfwSetCursorEnterCallback(win, enter_callback);
   glfwSetKeyCallback(win, key_callback);
}

/** MAIN **/
int main(int argc, char **argv) {
   session = new Session(); // Create the main game session
   setInputCallbacks();
    
   loadAllTextures();
   session->run(); // Runs the main game loop
   delete session; // Free the main game session from the heap
    
   return 0;
}