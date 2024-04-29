#pragma once





class FConfigCache
{
public:
	static void InitConfigSystem();


	static bool InitConfigFiles();
	


private:
	FConfigCache();

private:
	bool bReady = false;



};