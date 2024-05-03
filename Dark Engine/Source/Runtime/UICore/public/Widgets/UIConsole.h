#pragma once
#include "UIWidget.h"
#include "Containers/DarkString.h"
#include "Containers/Array.h"



class UIConsole : public UIWidget
{
public:
	UIConsole();

	void DrawImGui();
	void OnLogAdd(struct FConsoleLog InLog);

	TArray<FString>	History;
	int32 CurrentIndex = 0;

	int32 CompletionState = 0;
	bool CompletionDirty = true;
	TArray<FString>	CompletionBuf;

private:
	int32 ScrollToBottom = 1;
	char InputText[256] = "";
	
	//TArray<struct FConsoleLog> Messages;





};