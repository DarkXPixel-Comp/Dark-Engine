#pragma once
#include "Memory/MemoryCore.h"
#include "Widgets/DUIControlConstruction.h"


class DUIWidget	: 	
	public FDUIControlledConstruction,
	public std::enable_shared_from_this<DUIWidget>
{
	DUI_DECLARE_WIDGET_API(DUIWidget, FDUIControlledConstruction, DarkUI_API);
public:
	//DarkUI_API int32 Paint(const )




protected:





};