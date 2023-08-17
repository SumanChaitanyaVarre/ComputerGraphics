#pragma once

#include "glm/glm.hpp"
#include "lights.hpp"
#include "event.hpp"
#include <string>
class Scene;

struct NodeConfig
{
 float scale = 1.0f;
 glm::vec3 rot = glm::vec3(0.0, 0.0, 0.0);
 glm::vec3 trans = glm::vec3(0.0, 0.0, 0.0);
};

struct Frame
{
 int cam = 0;
 int plight1 = 1;
 int plight2 = 1;
 int headlight = 1;
 int spotlight = 1;
 NodeConfig rnode_config[21];
 NodeConfig bnode_config[17];
};

struct KeyNode
{
 unsigned int frame_number = 0;
 Frame keyframe;
 KeyNode* prev;
 KeyNode* next;
};

class Timeline
{
 unsigned int n_frames = 1800;
 unsigned int n_keyframes = 0;
 unsigned int curr_frame = 0;
 bool is_keyframe = false;
 KeyNode* m_headKeyNode;
 KeyNode* m_tailKeyNode;
 Scene* m_scene=NULL;
public:
 void setScene(Scene* scene) { m_scene = scene; };
 void insertKeyframe(unsigned int frame_number, Frame frame);
 void deletKeyframe();
 void moveFrame(int offset);
 Frame getCurrentFrame();
 unsigned int getCurrentFrameNumber() const { return curr_frame; };
 bool isKeyframe() const { return is_keyframe; };
 void inputHandle(Event &e);

 void saveTimeline(std::string path) const;
 void loadTimeline(std::string path);
};