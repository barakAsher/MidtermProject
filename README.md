# MP - The Fellowship
## Land: Rangi
## Names: Eddie Voss - Eragon, Ben Hempelmann - Gengiben, Barak Asher - Starlord, Porter Fredrickson - Peapodpuffer

Description: 

This program creates a 3D graphics environment with 4 animated 3D players. The environment has multiple stationary 
objects which give the user a better perspective of the word in comparison to the players. The program also contains
functionality for 3 different camera models one of which makes used of a frame in frame viewport. Specifically, the program 
has an arcBall camera, a free camera, and a viewport first person camera. Additionally, the program makes use of VAO's for
all objects and has custom shaders for the environment. Specifically the shaders implemented form a full Phong Reflectance 
model using Gouraud shading. The environment contains one directional light and one spotlight which showcase additive lighting
and attenuation and the lights are colored. 

For grading, there is one of each type of light in the scene. The purple light in the center is the spotlight, the white light
that is off to the side is a point light, and the directional light is dim but adds a yellowish color to all of the objects.

Usage: 

Forward movement: W key or Up arrow key
Backward movement: S key or down arrow
Rotate Character left: A or left arrow key
Rotate Character right: D or right arrow key

ArcBall Camera selection: Press 1
    Use the mouse + left click to drag the ArcBall around the look at point

Free Camera selection: Press 2
    Press the space bar to move Camera forward 
    Press Shift + Space to move the camera backward
    Use the mouse + left click to drag and change the free cams heading

If in ArcBall view, press 3 to make first person viewport camera appear in the top right.

Change Characters: Press the control key to cycle between which character the camera follows. 

Compilation: 

Download the MP zip file. Unzip. All source code can be compiled and ran with cmake in CLION or another IDE.






Implementation details / bugs: 

The alpha value is the same throughout the program and is only set once. We have the ability to have up to 10 of each type of light (arbitrarily set in the vertex shader )
and all you have to do to add more lights is push a new light object to the corresponding array in the MPEngine class
(in the setup lights function). We added a teapot to better see the lighting types since the lighting looks a 
bit weird on cubes with only a few vertices.




Distribution of responsibilities:
* Eddie: Set up the players and the abstract player class. Did a lot of work with moving the players around the scene and setting up controls for the program.
* Ben: Worked on implementing he different light types. Built out the structure for the vertex shader and spent a lot of time with setting up uniforms in an abstractable way for the lights  
* Porter: Set up the cameras and implemented the logic for moving the cameras around the scene with the players.
* Barak: Worked on the Phong Shading model and general lighting implementation in both the vertex shader and main engine class

Time: ~20 hours together

Lab Usefulness: 

10/10. The lab and assignment were great starting points for this project and we felt as though they both helped us understand the project

Lab Enjoyment:

9/10. Implementing a full lighting system was a lot of fun, and we enjoyed it quite a bit.




