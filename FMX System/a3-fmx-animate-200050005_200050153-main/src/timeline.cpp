#include "timeline.hpp"
#include "scene.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

Frame lerp(const Frame& f1, const Frame& f2, float ratio)
{
 Frame frame;

 frame.cam = f1.cam;
 frame.plight1 = f1.plight1;
 frame.plight2 = f1.plight2;
 frame.headlight = f1.headlight;
 frame.spotlight = f1.spotlight;
 for (int i = 0; i < 21; i++)
 {
  frame.rnode_config[i].scale = f1.rnode_config[i].scale*(1-ratio)+f2.rnode_config[i].scale*ratio;
  frame.rnode_config[i].rot = f1.rnode_config[i].rot*(1-ratio)+f2.rnode_config[i].rot*ratio;
  frame.rnode_config[i].trans= f1.rnode_config[i].trans*(1-ratio)+f2.rnode_config[i].trans*ratio;
 }
 for (int i = 0; i < 16; i++)
 {
  frame.bnode_config[i].scale = f1.bnode_config[i].scale*(1-ratio)+f2.bnode_config[i].scale*ratio;
  frame.bnode_config[i].rot = f1.bnode_config[i].rot*(1-ratio)+f2.bnode_config[i].rot*ratio;
  frame.bnode_config[i].trans= f1.bnode_config[i].trans*(1-ratio)+f2.bnode_config[i].trans*ratio;
 }
 return frame;
}


void Timeline::insertKeyframe(unsigned int frame_number, Frame frame)
{
 KeyNode* keynode = new KeyNode();
 if (frame_number == curr_frame)
  is_keyframe = true;
 
 keynode->frame_number = frame_number;
 keynode->keyframe = frame;
 n_keyframes++;

 if (m_headKeyNode == NULL)
 {
  m_headKeyNode = keynode;
  m_tailKeyNode = keynode;
  keynode->next = NULL;
  keynode->prev = NULL;
 }
 else
 {
  KeyNode* knode = m_headKeyNode;
  while (knode != NULL)
  {
   if (keynode->frame_number == knode->frame_number)
   {
    knode->keyframe = keynode->keyframe;
    n_keyframes--;
    delete keynode;
    break;
   }
   if (keynode->frame_number < knode->frame_number)
   {
    KeyNode* prevnode = knode->prev;
    if (prevnode != NULL)
    {
     prevnode->next = keynode;
     keynode->prev = prevnode;
    }
    else
    {
     m_headKeyNode = keynode;
    }
    keynode->next = knode;
    knode->prev = keynode;
    break;
   }
   knode = knode->next;
  }
  //adding keynode to end of list
  if (knode == NULL)
  {
   m_tailKeyNode->next = keynode;
   keynode->prev = m_tailKeyNode;
   m_tailKeyNode = keynode;
  }
 }
}

Frame Timeline::getCurrentFrame()
{
 Frame currframe;
 if (m_headKeyNode == NULL)
  return currframe;

 KeyNode* knode = m_headKeyNode;
 while (knode != NULL)
 {
  if (knode->frame_number == curr_frame)
   return knode->keyframe;
  if (knode->frame_number > curr_frame)
   break;
  knode = knode->next;
 }
 if (knode == NULL)
  return m_tailKeyNode->keyframe;
 else if (knode == m_headKeyNode)
  return m_headKeyNode->keyframe;
 else
 {
  return lerp(
   knode->prev->keyframe,
   knode->keyframe,
   (curr_frame - knode->prev->frame_number) / (float)(knode->frame_number - knode->prev->frame_number));
 }
}

void Timeline::deletKeyframe()
{
 KeyNode *knode = m_headKeyNode;

 is_keyframe = false;
 while (knode != NULL)
 {
  if (knode->frame_number == curr_frame)
  {
   KeyNode *prenode = knode->prev;
   KeyNode *postnode = knode->next;
   delete knode;
   if (prenode == NULL)
   {
    m_headKeyNode = postnode;
    m_headKeyNode->prev = NULL;
   }
   else if (postnode == NULL)
   {
    m_tailKeyNode = prenode;
    m_tailKeyNode->next = NULL;
   }
   else
   {
    prenode->next = postnode;
    postnode->prev = prenode;
   }
   n_keyframes--;
   break;
  }
  knode = knode->next;
 }
}

void Timeline::moveFrame(int offset)
{
 if (offset < -(int)curr_frame)
  curr_frame = 0;
 else
  curr_frame = curr_frame + offset;
 if (curr_frame >= n_frames)
  curr_frame = n_frames-1;

 KeyNode* knode=m_headKeyNode;
 is_keyframe = false;
 while (knode != NULL)
 {
  if (curr_frame == knode->frame_number)
   is_keyframe = true;
  knode = knode->next;
 }
 m_scene->updateFrame(getCurrentFrame());
}

void Timeline::saveTimeline(std::string path) const
{
 std::ofstream file;
 file.open(path);
 file << n_keyframes << "\n";
 KeyNode *knode = m_headKeyNode;
 while (knode != NULL)
 {
  file << knode->frame_number << " "
   << knode->keyframe.cam << " "
   << knode->keyframe.plight1 << " "
   << knode->keyframe.plight2 << " "
   << knode->keyframe.headlight << " "
   << knode->keyframe.spotlight << " ";
  for (int i = 0; i < 21; i++)
  {
   file 
    << knode->keyframe.rnode_config[i].scale << " "
    << knode->keyframe.rnode_config[i].rot[0] << " "
    << knode->keyframe.rnode_config[i].rot[1] << " "
    << knode->keyframe.rnode_config[i].rot[2] << " "
    << knode->keyframe.rnode_config[i].trans[0] << " "
    << knode->keyframe.rnode_config[i].trans[1] << " "
    << knode->keyframe.rnode_config[i].trans[2] << " ";
  }

  for (int i = 0; i < 16; i++)
  {
   file 
    << knode->keyframe.bnode_config[i].scale << " "
    << knode->keyframe.bnode_config[i].rot[0] << " " 
    << knode->keyframe.bnode_config[i].rot[1] << " " 
    << knode->keyframe.bnode_config[i].rot[2] << " "
    << knode->keyframe.bnode_config[i].trans[0] << " "
    << knode->keyframe.bnode_config[i].trans[1] << " " 
    << knode->keyframe.bnode_config[i].trans[2] << " ";
  }
  file << "\n";
  knode = knode->next;
 }
 file.close();
}

void Timeline::loadTimeline(std::string path)
{
 KeyNode* knode = m_headKeyNode;
 while (knode != NULL)
 {
  KeyNode* prevnode = knode;
  knode = knode->next;
  delete prevnode;
 }
 n_keyframes = 0;
 m_headKeyNode = NULL;
 m_tailKeyNode = NULL;

 std::ifstream file(path);
 unsigned int n_framesinfile;
 file >> n_framesinfile;
 unsigned int frame_number;
 Frame frame;
 for (int i = 0; i < n_framesinfile; i++)
 {
  file >> frame_number
   >> frame.cam
   >> frame.plight1
   >> frame.plight2
   >> frame.headlight
   >> frame.spotlight;
  for (int i = 0; i < 21; i++)
  {
   file
    >> frame.rnode_config[i].scale
    >> frame.rnode_config[i].rot[0]
    >> frame.rnode_config[i].rot[1]
    >> frame.rnode_config[i].rot[2]
    >> frame.rnode_config[i].trans[0]
    >> frame.rnode_config[i].trans[1]
    >> frame.rnode_config[i].trans[2];
  }

  for (int i = 0; i < 16; i++)
  {
   file
    >> frame.bnode_config[i].scale
    >> frame.bnode_config[i].rot[0]
    >> frame.bnode_config[i].rot[1]
    >> frame.bnode_config[i].rot[2]
    >> frame.bnode_config[i].trans[0]
    >> frame.bnode_config[i].trans[1]
    >> frame.bnode_config[i].trans[2];
  }
  insertKeyframe(frame_number, frame);
 }
}
void Timeline::inputHandle(Event& e)
{
 int key = e.m_key;
 int action = e.m_action;
 int mods = e.m_mods;

 if (key == GLFW_KEY_RIGHT_BRACKET && action != GLFW_RELEASE){
  if(mods == GLFW_MOD_SHIFT)
    moveFrame(10);
  else
    moveFrame(1);
 }
 else if (key == GLFW_KEY_LEFT_BRACKET && action != GLFW_RELEASE)
  {
    if(mods == GLFW_MOD_SHIFT)
      moveFrame(-10);
    else
      moveFrame(-1);
  }
 else if (key == GLFW_KEY_A && action == GLFW_PRESS)
  insertKeyframe(curr_frame, m_scene->createKeyframe());
 else if (key == GLFW_KEY_D && action == GLFW_PRESS)
 {
  deletKeyframe();
  m_scene->updateFrame(getCurrentFrame()); 
 }
 else if (key == GLFW_KEY_S && action == GLFW_PRESS)
  saveTimeline("keyframes.txt");
 else if (key == GLFW_KEY_L && action == GLFW_PRESS)
 {
  loadTimeline("keyframes.txt");
  m_scene->updateFrame(getCurrentFrame()); 
 }
}