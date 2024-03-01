#pragma once
#include "DarkClient.h"
#include "RenderingCommon.h"
#include "RHIResources.h"


class FSceneViewport : public FViewport, public IUIViewport
{



private:
	TRefCountPtr<FRHIViewport> ViewportRHI;
};