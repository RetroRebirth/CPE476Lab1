#include "Camera.h"

using namespace std;

Camera::Camera(
      GLint _h_uP,
      GLint _h_uV,
      GLint _h_uView) {
   // Default attribute values
   pos = glm::vec3(0, 1, 0);
   theta = -M_PI/2.0;
   phi = 0.0;
   bounded = true;
   speed = INITIAL_SPEED;
   blocked = false;
   pov = true;
   playingMinigame = false;
   radius = 1.0;
   playerYrot = 0.0;

   // Defined attribute values
   h_uP = _h_uP;
   h_uV = _h_uV;
   h_uView = _h_uView;
}

Camera::~Camera() {}

inline void Camera::safe_glUniformMatrix4fv(const GLint handle, const GLfloat data[]) {
   if (handle >= 0)
      glUniformMatrix4fv(handle, 1, GL_FALSE, data);
}

float Camera::getYRot() {
   return Util::radiansToDegrees(theta);
}

float Camera::getXRot() {
   return Util::radiansToDegrees(phi);
}

glm::vec3 Camera::lookAtPt() {
   glm::vec3 lookAtPt = glm::vec3(cos(phi)*cos(theta), sin(phi), cos(phi)*cos((M_PI/2)-theta));
   lookAtPt += pos;
   return lookAtPt;
}

void Camera::setProjectionMatrix(int g_width, int g_height) {
   Projection = glm::perspective(90.0f, (float)g_width/g_height, 0.1f, 100.f);
   safe_glUniformMatrix4fv(h_uP, glm::value_ptr(Projection));
}

void Camera::setView() {
   glm::mat4 lookAtMat = glm::lookAt(lookAtPt(), pos, glm::vec3(0, 1, 0));

   //mult view by phi rotation matrix
   View = glm::rotate(glm::mat4(1.0f), phi, glm::vec3(1, 0, 0)) * lookAtMat;

   safe_glUniformMatrix4fv(h_uV, glm::value_ptr(View));
   glUniform3f(h_uView, pos.x, pos.y, pos.z);
}

void Camera::step(Window* window) {
   setProjectionMatrix(window->width, window->height);
   setView();

   if (!playingMinigame && (!bounded || !blocked)) {
      pos = calcNewPos(window);
   }
   // pos = !bounded || !blocked ? calcNewPos(window) : pos;
   blocked = false;
}

glm::vec3 Camera::calcNewPos(Window* window) {
   glm::vec3 newPos = pos;
   float moveInc = speed * window->dt;
   float playerYrad = Util::degreesToRadians(playerYrot);

   glm::vec3 viewVector = glm::normalize(newPos - lookAtPt());
   glm::vec3 strafeVector = glm::normalize(glm::cross(viewVector, glm::vec3(0, 1, 0)));
   glm::vec3 crossVector = glm::normalize(glm::cross(viewVector, strafeVector));
   // Scale vectors
   viewVector *= moveInc;
   strafeVector *= moveInc;
   crossVector *= moveInc;

   GLFWwindow* win = window->glfw_window;
   if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS) { // Move forward
      newPos.x += moveInc * sin(playerYrad);
      newPos.z += moveInc * cos(playerYrad);
   }
   if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS) { // Strafe left
      playerYrot += PLAYER_ROT_DEG;
   }
   if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS) { // Strafe right
      playerYrot -= PLAYER_ROT_DEG;
   }
   // for debugging purposes
   if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS) { // Move backward
      newPos -= viewVector;
   }
   if (glfwGetKey(win, GLFW_KEY_Q) == GLFW_PRESS) { // Move up
      newPos -= crossVector;
   }
   if (glfwGetKey(win, GLFW_KEY_E) == GLFW_PRESS) { // Move down
      newPos += crossVector;
   }
   
   // this is what we had originally with strafing
   // using the above code we can move and see the front of our char/potato
   /*
   if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS) // Move forward
      newPos -= viewVector;
   if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS) // Move backward
      newPos += viewVector;
   if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS) // Strafe left
      newPos += strafeVector;
   if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS) // Strafe right
      newPos -= strafeVector;
   if (glfwGetKey(win, GLFW_KEY_Q) == GLFW_PRESS) // Move up
      newPos -= crossVector;
   if (glfwGetKey(win, GLFW_KEY_E) == GLFW_PRESS) // Move down
      newPos += crossVector;*/


   // Bounding
  if (bounded) {
     float s = SIZE - 0.2f;
     if (newPos.x < -s)
        newPos.x = -s;
     if (newPos.x > s)
        newPos.x = s;
     if (newPos.z < -s)
        newPos.z = -s;
     if (newPos.z > s)
        newPos.z = s;

     newPos.y = 1;
  }

   if (player != NULL) {
      player->setPos(calculatePlayerPos());
      player->scale(glm::vec3(1.0, 2.0, 1.0));
      player->rotate(playerYrot, glm::vec3(0, 1, 0));
      
      if (pov) 
         player->draw();
   }

   return newPos;
}

void Camera::mouse_callback(GLFWwindow* window, double xpos, double ypos, int g_width, int g_height) {
   // Update theta (x angle) and phi (y angle)
   float half_width = g_width / 2.0;
   float half_height = g_height / 2.0;
   float xPosFromCenter = xpos - half_width;
   float yPosFromCenter = ypos - half_height;
   float xMag = xPosFromCenter / half_width;
   float yMag = yPosFromCenter / half_height;

   theta += MOUSE_SPEED*M_PI*xMag;
   // Bound phi to 80 degrees
   float newPhi = phi - MOUSE_SPEED*M_PI*yMag/2.0;
   //bounded between 80 and -40 to keep from going into the char
   if (glm::degrees(newPhi) < 80 && glm::degrees(newPhi) > -40) {
      phi = newPhi;
   }

   // Keep mouse in center
   glfwSetCursorPos(window, g_width/2, g_height/2);
}

void Camera::enter_callback(GLFWwindow* window, int entered, int g_width, int g_height) {
   // Position mouse at center if enter screen
   glfwSetCursorPos(window, g_width/2, g_height/2);
}

void Camera::moveToMinigame() {
   // Set the appropriate flags
   this->playingMinigame = true;
   this->pov = false;
}

void Camera::moveToOverworld() {
   // Set the appropriate flags
   this->playingMinigame = false;
   this->pov = true;

   // Send the camera to some abritrary place outside the overworld.
   this->pos = glm::vec3(0, 2, 0);
   this->theta = -M_PI/2.0;
   this->phi = 0.0;
}

void Camera::initPlayer(Object *_player) {
   player = _player;
}

glm::vec3 Camera::calculatePlayerPos() {
   glm::vec3 temp;
   float Yrot = getYRot();

   temp.x = pos.x;
   temp.y = 1;
   temp.z = pos.z;

   return temp;
}
  
