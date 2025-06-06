#pragma once
#include "Rendering/DrawElementTypes.h"



static bool ShouldCull(const FDUIWindowElementList& ElementList)
{
	return false;


}


void FDUIDrawElement::MakeDebugQuad(FDUIWindowElementList& ElementList, uint32 InLayer, const FPaintGeometry& PaintGeometry, FLinearColor Tint)
{
	if (ShouldCull(ElementList))
	{
		return;
	}


}
