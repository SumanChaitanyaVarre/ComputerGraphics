#pragma once
#include "shader.hpp"
#include "event.hpp"
#include "camera.hpp"
#include "track.hpp"
#include "rider.hpp"
#include "bike.hpp"
#include "lights.hpp"
#include "timeline.hpp"
#include "texture.hpp"
#include "cube.hpp"

#define BIKE 0
#define RIDER 1
#define LIGHTS 2

class Scene
{
private:
 Timeline m_timeline;

 Texture m_trackTexture = Texture("./src/tex/col.jpg");
 Texture m_riderTexture = Texture("./src/tex/shirt.jpg");
 Texture m_bikeTexture = Texture("./src/tex/seat.jpg");
 Cubemap m_skyboxTexture = Cubemap("./src/tex/cubemap/");

 Shader m_basicShader = 
  Shader::createShader("./src/bike_vertex.glsl", "./src/bike_fragment.glsl");
 Shader m_trackShader = 
  Shader::createShader("./src/track_vertex.glsl", "./src/track_fragment.glsl");
 Shader m_skyboxShader = 
  Shader::createShader("./src/skybox_vertex.glsl", "./src/skybox_fragment.glsl");
 
 Camera m_camera = Camera(640, 480);
 AttachedCamera m_firstpCam = AttachedCamera(640, 480);
 AttachedCamera m_thirdpCam = AttachedCamera(640, 480);
 
 Skybox m_skybox = Skybox(glm::vec3(2), glm::vec3(-1), glm::vec3(0));
 Track m_track = Track("./src/track.data");
 Rider m_rider = Rider(NULL, m_basicShader);
 Bike m_bike = Bike(NULL, m_basicShader);

 PointLight m_plights[2];
 FollowLight m_spotlight;
 HeadLight m_headlight;


 bool is_playing = false;
 bool is_recording = false;
 double nowtime;
 double prevtime;

 struct sceneContext
 {
  Camera* currCam;
  int currModel;
  int currNode;
  Light* currLight;
 };
 sceneContext m_context;
 // Shader m_main;
public:
 Scene();

 void render();

 void eventDispatcher(Event &e);

 void windowSizeCallback(int width, int height);

 Frame createKeyframe();
 void updateFrame(Frame frame);
 void printData() const;
 void clearPrintedData() const;

 void startPlay(); 
 void Play(); 
 void stopPlay();
 
 void record();
 
 void capture(char* filename);



};