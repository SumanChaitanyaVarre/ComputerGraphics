// using namespace std;
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include "window.hpp"
#include "scene.hpp"
#include "shader.hpp"
#include "track.hpp"

int main(int argc, char** argv)
{
 //! The pointer to the GLFW window
 Window window;
 Scene scene;

 // Load shaders and use the resulting shader program
 window.setScene(&scene);

 // Loop until the user closes the window
 while (window.shouldClose() == 0)
 {
  scene.render();
  // Swap front and back buffers
  window.swapBuffers();
  // Poll for and process events
  window.pollEvents();
 }

 window.terminate();
 return 0;
}