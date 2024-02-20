#include "DarkClient.h"


void FViewport::Draw()
{
	FWorld* World = GetClient()->GetWorld();


	ViewportClient->Draw(this, World);













}