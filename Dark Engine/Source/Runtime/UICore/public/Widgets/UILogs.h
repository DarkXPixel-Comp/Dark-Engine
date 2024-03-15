#pragma once
#include "UIWidget.h"
#include "Containers/DarkString.h"



class UILogs : public UIWidget
{
public:
	UILogs();

	void DrawImGui();
	void OnLogAdd();

private:
	uint64 LastLogsSize = 0;
	bool ScrollToBottom = true;



};