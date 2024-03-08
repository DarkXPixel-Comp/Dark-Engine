#pragma once
#include "DarkClient.h"



class FCanvas
{	
public:
	static FCanvas* Create();

public:
	void Flush();

private:
	FRenderTarget* RenderTarget;


};