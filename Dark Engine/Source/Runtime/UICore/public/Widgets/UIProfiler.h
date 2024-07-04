#pragma once
#include "UIWidget.h"
#include "Containers/DarkString.h"
#include "Containers/Array.h"
#include "HAL/Process.h"



class UIProfiler : public UIWidget
{
public:
	UIProfiler();
	virtual ~UIProfiler() noexcept;
	

	virtual void DrawImGui() override;
	virtual void Update(float DeltaTime) override;

protected:
	void StopAndSaveCapture();
	void OpenProfiler();

private:
	char Buffer[256];
	bool bInCapture = false;
	
	TSharedPtr<FProcess> ProfilerProcess;
};