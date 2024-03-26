#pragma once
#include "Math/MathFwd.h"
#include "Containers/Array.h"
#include "Memory/TUniquePtr.h"
#include <functional>
#include "UICommon.h"
//#include "UIWindow.h"

class UIWindow;
struct ImVec4;


ImVec4 FVectorToImVec4(const FVector& InVector);

class UIWidget
{
	friend class UIWindow;
public:
	UIWidget(FString InName = TEXT("UIWidget")):
		Name(InName)
	{
		DE_LOG(UICoreLog, Log, TEXT("Create %s"), *Name);
	}
	virtual void Update(float DeltaTime)
	{
		ForEachChild([DeltaTime](UIWidget* Widget)
			{
				Widget->Update(DeltaTime);
			});
	}
	virtual void DrawImGui() {}
	
	void SetSize(FIntPoint InSize) { Size = InSize; }
	void SetPostion(FIntPoint InPosition) { Position = InPosition; }
	FString	GetName() const { return Name; }
	void SetName(FString InName) { Name = InName; }
	void AddChild(TSharedPtr<UIWidget> Child) { Child->Owner = Owner; ChildWidgets.Add(Child); }
	FIntRect GetRect() { return Rect; }

	UIWindow* Owner;

protected:
	//void ForEachChild(void(*Func)(UIWidget*));
	void ForEachChild(std::function<void(UIWidget*)> Func);

private:
	void InitWindow(UIWindow* InWindow) { Owner = InWindow;  }

protected:
	FIntPoint Position;
	FIntRect Rect;
	FIntPoint Size;
	TArray<TSharedPtr<UIWidget>> ChildWidgets;
	FString Name;
	uint64 Index = 0;
	bool bHaveExitButton = false;

};