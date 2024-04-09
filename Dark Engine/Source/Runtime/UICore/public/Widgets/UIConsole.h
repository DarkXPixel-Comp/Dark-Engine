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

private:
	int32 ScrollToBottom = 1;
	char InputText[256] = "";
	//TArray<struct FConsoleLog> Messages;




};