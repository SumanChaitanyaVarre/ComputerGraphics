#include "camera.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "GLFW/glfw3.h"

//const glm::mat4 Camera::getOrientation()
//{
//  glm::mat4 rotation_matrix = glm::rotate(
//    glm::mat4(1.0f),
//    m_rotdata.x,
//    glm::vec3(1.0f, 0.0f, 0.0f));
//  rotation_matrix = glm::rotate(
//    rotation_matrix,
//    m_rotdata.y,
//    glm::vec3(0.0f, 1.0f, 0.0f));
//  rotation_matrix = glm::rotate(
//    rotation_matrix,
//    m_rotdata.z,
//    glm::vec3(0.0f, 0.0f, 1.0f));
//  return rotation_matrix;
//}


const glm::mat4 Camera::getProjectionViewMatrix(bool translation)
{

 glm::mat4 view_matrix = glm::lookAt(
  m_position,
  m_position + getForwardVector(),
  glm::vec3(0.0, 1.0, 0.0));

 if (!translation)
  view_matrix = glm::mat4(glm::mat3(view_matrix));
 //glm::mat4 proj_matrix = glm::ortho(
 //  -4.0/ m_zoom * aspect_ratio, //left
 //  4.0/m_zoom * aspect_ratio, //right
 //  -4.0/m_zoom, //bottom
 //  4.0/m_zoom, //top
 //  -20.0, //zNear
 //  20.0); //zFar
 
 float aspect_ratio = (float)width / (float)height;
 glm::mat4 proj_matrix = glm::perspective(glm::radians(45.0f), aspect_ratio, 0.1f, 100.0f);

 return proj_matrix * view_matrix;
}

glm::vec3 Camera::getForwardVector()
{
 //glm::mat4 rotation_matrix = glm::rotate(
 //  glm::mat4(1.0f),
 //  glm::radians(pitch),
 //  glm::vec3(1.0f, 0.0f, 0.0f));
 //rotation_matrix = glm::rotate(
 //  rotation_matrix,
 //  glm::radians(yaw),
 //  glm::vec3(0.0f, 1.0f, 0.0f));

 //auto forwardVec =rotation_matrix * glm::vec4(0, 0, -1, 0);
 glm::vec3 front;
 front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
 front.y = sin(glm::radians(pitch));
 front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
 front = glm::normalize(front);
 return glm::vec3(front);
}

void Camera::changePitch(float inc)
{
 pitch += inc;
 if (pitch > 89.0f)
  pitch = 89.0f;
 if (pitch < -89.0f)
  pitch = -89.0f;
}

void Camera::changeYaw(float inc)
{
 yaw += inc;
 if (yaw > 360)
  yaw-=360;
 if (pitch < 0)
  yaw += 360;
}

void Camera::inputHandle(Event& e)
{
  int key = e.m_key;
  int action = e.m_action;
  int mods = e.m_mods;
  if (key == GLFW_KEY_DOWN && (action == GLFW_REPEAT || action == GLFW_PRESS))
   changePitch(-1.5);
  else if (key == GLFW_KEY_UP && (action == GLFW_REPEAT || action == GLFW_PRESS))
   changePitch(1.5);
  else if (key == GLFW_KEY_RIGHT && (action == GLFW_REPEAT || action == GLFW_PRESS))
   changeYaw(1.5);
  else if (key == GLFW_KEY_LEFT && (action == GLFW_REPEAT || action == GLFW_PRESS))
   changeYaw(-1.5);
  else if (key == GLFW_KEY_X && mods != GLFW_MOD_SHIFT && action != GLFW_RELEASE)
   m_position.x += 0.1;
  else if (key == GLFW_KEY_X && mods == GLFW_MOD_SHIFT && action != GLFW_RELEASE)
   m_position.x -= 0.1;
  else if (key == GLFW_KEY_Y && mods != GLFW_MOD_SHIFT && action != GLFW_RELEASE)
   m_position.y += 0.1;
  else if (key == GLFW_KEY_Y && mods == GLFW_MOD_SHIFT && action != GLFW_RELEASE)
   m_position.y -= 0.1;
  else if (key == GLFW_KEY_Z && mods != GLFW_MOD_SHIFT && action != GLFW_RELEASE)
   m_position.z += 0.1;
  else if (key == GLFW_KEY_Z && mods == GLFW_MOD_SHIFT && action != GLFW_RELEASE)
   m_position.z -= 0.1;
}

AttachedCamera::AttachedCamera(int width, int height):Camera(width, height)
{
 m_parent = NULL;
}

const glm::mat4 AttachedCamera::getProjectionViewMatrix(bool translation)
{
 auto transMat = m_parent->getTransformationMatrix();
 auto position = glm::vec3(transMat*glm::vec4(m_position,1));
 auto forwardVec = glm::vec3(transMat*glm::vec4(getForwardVector(),0));

 glm::mat4 view_matrix = glm::lookAt(
  position,
  position + forwardVec,
  glm::vec3(0.0, 1.0, 0.0));

 if (!translation)
  view_matrix = glm::mat4(glm::mat3(view_matrix));

 float aspect_ratio = (float)width / (float)height;
 glm::mat4 proj_matrix = glm::perspective(glm::radians(45.0f), aspect_ratio, 0.1f, 100.0f);

 return proj_matrix * view_matrix;
}

const glm::vec3 AttachedCamera::getPosition()
{
 auto transMat = m_parent->getTransformationMatrix();
 return glm::vec3(transMat * glm::vec4(m_position, 1));
}
