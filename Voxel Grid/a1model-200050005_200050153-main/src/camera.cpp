#include "camera.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "GLFW/glfw3.h"

const glm::mat4 Camera::getOrientation()
{
  glm::mat4 rotation_matrix = glm::rotate(
    glm::mat4(1.0f),
    m_rotdata.x,
    glm::vec3(1.0f, 0.0f, 0.0f));
  rotation_matrix = glm::rotate(
    rotation_matrix,
    m_rotdata.y,
    glm::vec3(0.0f, 1.0f, 0.0f));
  rotation_matrix = glm::rotate(
    rotation_matrix,
    m_rotdata.z,
    glm::vec3(0.0f, 0.0f, 1.0f));
  return rotation_matrix;
}

const glm::mat4 Camera::getProjectionViewMatrix()
{
  glm::mat4 rotation_matrix = getOrientation();

  glm::mat4 view_matrix = glm::lookAt(
    glm::vec3(0.0, 0.0, 2.0),
    glm::vec3(0.0, 0.0, 0.0),
    glm::vec3(0.0, 1.0, 0.0));
  float aspect_ratio = (float)width / (float)height;
  glm::mat4 ortho_matrix = glm::ortho(
    -2.0 * aspect_ratio, //left
    2.0 * aspect_ratio, //right
    -2.0, //bottom
    2.0, //top
    -20.0, //zNear
    20.0); //zFar
  return ortho_matrix * view_matrix * rotation_matrix;
}

void Camera::inputHandle(Event& e)
{
  int key = e.m_key;
  int action = e.m_action;
  int mods = e.m_mods;
  if (key == GLFW_KEY_LEFT && (action == GLFW_REPEAT || action == GLFW_PRESS))
    m_rotdata.y -= 0.1;
  else if (key == GLFW_KEY_RIGHT && (action == GLFW_REPEAT || action == GLFW_PRESS))
    m_rotdata.y += 0.1;
  else if (key == GLFW_KEY_UP && (action == GLFW_REPEAT || action == GLFW_PRESS))
    m_rotdata.x -= 0.1;
  else if (key == GLFW_KEY_DOWN && (action == GLFW_REPEAT || action == GLFW_PRESS))
    m_rotdata.x += 0.1;
  else if (key == GLFW_KEY_LEFT_BRACKET && (action == GLFW_REPEAT || action == GLFW_PRESS))
    m_rotdata.z -= 0.1;
  else if (key == GLFW_KEY_RIGHT_BRACKET && (action == GLFW_REPEAT || action == GLFW_PRESS))
    m_rotdata.z += 0.1;
}

