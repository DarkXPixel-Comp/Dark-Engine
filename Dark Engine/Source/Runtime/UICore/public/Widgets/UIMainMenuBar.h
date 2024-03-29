#pragma once
#include "UIWidget.h"
#include "imgui.h"



class UIMainMenuBar : public UIWidget
{
public:
	UIMainMenuBar();

	virtual void DrawImGui();

	virtual void Update(float DeltaTime);

	void AddMenu(const TSharedPtr<class UIMenu>& InMenu);

	void SetIconTexture(TRefCountPtr<class FRHITexture> InIconTexture)
	{
		IconTextureRHI = InIconTexture;
	}

protected:
	void DrawMenuBar();
	
private:
	FString Title = TEXT("Dark Engine");
	TArray<TSharedPtr<class UIMenu>> MenuItems;
	TRefCountPtr<class FRHITexture> IconTextureRHI;





};