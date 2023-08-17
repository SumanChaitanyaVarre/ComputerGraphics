#include "window.hpp"
#include "scene.hpp"
#include <vector>

int main(int argc, char **argv)
{
  Window window;
  Scene scene(50, 25);

  window.setEventHandler(std::bind(&Scene::eventDispatcher, &scene, std::placeholders::_1));
  window.setWindowSizeCallback(std::bind(
    &Scene::windowSizeCallback,
    &scene,
    std::placeholders::_1,
    std::placeholders::_2
    ));
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
