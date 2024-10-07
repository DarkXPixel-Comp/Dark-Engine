#pragma once
#include "UIWidget.h"
#include "imgui.h"



class UIMainMenuBar : public UIWidget
{
public:
	UIMainMenuBar();
	virtual ~UIMainMenuBar() = default;

	virtual void DrawImGui();

	virtual void Update(float DeltaTime);

	void AddMenu(const TSharedPtr<class UIMenu>& InMenu);

	void SetIconTexture(TRefCountPtr<class FRHITexture> InIconTexture)
	{
		IconTextureRHI = InIconTexture;
	}

	void SetTitle(FString InTitle) { Title = InTitle; }

protected:
	void DrawMenuBar();
	//void FastLoadIcon(TRefCountPtr<class FRHITexture>& InTexture, FString Path);
	//void FastLoadIcon(TRefCountPtr<class FRHITexture>& InTexture, uint8* Bytes, uint64 SizeBytes);
	
	
private:
	FString Title;
	TArray<TSharedPtr<class UIMenu>> MenuItems;
	TRefCountPtr<class FRHITexture> IconTextureRHI;
	TRefCountPtr<class FRHITexture> MaximizeTextureRHI;
	TRefCountPtr<class FRHITexture> CloseTextureRHI;
	TRefCountPtr<class FRHITexture> Close2TextureRHI;
	TRefCountPtr<class FRHITexture> MinimizeTextureRHI;
	TRefCountPtr<class FRHITexture> RestoreTextureRHI;

	bool RequestMaximizeOrRestore = false;
	bool RequestMinimize = false;






};