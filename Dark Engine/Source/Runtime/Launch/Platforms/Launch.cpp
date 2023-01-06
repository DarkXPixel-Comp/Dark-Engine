#include <Runtime/Core/Core.h>


//temp
bool LoadContent();
bool UnLoadContent();


int32_t guardedMain()
{
//	int32_t ErrorLevel = EngineInit();
	

	//DEngine::SetFullScreenMode(true);
	

	/*if (ErrorLevel != 0)
	{
		return ErrorLevel;
	}*/

	while (!GEngine.isAppQuit())
	{
		//Windows::HandlerMessages();
		//mainWindow.Update();

		GEngine.UpdateLoop();
		//EngineTick();
	}
	
	//ErrorLevel = Clear();

	return 0;

}