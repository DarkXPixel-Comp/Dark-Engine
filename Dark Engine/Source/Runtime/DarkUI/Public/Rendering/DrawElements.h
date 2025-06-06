#pragma once
#include "Widgets/DWindow.h"


class FDUIWindowElementList
{
public:
	FDUIWindowElementList(const TSharedPtr<DUIWindow>& InPaintWindow);


	TSharedPtr<DUIWindow> GetPaintWindow() const;

	TSharedPtr<DUIWindow> GetRenderWindow() const;





};