#pragma once



class IModule
{
public:
	virtual ~IModule() {}

	virtual void StartModule() {}

	virtual void ShutdownModule() {}
};