#include "Widgets/DNullWidget.h"
#include "Widgets/DWidget.h"



class DARKUI_API DUINullWidgetContent : public DUIWidget
{
public:
	DarkUI_BEGIN_ARGS(DUINullWidgetContent)
	{
		_Visibility = EVisibility::SelfHitTestInvisible;
	}
	DarkUI_END_ARGS()

	void Construct(const FArguments& InArgs) {}



private:
	virtual void SetVisibility(TAttribute<EVisibility> InVisibility) override 
	{

	}
};

TSharedPtr<DUIWidget> NullWidgetConstruct()
{
	static TSharedPtr<DUIWidget> Result = DUINew(DUINullWidgetContent).Visibility(EVisibility::Hidden);
	return Result;
}

DARKUI_API TSharedPtr<class DUIWidget> DUINullWidget::NullWidget = NullWidgetConstruct();
