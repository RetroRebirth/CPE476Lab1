#ifndef __Camera__
#define __Camera__

#include "includes.h"
#include "Window.h"
#include "Object.h"

class Camera {
   public:
      bool debug;
      float speed;
      glm::vec3 pos;
      glm::vec3 debug_pos;
      bool blocked;
      bool pov;
      glm::mat4 Projection;
      glm::mat4 View;


      
      Camera(
         GLint _h_uP,
         GLint _h_uV,
         GLint _h_uView);
      virtual ~Camera();

      void initPlayer(Object *_player);
      float getXRot();
      float getYRot();
      void toggleBounded();
      glm::vec3 lookAtPt();
      void setProjectionMatrix(int g_width, int g_height);
      void setView();
      void step(Window* window);
      void mouse_callback(GLFWwindow* window, double xpos, double ypos, int g_width, int g_height);
      void enter_callback(GLFWwindow* window, int entered, int g_width, int g_height);
      glm::vec3 calcNewPos(Window* window);
      void moveToMinigame();
      void moveToOverworld();
   private:
      float theta;
      float phi;
      float radius;
      GLint h_uP;
      GLint h_uV;
      GLint h_uView;
      bool playingMinigame;
      Object* player;
      float playerYrot;

      glm::vec3 calculatePlayerPos();
      inline void safe_glUniformMatrix4fv(const GLint handle, const GLfloat data[]);
};

#endif
