#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>
#include <string>
#include "SceneManager.h"
#include "Debug.h"
#include "MMath.h"
#include "Utilities.h"

using namespace MATH;
using namespace Utilities;
  
int main(int argc, char* args[]) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	{
		std::string name = { "Graphics_Game_Engine" };
		Debug::DebugInit(name + "_Log");
		Debug::Info("Starting the GameSceneManager", __FILE__, __LINE__);

		SceneManager* gsm = SceneManager::GetInstance();
		if (gsm->Initialize(name, 1280, 720) == true) {
			gsm->Run();
		}
		delete gsm;
		Memory::PrintTotalMemoryAndAllocations();		
		
	}	

	exit(0);
}