#ifndef __Camera__
#define __Camera__

#include "includes.h"
#include "Window.h"
#include "Object.h"
#include "Booth.h"
//#include "World.h"

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
      

      //World* world;
      
      Camera(
         GLint _h_uP,
         GLint _h_uV,
         GLint _h_uView);
      virtual ~Camera();
      
      // world items loaded in by session
      vector<Booth*> booths;
      vector<Object*> structures;

      void initPlayer(Object *_player);
      float getXRot();
      float getYRot();
      void toggleBounded();
      glm::vec3 lookAtPt();
      void setProjectionMatrix(int g_width, int g_height);
      void setView();
      void applyProjectionMatrix(MatrixStack* P);
      void applyViewMatrix(MatrixStack* MV);
      
      void step(Window* window);//, bool playerHit, glm::vec3 colPlane);
      void mouse_callback(GLFWwindow* window, double xpos, double ypos, int g_width, int g_height);
      void enter_callback(GLFWwindow* window, int entered, int g_width, int g_height);
      glm::vec3 calcNewPos(Window* window);//, bool playerHit, glm::vec3 colPlane);
      void moveToMinigame();
      void moveToOverworld();
   private:
      glm::vec3 prevPos;
      float theta;
      float phi;
      float radius;
      GLint h_uP;
      GLint h_uV;
      GLint h_uView;
      bool playingMinigame;
      Object* player;
      float playerYrot;
      bool checkStaticObjectCollisions(Object* o, glm::vec3* colPlane);

      //glm::vec3 setPlayerPos();
      inline void safe_glUniformMatrix4fv(const GLint handle, const GLfloat data[]);
};

#endif
