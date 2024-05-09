#pragma once

class DEdEngine
{
public:
	virtual ~DEdEngine() = default;
	virtual void Tick(float DeltaTime) = 0;
	virtual void AddViewportClient(class FViewportClient*) = 0;
	virtual void Shutdown() = 0;




};

extern DEdEngine* Engine;
