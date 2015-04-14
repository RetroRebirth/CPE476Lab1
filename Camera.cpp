#include "Camera.h"

using namespace std;

Camera::Camera(
      GLint _h_uP,
      GLint _h_uV,
      GLint _h_uView) {
   // Default attribute values
   view = glm::vec3(0, 1, 0);
   theta = -M_PI/2.0;
   phi = 0.0;
   bounded = true;
   speed = MOVEMENT_SPEED;

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

glm::vec3 Camera::lookAtPt() {
   glm::vec3 lookAtPt = glm::vec3(cos(phi)*cos(theta), sin(phi), cos(phi)*cos((M_PI/2)-theta));
   lookAtPt += view;
   return lookAtPt;
}

void Camera::setProjectionMatrix(int g_width, int g_height) {
   glm::mat4 Projection = glm::perspective(90.0f, (float)g_width/g_height, 0.1f, 100.f);
   safe_glUniformMatrix4fv(h_uP, glm::value_ptr(Projection));
}

void Camera::setView() {
   glm::mat4 View = glm::lookAt(view, lookAtPt(), glm::vec3(0, 1, 0));
   safe_glUniformMatrix4fv(h_uV, glm::value_ptr(View));
   glUniform3f(h_uView, view.x, view.y, view.z);
}

void Camera::step(Window* window) {
   updatePosition(window);
   setProjectionMatrix(window->width, window->height);
   setView();
}

void Camera::updatePosition(Window* window) {
   glm::vec3 viewVector = glm::normalize(lookAtPt() - view);
   glm::vec3 strafeVector = glm::normalize(glm::cross(viewVector, glm::vec3(0, 1, 0)));
   glm::vec3 crossVector = glm::normalize(glm::cross(viewVector, strafeVector));
   // Scale vectors
   viewVector *= (speed * window->dt);
   strafeVector *= (speed * window->dt);
   crossVector *=(speed * window->dt);

   GLFWwindow* win = window->glfw_window;
   if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS) // Move forward
      view += viewVector;
   if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS) // Move backward
      view -= viewVector;
   if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS) // Strafe left
      view -= strafeVector;
   if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS) // Strafe right
      view += strafeVector;
   if (glfwGetKey(win, GLFW_KEY_Q) == GLFW_PRESS) // Move up
      view += crossVector;
   if (glfwGetKey(win, GLFW_KEY_E) == GLFW_PRESS) // Move down
      view -= crossVector;
   
   // Bounding
   if (bounded) {
      if (view.x < -SIZE)
         view.x = -SIZE;
      if (view.x > SIZE)
         view.x = SIZE;
      if (view.z < -SIZE)
         view.z = -SIZE;
      if (view.z > SIZE)
         view.z = SIZE;

      view.y = 1;
   }
}

void Camera::mouse_callback(GLFWwindow* window, double xpos, double ypos, int g_width, int g_height) {
   // Update theta (x angle) and phi (y angle)
   float half_width = g_width / 2.0;
   float half_height = g_height / 2.0;
   float xPosFromCenter = xpos - half_width;
   float yPosFromCenter = ypos - half_height;
   float xMag = xPosFromCenter / half_width;
   float yMag = yPosFromCenter / half_height;

   theta += ROTATE_SPEED*M_PI*xMag;
   // Bound phi to 80 degrees
   float newPhi = phi - ROTATE_SPEED*M_PI*yMag/2.0;
   if (glm::degrees(newPhi) < 80 && glm::degrees(newPhi) > -80) {
      phi = newPhi;
   }

   // Keep mouse in center
   glfwSetCursorPos(window, g_width/2, g_height/2);
}

void Camera::enter_callback(GLFWwindow* window, int entered, int g_width, int g_height) {
   // Position mouse at center if enter screen
   glfwSetCursorPos(window, g_width/2, g_height/2);
}
