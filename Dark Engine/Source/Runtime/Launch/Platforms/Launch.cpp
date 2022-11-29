#include <Runtime/Core/Core.h>


//temp
bool LoadContent();
bool UnLoadContent();


int32_t EngineInit()
{
	int32_t ErrorLevel = 0;
	/*if (!graphics::initialize(graphics::graphic_platform::d3d12))
	{
		ErrorLevel = 1;
	}*/


	return ErrorLevel;
}

int32_t Clear()
{
	//graphics::shutdown();

	//Logger::log("System succsefully shutdown", LOGGER_INFO);
	//Logger::log("", LOGGER_INFO);
	
	GEngine.Shutdown();


	return 0;
}



int32_t guardedMain()
{
	int32_t ErrorLevel = EngineInit();
	

	//DEngine::SetFullScreenMode(true);
	

	if (ErrorLevel != 0)
	{
		return ErrorLevel;
	}

	while (!GEngine.isAppQuit())
	{
		//Windows::HandlerMessages();
		//mainWindow.Update();

		GEngine.UpdateLoop();
		//EngineTick();
	}
	
	ErrorLevel = Clear();

	return 0;

}