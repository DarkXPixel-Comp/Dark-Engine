#pragma once
#include "GenericPlatform/GenericWindow.h"
#include "GenericPlatform/GenericWindowDefinition.h"
#include "Memory/MemoryCore.h"



class FGenericApplication
{
public:
	virtual ~FGenericApplication() {}

	virtual TSharedPtr<FGenericWindow> MakeWindow()
	{
		return MakeShareble(new FGenericWindow());
	}

	virtual void InitializeWindow(const TSharedPtr<FGenericWindow>& InWindow, const TSharedPtr<FGenericWindowDefinition>& InDefinition) {}
	virtual void PumpMessages() {}
	virtual void Tick(float DeltaTime) {}
	//virtual void SetMessageHandler(const )
	//virtual void GetMousePos() const {return }

protected:



};