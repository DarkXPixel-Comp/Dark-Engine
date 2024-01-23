#include "DarkClient.h"

class FViewportClient
{
public:
	FViewportClient() {}
	virtual ~FViewportClient() {}

	virtual void RedrawRequested(FViewport* Viewport);
	virtual void Draw(FViewport* Viewport);

	virtual bool InputKey();


	




 };