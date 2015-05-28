#include "Camera.h"
using namespace std;

Camera::Camera(
      GLint _h_uP,
      GLint _h_uV,
      GLint _h_uView) {
   // Default attribute valuesf
   pos = glm::vec3(0.0f, 1.0f, 0.0f);
   debug_pos = pos;
   theta = -M_PI/2.0f;
   phi = 0.0f;
   debug = false;
   speed = INITIAL_SPEED;
   blocked = false;
   pov = true;
   playingMinigame = false;
   radius = 1.0f;
   playerYrot = 0.0f;

   // Defined attribute values
   h_uP = _h_uP;
   h_uV = _h_uV;
   h_uView = _h_uView;
}

Camera::~Camera() {}

inline void Camera::safe_glUniformMatrix4fv(const GLint handle, const GLfloat data[]) {
   if (handle >= 0) {
      glUniformMatrix4fv(handle, 1, GL_FALSE, data);
   }
}

float Camera::getYRot() {
   return Util::radiansToDegrees(theta);
}

float Camera::getXRot() {
   return Util::radiansToDegrees(phi);
}

glm::vec3 Camera::lookAtPt() {
   glm::vec3 lookAtPt = glm::vec3(cos(phi)*cos(theta), sin(phi), cos(phi)*cos((M_PI/2.0f)-theta));
   lookAtPt += debug ? debug_pos : player->pos;
   return lookAtPt;
}

void Camera::setProjectionMatrix(int g_width, int g_height) {
   Projection = glm::perspective(90.0f, (float)g_width/g_height, 0.1f, 300.f);
   safe_glUniformMatrix4fv(h_uP, glm::value_ptr(Projection));
}

void Camera::setView() {
   glm::vec3 curPos = debug ? debug_pos : player->pos;
   glm::mat4 lookAtMat = glm::lookAt(lookAtPt(), curPos, glm::vec3(0.0f, 1.0f, 0.0f));

   //mult view by phi rotation matrix
   glm::mat4 view_mat = glm::rotate(glm::mat4(1.0f), -1.0f*phi, glm::vec3(4.0f, 0.0f, 0.0f)) * lookAtMat;

   safe_glUniformMatrix4fv(h_uV, glm::value_ptr(view_mat));
   glUniform3f(h_uView, player->pos.x, player->pos.y, player->pos.z);

   if (!debug) {
      View = view_mat;
   }
}

void Camera::step(Window* window) {

   if (debug) {
      debug_pos = calcNewPos(window);
   } else {
      if (!playingMinigame && !blocked) {
         player->pos = calcNewPos(window);
      }
      blocked = false;
   }
   setProjectionMatrix(window->width, window->height);
   setView();
}

bool Camera::checkStaticObjectCollisions(Object* o, glm::vec3* colPlane) {
   for (int i=0; i<structures.size(); ++i) {
      if (structures[i]->planarCollisionCheck(player, colPlane)) {
         return true;
      }
   }
   for (int i=0; i<booths.size(); ++i) {
      if (booths[i]->booth[1]->planarCollisionCheck(player, colPlane)) {
         return true;
      }
      booths[i]->checkInteract(player->pos);
      
      if (booths[i]->isActive()) {
         char ln[60];
         string minigame = booths[i]->getMinigameDisplayName();
         fontEngine->useFont("goodDog", 48);
         sprintf(ln, "Press ENTER to play %s!\n", minigame.c_str());
         fontEngine->display(glm::vec4(0.99, 0.56, 0.55, 1.0), ln, 0-fontEngine->getTextWidth(ln)/2.0, 0.3);
      }
   }
   return false;
}

void Camera::applyProjectionMatrix(MatrixStack* P) {
   P->multMatrix(Projection);
}

void Camera::applyViewMatrix(MatrixStack* MV) {
   MV->multMatrix(View);
}

glm::vec3 Camera::calcNewPos(Window* window) {
   glm::vec3 newPos = debug ? debug_pos : player->pos;
   float moveInc = speed * window->dt;
   float playerYrad = Util::degreesToRadians(playerYrot);

   glm::vec3 viewVector = glm::normalize(newPos - lookAtPt());
   glm::vec3 strafeVector = glm::normalize(glm::cross(viewVector, glm::vec3(0.0f, 1.0f, 0.0f)));
   glm::vec3 crossVector = glm::normalize(glm::cross(viewVector, strafeVector));
   // Scale vectors
   viewVector *= moveInc;
   strafeVector *= moveInc;
   crossVector *= moveInc;

   GLFWwindow* win = window->glfw_window;
   if (!debug) {
      // Normal camera controls
      if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS) { // Move forward
         newPos.x += moveInc * sin(playerYrad);
         newPos.z += moveInc * cos(playerYrad);
         
         //pos.x += moveInc * sin(playerYrad);
         //pos.z += moveInc * cos(playerYrad);
      }
      if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS) { // Rotate left
         playerYrot += PLAYER_ROT_DEG;
         
         theta -= Util::degreesToRadians(PLAYER_ROT_DEG);
      }
      if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS) { // Rotate right
         playerYrot -= PLAYER_ROT_DEG;
         
         theta += Util::degreesToRadians(PLAYER_ROT_DEG);         
      }
      
      if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS) { // Move backward
         newPos.x -= moveInc * sin(playerYrad);
         newPos.z -= moveInc * cos(playerYrad);
      }
      
   } else {
      // Debug free-flying camera
      if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS) { // Move forward
         newPos += viewVector;
      }
      if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS) { // Move backward
         newPos -= viewVector;
      }
      if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS) { // Strafe left
         newPos -= strafeVector;
      }
      if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS) { // Strafe right
         newPos += strafeVector;
      }
      if (glfwGetKey(win, GLFW_KEY_Q) == GLFW_PRESS) { // Move up
         newPos += crossVector;
      }
      if (glfwGetKey(win, GLFW_KEY_E) == GLFW_PRESS) { // Move down
         newPos -= crossVector;
      }
   }

   // Bounding
  if (!debug) {
     float s = SIZE - 0.2f;
     if (newPos.x < -s)
        newPos.x = -s;
     if (newPos.x > s)
        newPos.x = s;
     if (newPos.z < -s)
        newPos.z = -s;
     if (newPos.z > s)
        newPos.z = s;

     newPos.y = 1.0f;
  }

   if (player != NULL) {
   
      // run hit detection on this so called new position!
      glm::vec3 colPlane = glm::vec3(0.0f, 0.0f, 0.0f);
      if (checkStaticObjectCollisions(player, &colPlane)) {
         // there is a hit...
         if (colPlane.x == -1.0f) { // hit minimum x plane
            //printf("hit minimum x plane\n");
            if (newPos.x > prevPos.x) {
               newPos = glm::vec3(prevPos.x-0.001f, newPos.y, newPos.z);
            }
         }
         else if (colPlane.x == 1.0f) { // hit maximum x plane
            //printf("hit maximum x plane\n");
            if (newPos.x < prevPos.x) {
               newPos = glm::vec3(prevPos.x+0.001f, newPos.y, newPos.z);
            }
         }
         else if (colPlane.z == -1.0f) { // hit minimum z plane
            //printf("hit minimum z plane\n");
            if (newPos.z > prevPos.z) {
               newPos = glm::vec3(newPos.x, newPos.y, prevPos.z-0.001f);
            }
         }
         else if (colPlane.z == 1.0f) { // hit maximum z plane
            //printf("hit maximum z plane\n");
            if (newPos.z < prevPos.z) {
               newPos = glm::vec3(newPos.x, newPos.y, prevPos.z+0.001f);
            }
         }
         else {
            //printf("Thats not good.... error in planar hit detection...\ncolPlane: ");
            //printVec3(colPlane);
         }
      }
      
      prevPos = newPos;
      player->setPos(newPos);//calculatePlayerPos());
      //player->scale(glm::vec3(1.0f, 1.0f, 1.0f));
      player->rotate(0.0, glm::vec3(0.0f, 0.0f, 0.0f));
      player->rotate(playerYrot, glm::vec3(0.0f, 1.0f, 0.0f));
      
      if (pov) {
         player->draw();
      }
   }
   
   return newPos;
}

void Camera::mouse_callback(GLFWwindow* window, double xpos, double ypos, int g_width, int g_height) {
   // Don't do anything if we are playing the minigame
   if (playingMinigame) {
      return;
   }

   // Update theta (x angle) and phi (y angle)
   float half_width = g_width / 2.0f;
   float half_height = g_height / 2.0f;
   float xPosFromCenter = xpos - half_width;
   float yPosFromCenter = ypos - half_height;
   float xMag = xPosFromCenter / half_width;
   float yMag = yPosFromCenter / half_height;

   theta += MOUSE_SPEED*M_PI*xMag;
   // Bound phi to 80 degrees
   float newPhi = phi + MOUSE_SPEED*M_PI*yMag/2.0;
   //bounded between 80 and -40 to keep from going into the char
   if (glm::degrees(newPhi) < 80 && glm::degrees(newPhi) > -40) {
      phi = newPhi;
   }

   // Keep mouse in center
   glfwSetCursorPos(window, g_width/2, g_height/2);
}

void Camera::enter_callback(GLFWwindow* window, int entered, int g_width, int g_height) {
   // Don't do anything if we are playing the minigame
   if (playingMinigame) {
      return;
   }

   // Position mouse at center if enter screen
   glfwSetCursorPos(window, g_width/2, g_height/2);
}

void Camera::moveToMinigame() {
   // Set the appropriate flags
   this->playingMinigame = true;
   this->pov = false;

   // Send camera to the origin
   this->player->pos = glm::vec3(0, 2, 0);
   this->theta = -M_PI/2.0;
   this->phi = 0.0;
}

void Camera::moveToOverworld() {
   // Set the appropriate flags
   this->playingMinigame = false;
   this->pov = true;
   playerYrot = 0.0f;
}

void Camera::initPlayer(Object *_player) {
   player = _player;
}
