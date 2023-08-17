#pragma once
#include "grid.hpp"
#include "cursor.hpp"
#include "shader.hpp"
#include "event.hpp"
#include "camera.hpp"
#include "model.hpp"


enum class Mode
{
  MODELLING_MODE,
  INSPECTION_MODE
};

class Scene
{
private:
  Camera m_camera;
  Grid m_grid;
  Cursor m_cursor;
  Model m_model;

  Shader m_gridShader;
  Shader m_cursorShader;
  Shader m_modelShader;

  Mode m_mode;
  //Shader m_main;
public:
  Scene(unsigned int voxel_res, unsigned int grid_res);

  void render();

  void eventDispatcher(Event& e);

  void windowSizeCallback(int width, int height);
};