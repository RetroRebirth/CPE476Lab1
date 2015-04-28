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
      camera->bounded = !camera->bounded;
      break;
   case GLFW_KEY_P:
      camera->pov = !camera->pov;
      break;
   case GLFW_KEY_UP:
      if (camera->speed < MAX_SPEED) {
         camera->speed += DELTA_SPEED;
      }
      break;
   case GLFW_KEY_DOWN:
      if (camera->speed > MIN_SPEED) {
         camera->speed -= DELTA_SPEED;
      }
      break;
   case GLFW_KEY_ENTER:
      if (session->getGameState() == WORLD_STATE) {
         session->enterMinigame();
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
  double mouse_x;
  double mouse_y;
  glfwGetCursorPos(window, &mouse_x, &mouse_y);

  int height;
  int width;
  glfwGetWindowSize(window, &width, &height);
  
  glm::vec3 direction = Util::mouse_click(mouse_x, mouse_y, height, width, session->getCamera()->Projection, 
      session->getCamera()->View, session->getCamera()->pos);
  session->mouseClick(direction);
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
