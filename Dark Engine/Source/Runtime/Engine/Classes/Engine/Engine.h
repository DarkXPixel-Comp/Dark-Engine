#pragma once

class DEdEngine
{
public:
	virtual void Tick(float DeltaTime) = 0;
	virtual void AddViewportClient(class FViewportClient*) = 0;



};

extern DEdEngine* Engine;
