#pragma once
#include "Math/IntRect.h"
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
		++CounterID;
	}
	virtual ~UIWidget()
	{
		DE_LOG(UICoreLog, Log, TEXT("Destroy %s"), *Name);
	}
	virtual void Update(float DeltaTime);
	virtual void DrawImGui() {}

	void FastLoadIcon(TRefCountPtr<class FRHITexture>& InTexture, FString Path);
	
	void SetSize(FIntPoint InSize) { Size = InSize; }
	void SetPostion(FIntPoint InPosition) { Position = InPosition; }
	FString	GetName() const { return Name; }
	void SetName(FString InName) { Name = InName; }
	void AddChild(TSharedPtr<UIWidget> Child)
	{ 
		if (AllWidgets.find(Child->Name) != AllWidgets.end())
		{
			Child->SetName(FString::PrintF(TEXT("%s##%ull"), *Child->Name, CounterID));
		}
		Child->Window = Window;
		Child->Owner = this;
		Child->bOpen = true;
		AllWidgets.emplace(Child->Name, Child.get());
		ChildWidgets.Add(Child);
	}
	void RemoveChild(TSharedPtr<UIWidget> Child)
	{
		Child->Window = nullptr;
		Child->Owner = nullptr;
		ChildWidgets.Remove(Child);
	}

	void DefferedRemoveChild(UIWidget* Child)
	{
		DefferedRemoved.Add(Child);
	}

	bool IsOpen() const
	{
		return bOpen;
	}

	void CloseWidget();

	void SetOpen(bool InbOpen)
	{
		bOpen = InbOpen;
	}


	//operator int() const;

	FIntRect GetRect() { return Rect; }
	UIWindow* Window = nullptr;
	UIWidget* Owner = nullptr;
	bool bHaveCloseButton = false;
 
protected:
	typedef UIWidget Super;


protected:
	//void ForEachChild(void(*Func)(UIWidget*));
	void ForEachChild(std::function<void(UIWidget*)> Func);
	bool* IsWidgetClose()
	{
		return bHaveCloseButton ? bOpen ? &bOpen : nullptr : nullptr;
	}


private:
	void InitWindow(UIWindow* InWindow) { Window = InWindow;  }
	TArray<UIWidget*> DefferedRemoved;

	static uint64 CounterID;

protected:
	FIntPoint Position;
	FIntRect Rect;
	FIntPoint Size;
	TArray<TSharedPtr<UIWidget>> ChildWidgets;
	FString Name;
	uint64 Index = 0;
	bool bOpen = true;
	static TUnordoredMap<FString, UIWidget*> AllWidgets;
};
