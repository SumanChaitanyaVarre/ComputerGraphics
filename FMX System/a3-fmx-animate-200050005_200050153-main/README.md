# GRAPHICS ASSIGNMENT-3

Team Members
  * Alan Babu-200050005
  * Varre Suman Chaitanya-200050153

Declaration-Hereby, We declare that this assignment is on our own and we did not copy it from anywhere.

Citations-www.learnopengl.com, openGL tutorials provided by the professor.

# Link to video:

[Video link](https://youtu.be/YqYhguLE-cY)

## Warning - Scene data printing :
      
Make sure console/terminal is wide enough for printing to work properly.

If printing is buggy disable pretty printing by commenting line 11 in scene.cpp 

```#define PRETTY_PRINT```

comment line 10 in in scene.cpp to disable printing

```#define PRINT_DATA```


# Key Bindings :

Camera

      Change positon: Press/hold X,Y,Z + (Shift)
      Look around : Press/hold arrow keys
      Cycle through cameras: C 

Models: BIKE, RIDER, LIGHTS

      Cycle through models: M
      Cycle through heirarchy nodes: <(,) and >(.) keys  (for RIDER and BIKE)
      Cycle through different lights: N (for LIGHTS)

Heirarchy Nodes: 

      Change positon: Press/hold H,J,K + (Shift)
      Change rotation: Press/hold U,I,O + (Shift)

Lights:

      Switch ON/OFF: /(?) key

Timeline:

      Save timeline: S
      Load timeline: L
      Add Keyframe at current frame: A 
      Delete Keyframe at current frame: D 
      Move up/down timeline(1 frame at a time): Press/hold [ ({) AND ] (}) keys
      Move up/down timeline(10 frame at a time): Press/hold [ ({) AND ] (}) + Shift keys
      Start/stop Play: P
      Toggle Record ON/OFF: R (will record whlie playing)


# Elements in scene :


* Cameras:

      1-scene camera
      2-first person camera
      3-third person camera

* Lights:

      1-2 point lights
      2-spotlight
      3-headlight 

* Track

* Heirarchy Nodes of Rider:

      1-hip; 2-torso; 3-neck;	4-head;

      5-upper_left_hand; 6-lower_left_hand; 7-upper_right_hand; 8-lower_right_hand; 

      9-upper_left_leg; 10-lower_left_leg; 11-upper_right_leg; 12-lower_right_leg;

      13-hair;  14-eye_left; 15-eye_right; 16-shoe_left; 17-shoe_right;

      18-glove_left; 19-glove_right; 20-mouth; 21-helmet;

* Heirarchy Nodes of Bike:

      1-engine; 2-handle; 3-handle_rod; 4-back_rod;

      5-wheel_front; 6-wheel_back; 7-seat; 8-tank;

      9-spoke_front1; 10-spoke_front2; 11-spoke_front3; 12-spoke_front4;

      13-spoke_back1; 14-spoke_back2; 15-spoke_back3; 16-spoke_back4;
