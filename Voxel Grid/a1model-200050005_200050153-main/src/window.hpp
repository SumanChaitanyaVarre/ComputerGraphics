#ifndef _WINDOW_HPP_
#define _WINDOW_HPP_
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <functional>
#include "event.hpp"

// static int mode_of_operation = 0;
class Window
{
private:
  
  //Scene m_scene;
  //OpenglContext m_context;
  struct WindowData
  {
    std::function<void(Event&)> m_event_handler;
    std::function<void(int width, int height)> m_windowsize_callback;
  };
  WindowData m_data;
  GLFWwindow* m_window;
  
public:
  Window();
  void setEventHandler(std::function<void(Event&)> func)
  {
    m_data.m_event_handler = func;
  }
  void setWindowSizeCallback(std::function<void(int, int)> func)
  {
    m_data.m_windowsize_callback = func;
  }

  bool shouldClose() { return glfwWindowShouldClose(m_window); };
  void swapBuffers() { glfwSwapBuffers(m_window); };
  void pollEvents() { glfwPollEvents(); };
  void terminate() { glfwTerminate(); };
  static void error_callback(int error, const char* description);
  static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
  static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
  static void windowsize_callback(GLFWwindow* window, int width, int height);
};




#endif