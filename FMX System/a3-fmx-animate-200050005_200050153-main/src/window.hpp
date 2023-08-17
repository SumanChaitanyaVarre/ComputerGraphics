#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <functional>
#include "event.hpp"
#include "scene.hpp"

// static int mode_of_operation = 0;
class Window
{
private:
  // Scene m_scene;
  // OpenglContext m_context;
  Scene *m_scene=NULL;
  GLFWwindow *m_window;

public:
  Window();
  void setScene(Scene *scene)
  {
    m_scene = scene;
    glfwSetWindowUserPointer(m_window, m_scene);
  }

  bool shouldClose() { return glfwWindowShouldClose(m_window); };
  void swapBuffers() { glfwSwapBuffers(m_window); };
  void pollEvents() { glfwPollEvents(); };
  void terminate() { glfwTerminate(); };
  static void error_callback(int error, const char *description);
  static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
  static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
  static void windowsize_callback(GLFWwindow *window, int width, int height);
};
