#include "Widgets/UIWidget.h"



void UIWidget::ForEachChild(std::function<void(UIWidget*)> Func)
{
	for (const auto& Widget : ChildWidgets)
	{
		Func(Widget.get());
	}
}
