#include "glm/glm.hpp"
#include "event.hpp"

class Camera
{
private:
  int width, height;
  struct rotData
  {
    float x = 0, y = 0, z = 0;
  };
  rotData m_rotdata;
public:
  Camera(int width, int height) :width(width), height(height) {};
  const glm::mat4 getOrientation();
  const glm::mat4 getProjectionViewMatrix();

  void setAspect(int width, int height)
  {
    this->width = width;
    this->height = height;
  };
  void inputHandle(Event& e);
};