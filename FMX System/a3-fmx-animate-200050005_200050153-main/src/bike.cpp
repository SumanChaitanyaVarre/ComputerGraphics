#include "bike.hpp"
#include "cube.hpp"
#include "sphere.hpp"
#include "torus.hpp"
#include "hexahedron.hpp"

Bike::Bike(HNode* parent, Shader& shader)
{
  // engine vertices
  glm::vec3 engine_vertex[8];
  engine_vertex[0] = glm::vec3(-1.9f, -0.6f, 0.4f); 
  engine_vertex[1] = glm::vec3(1.9f, -0.6f, 0.4f);
  engine_vertex[2] = glm::vec3(3.0f, 1.0f, 0.4f);
  engine_vertex[3] = glm::vec3(-3.0f, 1.0f, 0.4f);
  engine_vertex[4] = glm::vec3(-1.9f, -0.6f, -0.4f);
  engine_vertex[5] = glm::vec3(1.9f, -0.6f, -0.4f);
  engine_vertex[6] = glm::vec3(3.0f, 1.0f, -0.4f);
  engine_vertex[7] = glm::vec3(-3.0f, 1.0f, -0.4f);
  // tank vertices
  glm::vec3 tank_vertex[8];
  tank_vertex[0] = glm::vec3(-1.65f, 0.0f, 0.44f); 
  tank_vertex[1] = glm::vec3(1.65f, 0.0f, 0.44f);
  tank_vertex[2] = glm::vec3(1.0f, 1.2f, 0.44f);
  tank_vertex[3] = glm::vec3(-1.0f, 1.2f, 0.44f);
  tank_vertex[4] = glm::vec3(-1.65f, 0.0f, -0.44f);
  tank_vertex[5] = glm::vec3(1.65f, 0.0f, -0.44f);
  tank_vertex[6] = glm::vec3(1.0f, 1.2f, -0.44f);
  tank_vertex[7] = glm::vec3(-1.0f, 1.2f, -0.44f);
  // handle rod vertices
  glm::vec3 hrod_vertex[8];
  hrod_vertex[0] = glm::vec3(0.3f, -2.1f, 0.2f);
  hrod_vertex[1] = glm::vec3(0.7f, -2.1f, 0.2f);
  hrod_vertex[2] = glm::vec3(-0.2f, 2.6f, 0.2f);
  hrod_vertex[3] = glm::vec3(-0.6f, 2.3f, 0.2f);
  hrod_vertex[4] = glm::vec3(0.3f, -2.1f, -0.2f);
  hrod_vertex[5] = glm::vec3(0.7f, -2.1f, -0.2f);
  hrod_vertex[6] = glm::vec3(-0.2f, 2.6f, -0.2f);
  hrod_vertex[7] = glm::vec3(-0.6f, 2.3f, -0.2f);
  // back_rod vertices
  glm::vec3 back_rod_vertex[8];
  back_rod_vertex[0] = glm::vec3(-3.4f, -1.2f, 0.2f); 
  back_rod_vertex[1] = glm::vec3(0.1f, -0.2f, 0.2f);
  back_rod_vertex[2] = glm::vec3(-0.1f, 0.2f, 0.2f);
  back_rod_vertex[3] = glm::vec3(-3.6f, -0.8f, 0.2f);
  back_rod_vertex[4] = glm::vec3(-3.4f, -1.2f, -0.2f);
  back_rod_vertex[5] = glm::vec3(0.1f, -0.2f, -0.2f);
  back_rod_vertex[6] = glm::vec3(-0.1f, 0.2f, -0.2f);
  back_rod_vertex[7] = glm::vec3(-3.6f, -0.8f, -0.2f);

  // Creating the hierarchy:
  Torus *wheel = new Torus(0.23, 1.3, 100, 100, glm::vec3(0.1));
  Hexahedron *engine = new Hexahedron(engine_vertex, glm::vec3(0.2));
  Cube *seat = new Cube(glm::vec3(5.0, 0.7, 0.8), glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.1));
  Hexahedron *tank = new Hexahedron(tank_vertex, glm::vec3(0.0, 0.8, 0.0));
  Hexahedron *hrod = new Hexahedron(hrod_vertex, glm::vec3(0.2));
  Hexahedron *back_rod = new Hexahedron(back_rod_vertex, glm::vec3(0.3));
  Cube *handle = new Cube(glm::vec3(0.5, 0.4, 2.6), glm::vec3(0.0, 0.0, -1.3), glm::vec3(0.1));
  Cube *spoke = new Cube(glm::vec3(2.6, 0.04, 0.04), glm::vec3(-1.3, -0.02, -0.02), glm::vec3(0.5));


  // note that the buffers are initialized in the respective constructors

  wheel->setShader(shader);
  engine->setShader(shader);
  seat->setShader(shader);
  tank->setShader(shader);
  hrod->setShader(shader);
  back_rod->setShader(shader);
  handle->setShader(shader);
  spoke->setShader(shader);

  seat->setTextureSlot(3);
  // engine
  bnode[0] = new HNode(NULL, engine, "engine");
  // handle
  bnode[1] = new HNode(bnode[0], handle, "handle");
  bnode[1]->change_parameters(glm::vec3(2.4, 3.3, 0.0), glm::vec3(0.0));
  // hrod
  bnode[2] = new HNode(bnode[1], hrod, "handle_rod");
  bnode[2]->change_parameters(glm::vec3(0.6, -2.2, 0.0), glm::vec3(0.0));
  // back_rod
  bnode[3] = new HNode(bnode[0], back_rod, "back_rod");
  bnode[3]->change_parameters(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0));
  // wheel1
  bnode[4] = new HNode(bnode[2], wheel, "wheel_front");
  bnode[4]->change_parameters(glm::vec3(0.5, -2.0, 0.0), glm::vec3(0.0));
  // wheel2
  bnode[5] = new HNode(bnode[0], wheel, "wheel_back");
  bnode[5]->change_parameters(glm::vec3(-3.5, -1.0, 0.0), glm::vec3(0.0));
  // seat
  bnode[6] = new HNode(bnode[0], seat, "seat");
  bnode[6]->change_parameters(glm::vec3(-6.0, 1.0, -0.4), glm::vec3(0.0));
  // tank
  bnode[7] = new HNode(bnode[0], tank, "tank");
  bnode[7]->change_parameters(glm::vec3(1.35, 1.0, 0.0), glm::vec3(0.0));
  // spoke
  bnode[8] = new HNode(bnode[4], spoke, "spoke_front1");
  bnode[8]->change_parameters(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0));
  bnode[9] = new HNode(bnode[4], spoke, "spoke_front2");
  bnode[9]->change_parameters(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 45.0));
  bnode[10] = new HNode(bnode[4], spoke, "spoke_front3");
  bnode[10]->change_parameters(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 90.0));
  bnode[11] = new HNode(bnode[4], spoke, "spoke_front4");
  bnode[11]->change_parameters(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, -45.0));
  // spoke
  bnode[12] = new HNode(bnode[5], spoke, "spoke_back1");
  bnode[12]->change_parameters(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0));
  bnode[13] = new HNode(bnode[5], spoke, "spoke_back2");
  bnode[13]->change_parameters(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 45.0));
  bnode[14] = new HNode(bnode[5], spoke, "spoke_back3");
  bnode[14]->change_parameters(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 90.0));
  bnode[15] = new HNode(bnode[5], spoke, "spoke_back4");
  bnode[15]->change_parameters(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, -45.0));

}

void Bike::render(glm::mat4& viewProjMatrix)
{
 bnode[0]->render_tree(viewProjMatrix);
}
