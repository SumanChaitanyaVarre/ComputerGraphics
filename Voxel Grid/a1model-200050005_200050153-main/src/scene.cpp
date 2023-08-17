#include "scene.hpp"
#include "window.hpp"
#include "glm/gtc/matrix_transform.hpp"

Scene::Scene(unsigned int voxel_res, unsigned int grid_res) :
  m_camera(Camera(640 , 480)),
  m_grid(Grid(grid_res)),
  m_model(Model(voxel_res)),
  m_cursor(Cursor(voxel_res, m_model)),
  m_mode(Mode::MODELLING_MODE)
{
  std::cout << "Modelling Mode is active" << std::endl;
  m_gridShader = Shader::createShader("./src/simple_vs.glsl", "./src/simple_fs.glsl");
  m_cursorShader = Shader::createShader("./src/cursor_vs.glsl", "./src/cursor_fs.glsl");
  m_modelShader = Shader::createShader("./src/modelcube_vs.glsl", "./src/modelcube_fs.glsl");
  m_grid.setupVAO();
  m_cursor.setupVAO();
  m_model.setupVAO();

  glClearDepth(1.0);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  //m_cursor.setAddCubeFunction(std::bind(&Model::addCube, &m_model, std::placeholders::_1, std::placeholders::_2));
}

void Scene::render()
{
  glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 /* glm::mat4 rotation_matrix = m_camera.getOrientation();

  glm::mat4 view_matrix = glm::lookAt(
    glm::vec3(0.0, 0.0, -2.0),
    glm::vec3(0.0, 0.0, 0.0),
    glm::vec3(0.0, 1.0, 0.0));
  glm::mat4 ortho_matrix = glm::ortho(-2.0, 2.0, -2.0, 2.0, -20.0, 20.0);*/
  glm::mat4 projectview_matrix = m_camera.getProjectionViewMatrix();


  m_model.draw(projectview_matrix, m_modelShader);


  if (m_mode == Mode::MODELLING_MODE)
  {
    m_gridShader.use();
    m_gridShader.setUniformMat4("uModelViewProjectMatrix", projectview_matrix);
    m_grid.draw();
    projectview_matrix = projectview_matrix * m_cursor.getModelMatrix();
    m_cursorShader.use();
    m_cursorShader.setUniformMat4("uModelViewProjectMatrix", projectview_matrix);
    // cursor_shader.setUniformMat4("uTranslation", translation);
    m_cursor.draw();
  }
}

void Scene::eventDispatcher(Event& e)
{
  int key = e.m_key;
  int action = e.m_action;
  int mods = e.m_mods;
  if (key == GLFW_KEY_M && action == GLFW_PRESS) {
    m_mode = Mode::MODELLING_MODE;
    std::cout << "Modelling Mode is active" << std::endl;
  }
  else if (key == GLFW_KEY_I && action == GLFW_PRESS) {
    m_mode = Mode::INSPECTION_MODE;
    std::cout << "Inspection Mode is active" << std::endl;
  }

  //only pass modifying events if in modelling mode
  if(m_mode == Mode::MODELLING_MODE)
    m_cursor.inputHandle(e);
  m_camera.inputHandle(e);
  m_model.inputHandle(e);
}

void Scene::windowSizeCallback(int width, int height)
{
  m_camera.setAspect(width, height);
}