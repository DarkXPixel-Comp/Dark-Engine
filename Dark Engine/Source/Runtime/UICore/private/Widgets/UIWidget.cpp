#include "Widgets/UIWidget.h"
#include "imgui.h"



ImVec4 FVectorToImVec4(const FVector& InVector)
{
	return ImVec4(InVector.X, InVector.Y, InVector.Z, 1);
}

void UIWidget::ForEachChild(std::function<void(UIWidget*)> Func)
{
	for (const auto& Widget : ChildWidgets)
	{
		Func(Widget.get());
	}
}
