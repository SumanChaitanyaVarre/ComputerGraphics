#include <GL/glew.h>

#include <string>
#include <vector>
#include "glm/glm.hpp"
#include "mesh.hpp"

class Cube: public Mesh
{
private:
 glm::vec3 corner, dim;

public:
 Cube(glm::vec3 dimensions, glm::vec3 corner, glm::vec3 color);
 virtual void render(glm::mat4& modelviewprojmatrix, glm::mat4 modelMatrix) const override;
};

class Skybox : public Cube
{
public:
 Skybox(glm::vec3 dimensions, glm::vec3 corner, glm::vec3 color):
  Cube(dimensions, corner, color){}
 void render();
};