#include "window.hpp"

#include <iostream>
// int mode_of_operation = 0;

Window::Window()
{
  glfwSetErrorCallback(error_callback);

  //! Initialize GLFW
  if (!glfwInit())
    throw std::runtime_error("Cannot initialize GLFW");

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  m_window = glfwCreateWindow(640, 480, "CS475/CS675 OpenGL Framework", NULL, NULL);
  if (!m_window)
  {
    glfwTerminate();
    throw std::runtime_error("Cannot create window");
  }

  //! Make the window's context current
  glfwMakeContextCurrent(m_window);

  // Initialize GLEW
  // Turn this on to get Shader based OpenGL
  glewExperimental = GL_TRUE;
  GLenum err = glewInit();
  if (GLEW_OK != err)
  {
    // Problem: glewInit failed, something is seriously wrong.
    std::cerr << "GLEW Init Failed : %s" << std::endl;
  }

  // Print and see what context got enabled
  std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
  std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
  std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
  std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

  // Keyboard Callback
  glfwSetKeyCallback(m_window, key_callback);
  glfwSetWindowSizeCallback(m_window, windowsize_callback);
  // Framebuffer resize callback
  glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GL_TRUE);

  // opengl initialise state
  // TODO might want to create a context object
}

void Window::error_callback(int error, const char *description)
{
  std::cerr << description << std::endl;
}

void Window::framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  //! Resize the viewport to fit the window size - draw to entire window
  glViewport(0, 0, width, height);
}

void Window::windowsize_callback(GLFWwindow *window, int width, int height)
{
  Scene *scene = (Scene *)glfwGetWindowUserPointer(window);
  scene->windowSizeCallback(width, height);
}

void Window::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{

  //! Close the window if the ESC key was pressed
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
  else
  {
    Scene *scene = (Scene *)glfwGetWindowUserPointer(window);

    Event event({key, action, mods});
    scene->eventDispatcher(event);
  }
}
