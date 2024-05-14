#include "CoreGlobals.h"
#include "Misc/Config.h"


bool GIsRequestingExit = false;
bool GIsTitleBarHovered = false;
FConfigCache* GConfig = nullptr;
int32 GRenderMode = 0;
float GMaxFPS = 60.f;
float GNearClippingPlane = 10.f;
bool GFXAAEnable = false;
