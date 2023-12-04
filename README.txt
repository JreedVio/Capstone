Quantum Maze is a 3D, first person, multiplayer, puzzle game. The players have to solve a number of puzzles to complete the game. The game was made using a custom standalone C++ engine with Vulkan and SDL.

Github Link:
https://github.com/JreedVio/Capstone

Game Trailer:
YouTube: https://youtu.be/d-Y-lqMCalk

How to Run the Game:
The executable file is located in Build\ComponentFramework.exe
To test multiplayer, the game needs to be played in 2 instances on the same device. Use ip 127.0.0.1 to connect locally.

How to Find Code:
All code files and assets are located in the ComponentFramework folder.

How to Run Code in Visual Studio:
Most of the neccessary components are located in GameDev folder, except Vulkan SDK that has to be installed to C:\VulkanSDK\1.3.268.0 location with Include and Lib32 folders. Visual Studio has to be set to x86.

About the Team:
Vladimir Ianuskin - Networking
Ju-Chieh Liao - Dynamic FPS Camera, Advanced Asset Manager, Audio, UI
Sam Swintak - Physics, Collision Detection, Light
Stephen Mojar - Game Idea Creator

P.S. Every member has previously created individual Vulkan projects with implemention of Fragment and Vertex Shaders, Render Pipeline, Scene Manager, Static Camera, Asset Manager, Mesh Rendering and Texture mapping.

Dev Tools:
Networking: ENet, Cereal, TailScale
Audio: irrKlang
UI: IMGUI