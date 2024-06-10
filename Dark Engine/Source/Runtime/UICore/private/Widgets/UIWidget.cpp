#include "Widgets/UIWidget.h"
#include "imgui.h"
#include "Widgets/UIWindow.h"

TUnordoredMap<FString, UIWidget*> UIWidget::AllWidgets;
uint64 UIWidget::CounterID = 0;

ImVec4 FVectorToImVec4(const FVector& InVector)
{
	return ImVec4(InVector.X, InVector.Y, InVector.Z, 1);
}

const ImVec4 FVector3fToImVec4(const FVector3f& InVector)
{
	return ImVec4(InVector.X, InVector.Y, InVector.Z, 1);
}


void UIWidget::CloseWidget()
{
	AllWidgets.erase(Name);

	if(Owner)
		Owner->DefferedRemoveChild(this);
	//Window->RemoveWidget(this);
	Window = nullptr;
	Owner = nullptr;
}


void UIWidget::Update(float DeltaTime)
{
	for (auto& i : DefferedRemoved)
	{
		ChildWidgets.RemovePtr(i);
	}
	DefferedRemoved.Empty();

	ForEachChild([DeltaTime](UIWidget* Widget)
		{
			Widget->Update(DeltaTime);
		});

	if (bHaveCloseButton && !bOpen)
	{
		CloseWidget();
	}
}

void UIWidget::ForEachChild(std::function<void(UIWidget*)> Func)
{
	for (const auto& Widget : ChildWidgets)
	{
		Func(Widget.get());
	}
}
