#include <iomanip>
#include <filesystem>

#include "scene.hpp"
#include "window.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "stb_image_write.h"

#define FPS 30
#define PRINT_DATA
#define PRETTY_PRINT

Scene::Scene()
{
 glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
 glClearDepth(1.0);
 glEnable(GL_DEPTH_TEST);

 m_timeline.setScene(this);
 m_camera.setPosition(glm::vec3(-1, 5, 10));
 m_camera.changePitch(-30);

 m_plights[0] = PointLight("pointlight1", glm::vec3(-2, 4, 0));
 m_plights[1] = PointLight("pointlight2", glm::vec3(2, 4, 0));
 m_spotlight = FollowLight("spotlight", glm::vec3(0, 15, 0));
 m_headlight = HeadLight("headlight", glm::vec3(0, 0, 0), glm::vec3(0,0,70));


 m_track.loadFromFile();
 m_track.setupVAO();
 m_track.setScale(2);

 m_rider.setParent(m_bike.getRootNode());
 m_bike.getRootNode()->set_scale(0.05);

 m_context.currCam = &m_camera;
 m_context.currLight = &m_plights[0];
 m_context.currModel = RIDER;
 m_context.currNode = 0;

 m_spotlight.setTarget(m_rider.rnode[0]);
 m_headlight.setParent(m_bike.bnode[1]);//handle

 m_firstpCam.setParent(m_rider.rnode[3]);//head
 m_firstpCam.changeYaw(-180);
 m_firstpCam.setPosition(glm::vec3(0));

 m_thirdpCam.setParent(m_rider.rnode[3]);//head
 m_thirdpCam.changeYaw(-180);
 m_thirdpCam.changePitch(-24);
 m_thirdpCam.setPosition(glm::vec3(0, 8, -15));

 glActiveTexture(GL_TEXTURE0);
 glBindTexture(GL_TEXTURE_2D, m_trackTexture.getTextureID());
 glActiveTexture(GL_TEXTURE1);
 glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyboxTexture.getTextureID());
 glActiveTexture(GL_TEXTURE2);
 glBindTexture(GL_TEXTURE_2D, m_riderTexture.getTextureID());
 glActiveTexture(GL_TEXTURE3);
 glBindTexture(GL_TEXTURE_2D, m_bikeTexture.getTextureID());
 
 m_timeline.loadTimeline("keyframes.txt");
 updateFrame(m_timeline.getCurrentFrame());

 printData();
}

void Scene::render()
{
 if (is_playing)
  Play();

 glDepthFunc(GL_LESS);
 glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

 //bike+rider
 m_basicShader.use();
 m_basicShader.setUniformVec3("uCameraPos", m_context.currCam->getPosition());
 m_basicShader.setUniformInt("uPointLights[0].on", m_plights[0].on);
 m_basicShader.setUniformVec3("uPointLights[0].position", m_plights[0].getPosition());
 m_basicShader.setUniformFloat("uPointLights[0].range", m_plights[0].range);
 m_basicShader.setUniformInt("uPointLights[1].on", m_plights[1].on);
 m_basicShader.setUniformVec3("uPointLights[1].position", m_plights[1].getPosition());
 m_basicShader.setUniformFloat("uPointLights[1].range", m_plights[1].range);
 m_basicShader.setUniformInt("uSpotLight.on", m_spotlight.on);
 m_basicShader.setUniformVec3("uSpotLight.position", m_spotlight.getPosition());
 m_basicShader.setUniformVec3("uSpotLight.direction", m_spotlight.getDirection());
 m_basicShader.setUniformFloat("uSpotLight.innerCutoff", m_spotlight.getInnerCutoff());
 m_basicShader.setUniformFloat("uSpotLight.outerCutoff", m_spotlight.getOuterCutoff());
 m_basicShader.setUniformFloat("uSpotLight.range", m_spotlight.range);
 m_basicShader.setUniformInt("uHeadLight.on", m_headlight.on);
 m_basicShader.setUniformVec3("uHeadLight.position", m_headlight.getPosition());
 m_basicShader.setUniformVec3("uHeadLight.direction", m_headlight.getDirection());
 m_basicShader.setUniformFloat("uHeadLight.innerCutoff", m_headlight.innerCutoff);
 m_basicShader.setUniformFloat("uHeadLight.outerCutoff", m_headlight.outerCutoff);
 m_basicShader.setUniformFloat("uHeadLight.range", m_headlight.range);
 glm::mat4 projectview_matrix = m_context.currCam->getProjectionViewMatrix(true);
 m_bike.render(projectview_matrix);
 //track;
 m_trackShader.use();
 auto modelMat = m_track.getModelMatrix();
 m_trackShader.setUniformMat4("uProjViewModelMatrix", projectview_matrix*modelMat);
 m_trackShader.setUniformMat4("uModelMatrix", modelMat);
 m_trackShader.setUniformVec3("uCameraPos", m_context.currCam->getPosition());

 m_trackShader.setUniformInt("uPointLights[0].on", m_plights[0].on);
 m_trackShader.setUniformVec3("uPointLights[0].position", m_plights[0].getPosition());
 m_trackShader.setUniformFloat("uPointLights[0].range", m_plights[0].range);
 m_trackShader.setUniformInt("uPointLights[1].on", m_plights[1].on);
 m_trackShader.setUniformVec3("uPointLights[1].position", m_plights[1].getPosition());
 m_trackShader.setUniformFloat("uPointLights[1].range", m_plights[1].range);

 m_trackShader.setUniformInt("uSpotLight.on", m_spotlight.on);
 m_trackShader.setUniformVec3("uSpotLight.position", m_spotlight.getPosition());
 m_trackShader.setUniformVec3("uSpotLight.direction", m_spotlight.getDirection());
 m_trackShader.setUniformFloat("uSpotLight.innerCutoff", m_spotlight.getInnerCutoff());
 m_trackShader.setUniformFloat("uSpotLight.outerCutoff", m_spotlight.getOuterCutoff());
 m_trackShader.setUniformFloat("uSpotLight.range", m_spotlight.range);

 m_trackShader.setUniformInt("uHeadLight.on", m_headlight.on);
 m_trackShader.setUniformVec3("uHeadLight.position", m_headlight.getPosition());
 m_trackShader.setUniformVec3("uHeadLight.direction", m_headlight.getDirection());
 m_trackShader.setUniformFloat("uHeadLight.innerCutoff", m_headlight.innerCutoff);
 m_trackShader.setUniformFloat("uHeadLight.outerCutoff", m_headlight.outerCutoff);
 m_trackShader.setUniformFloat("uHeadLight.range", m_headlight.range);

 m_trackShader.setUniformInt("uColTex", 0);
 m_track.draw(m_trackShader);

 glDepthFunc(GL_LEQUAL);

 //skybox
 m_skyboxShader.use();
 m_skyboxShader.setUniformMat4("uProjViewMatrix", m_context.currCam->getProjectionViewMatrix(false));
 m_skyboxShader.setUniformInt("uCubemap", 1);
 m_skybox.render();



}

Frame Scene::createKeyframe()
{
  Frame keyframe;
  if(m_context.currCam == &m_camera)
    keyframe.cam = 0;
  else if(m_context.currCam == &m_firstpCam)
    keyframe.cam = 1;
  else if(m_context.currCam == &m_thirdpCam)
    keyframe.cam = 2;
  keyframe.plight1 = m_plights[0].on;
  keyframe.plight2 = m_plights[1].on;
  keyframe.spotlight = m_spotlight.on;
  keyframe.headlight = m_headlight.on;
  
  for(int i = 0; i < 21; i++)
  {
    keyframe.rnode_config[i].scale = m_rider.rnode[i]->get_scale();
    keyframe.rnode_config[i].rot = m_rider.rnode[i]->get_rot();
    keyframe.rnode_config[i].trans = m_rider.rnode[i]->get_trans();
  }

  for(int i = 0; i < 16; i++)
  {
    keyframe.bnode_config[i].scale = m_bike.bnode[i]->get_scale();
    keyframe.bnode_config[i].rot = m_bike.bnode[i]->get_rot();
    keyframe.bnode_config[i].trans = m_bike.bnode[i]->get_trans();
  }
  return keyframe;
}

void Scene::eventDispatcher(Event &e)
{
 int key = e.m_key;
 int action = e.m_action;
 int mods = e.m_mods;
 //switch cams
 if (key == GLFW_KEY_C && action == GLFW_PRESS)
 {
  if (m_context.currCam == &m_camera)
   m_context.currCam = &m_firstpCam;
  else if (m_context.currCam == &m_firstpCam)
   m_context.currCam = &m_thirdpCam;
  else if (m_context.currCam == &m_thirdpCam)
   m_context.currCam = &m_camera;
 }
 //switching lights
 else if (key == GLFW_KEY_N && action == GLFW_PRESS)
 {
  if (m_context.currLight == &m_plights[0])
   m_context.currLight = &m_plights[1];
  else if (m_context.currLight == &m_plights[1])
   m_context.currLight = &m_spotlight;
  else if (m_context.currLight == &m_spotlight)
   m_context.currLight = &m_headlight;
  else if (m_context.currLight == &m_headlight)
   m_context.currLight = &m_plights[0];
 }
 //switch models
 else if (key == GLFW_KEY_M && action == GLFW_PRESS)
 {
  m_context.currNode = 0;
  if (m_context.currModel == RIDER)
   m_context.currModel=BIKE;
  else if (m_context.currModel == BIKE)
   m_context.currModel = LIGHTS;
  else if (m_context.currModel == LIGHTS)
   m_context.currModel = RIDER;
 }
 //switching nodes
 else if (key == GLFW_KEY_PERIOD && action == GLFW_PRESS)
 {
  if (m_context.currModel == RIDER)
   m_context.currNode = (m_context.currNode + 1) % 21;
  else if (m_context.currModel == BIKE)
   m_context.currNode = (m_context.currNode + 1) % 16;
 }
 else if (key == GLFW_KEY_COMMA && action == GLFW_PRESS)
 {
  if (m_context.currModel == RIDER)
   m_context.currNode = (m_context.currNode - 1+ 21) % 21;
  else if (m_context.currModel == BIKE)
   m_context.currNode = (m_context.currNode - 1+ 16) % 16;
 }
 //playing
 else if (key == GLFW_KEY_P && action == GLFW_PRESS)
 {
  if (is_playing)
   stopPlay();
  else
   startPlay();
 }
 //recording
 else if (key == GLFW_KEY_R && action == GLFW_PRESS)
  is_recording = 1 - is_recording;
 
 m_context.currCam->inputHandle(e);
 if (m_context.currModel == RIDER)
  m_rider.rnode[m_context.currNode]->inputHandle(e);
 else if (m_context.currModel == BIKE)
  m_bike.bnode[m_context.currNode]->inputHandle(e);
 else if (m_context.currModel == LIGHTS)
  m_context.currLight->inputHandle(e);

 m_timeline.inputHandle(e);

 if (action != GLFW_RELEASE)
 {
  clearPrintedData();
  printData();
 }
}

void Scene::windowSizeCallback(int width, int height)
{
 m_camera.setAspect(width, height);
 m_firstpCam.setAspect(width, height);
 m_thirdpCam.setAspect(width, height);
}

void Scene::updateFrame(Frame frame)
{
 if (frame.cam == 0)
  m_context.currCam = &m_camera;
 else if (frame.cam == 1)
  m_context.currCam = &m_firstpCam;
 else if (frame.cam == 2)
  m_context.currCam = &m_thirdpCam;

 m_plights[0].on = frame.plight1;
 m_plights[1].on = frame.plight1;
 m_spotlight.on = frame.spotlight;
 m_headlight.on = frame.headlight;

 for (int i = 0; i < 21; i++)
 {
  m_rider.rnode[i]->set_scale(frame.rnode_config[i].scale);
  m_rider.rnode[i]->change_parameters(frame.rnode_config[i].trans, frame.rnode_config[i].rot);
 }
 for (int i = 0; i < 16; i++)
 {
  m_bike.bnode[i]->set_scale(frame.bnode_config[i].scale);
  m_bike.bnode[i]->change_parameters(frame.bnode_config[i].trans, frame.bnode_config[i].rot);
 }
}

void Scene::printData() const
{
#ifdef PRINT_DATA
 //camera
 std::cout << std::left << std::setw(40);
 if (m_context.currCam == &m_camera)
  std::cout << "Camera: (1) scene camera";
 else if (m_context.currCam == &m_firstpCam)
  std::cout << "Camera: (2) first person camera";
 else if (m_context.currCam == &m_thirdpCam)
  std::cout << "Camera: (3) third person camera";
 std::cout << " C to change | Arrow keys to look around | X,Y,Z+(shift) to move around"<<std::endl;
 //play/record
 if (is_playing)
  std::cout << "Play: ON  ";
 else
  std::cout << "Play: OFF ";
 if (is_recording)
  std::cout << "Record: ON  ";
 else
  std::cout << "Record: OFF ";
 std::cout << "                   P to toggle Play | R to toggle Record" << std::endl;
 //model
 std::cout << std::left << std::setw(40);
 if (m_context.currModel == BIKE)
 {
  std::cout << "Current model: BIKE" << " M key to change" << std::endl;
  m_bike.bnode[m_context.currNode]->printData();
 }
 else if (m_context.currModel == LIGHTS)
 {
  std::cout << "Current model: LIGHTS" << " M key to change" << std::endl;
  m_context.currLight->printData();
 }
 else if (m_context.currModel == RIDER)
 {
  std::cout << "Current model: RIDER" << " M key to change" << std::endl;
  m_rider.rnode[m_context.currNode]->printData();
 }
 //Frame
 std::cout << "Current Frame : " << std::left << std::setw(15)
  << m_timeline.getCurrentFrameNumber() << std::left << std::setw(10);
 if (m_timeline.isKeyframe())
  std::cout << "KeyFr";
 else
  std::cout << "";
 std::cout  << "[ and ] keys to change | A/D to add/delete keyframe | S/L to save/load timeline" << std::endl;
#endif // PRINT_DATA
}

void Scene::clearPrintedData() const
{
#ifdef PRINT_DATA
#ifdef PRETTY_PRINT
 for (int i = 0; i < 7; i++)
 {
  std::cout << "\033[A";
  std::cout << "\33[2K\r";
 }
 //std::cout << "\033[5;0f";
#endif //PRETTY_PRINT
#endif // PRINT_DATA
}

void Scene::startPlay()
{
 is_playing = true;
 prevtime = glfwGetTime();
 nowtime = prevtime;
}

void Scene::Play()
{
 nowtime = glfwGetTime();

 double deltatime = nowtime - prevtime;

 if (deltatime > 1/(float)FPS)
 {
  m_timeline.moveFrame(1);
  prevtime = prevtime+1/(float)FPS;
  //clearPrintedData();
  //printData();
  if(is_recording)
   record();
 }
}

void Scene::stopPlay()
{
 is_playing = false;
}

void Scene::record()
{
 int frame_n = m_timeline.getCurrentFrameNumber();
 std::string frame = "00000";

 int i = 4;
 while (frame_n != 0)
 {
  // char tmp[2];
  std::string tmp;
  // s(frame_n % 10, tmp, 10);
  tmp = std::to_string(frame_n%10);
  // itoa(frame_n % 10, tmp, 10);
  frame[i] = tmp[0];
  frame_n /= 10;
  i--;
 }
 std::filesystem::create_directory("./mov");

 frame = "./mov/" + frame + ".png";
 capture(&frame[0]);
}

void Scene::capture(char* filename)
{
 int width = m_context.currCam->getWidth();
 int height = m_context.currCam->getHeight();
 GLsizei nrChannels = 3;
 GLsizei stride = nrChannels * width;
 stride += (stride % 4) ? (4 - stride % 4) : 0;
 GLsizei bufferSize = stride * height;
 std::vector<char> buffer(bufferSize);
 glPixelStorei(GL_PACK_ALIGNMENT, 4);
 glReadBuffer(GL_FRONT);
 glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, buffer.data());
 stbi_flip_vertically_on_write(true);
 stbi_write_png(filename, width, height, nrChannels, buffer.data(), stride);
}

