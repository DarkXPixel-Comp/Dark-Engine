#pragma once


#include "Templates/DarkTemplate.h"
#include "Containers/DarkString.h"




class FPaths
{
public:

	static CORE_API const FString& ProjectSavedDir();

	static CORE_API FString LaunchDir();

	static CORE_API FString EngineDir();

	static CORE_API FString ContentDir();

	static CORE_API FString EngineBinariesDir();

	static CORE_API FString EngineUserDir(); 

	static CORE_API FString EngineShaderDir();

	static CORE_API FString EngineContentDir();

	static CORE_API FString SourceDir();

	static CORE_API FString RootDir();

	static CORE_API FString ProjectDir();

	static CORE_API FString ProjectUserDir();

	static CORE_API FString ProjectContentDir();

	//static CORE_API FString ShaderDir

	static CORE_API FString CombineDir(FString Str1, FString Str2);
											  



};