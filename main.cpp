#include "includes.h"
#include "Session.h"

Session* session;

/** MAIN **/
int main(int argc, char **argv) {
   session = new Session(); // Create the main game session

   session->run(); // Runs the main game loop

   delete session; // Free the main game session from the heap

   return 0;
}
