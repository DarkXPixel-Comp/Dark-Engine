#pragma once
#include "Widgets/UIEditorViewport.h"
#include "Engine/LevelEditorViewport.h"



class UILevelViewport : public UIEditorViewport
{
public:
	void ConstructLevelEditorViewportClient();
	virtual ~UILevelViewport() = default;



private:
	TSharedPtr<FLevelEditorViewportClient> LevelViewportClient;
};