#include "Memory/TUniquePtr.h"
#include "DarkClient.h"
#include "SceneViewport.h"

#include "ViewportClient.h"





class FGameViewportClient : public FViewportClient
{
private:
	TSharedPtr<class UWorld> World;

public:
	FViewport* Viewport;

	FSceneViewport* CreateGameViewport();

	void SetViewport(FViewport* InViewport);

	virtual void Draw(FViewport* Viewport) override;

	virtual UWorld* GetWorld();

	FSceneViewport* GetGameViewport();

	//virtual void Init();







};