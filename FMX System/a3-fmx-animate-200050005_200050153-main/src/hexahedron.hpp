#include <GL/glew.h>

#include <string>
#include <vector>
#include "glm/glm.hpp"
#include "mesh.hpp"

class Hexahedron: public Mesh
{
private:
 glm::vec3 * cordinates;
 int m_idx=0;
 glm::vec3 m_positions[36];
 glm::vec3 m_normals[36];

public:
 Hexahedron(glm::vec3* cordinates, glm::vec3 color);
 void cube_vertices();
 void quad(int a, int b, int c, int d);
 virtual void render(glm::mat4& modelviewprojmatrix, glm::mat4 modelMatrix) const override;
};