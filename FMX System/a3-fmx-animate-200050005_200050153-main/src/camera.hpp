#include "glm/glm.hpp"
#include "event.hpp"
#include "hierarchy_node.hpp"

class Camera
{
protected:
  int width, height;

  glm::vec3 m_position = glm::vec3(0.0f, 0.0f, 10.0f);
  float pitch = 0;
  float yaw = 270;
  
public:
 Camera(int width, int height) :width(width), height(height){};
  virtual const glm::mat4 getProjectionViewMatrix(bool translation);
  //returns view projection without translating camera
  void setPosition(glm::vec3 position) { m_position = position; };
  virtual const glm::vec3 getPosition() { return m_position; };
  glm::vec3 getForwardVector();
  void changePitch(float inc);
  void changeYaw(float inc);

  void setAspect(int width, int height)
  {
    this->width = width;
    this->height = height;
  };
  int getWidth() { return width; };
  int getHeight() { return height; };
  void inputHandle(Event& e);
};

class AttachedCamera :public Camera
{
private:
 HNode* m_parent;
public:
 AttachedCamera(int width, int height);
 const glm::mat4 getProjectionViewMatrix(bool translation) override;
 const glm::vec3 getPosition() override;
 void setParent(HNode* parent) { m_parent = parent; };
};