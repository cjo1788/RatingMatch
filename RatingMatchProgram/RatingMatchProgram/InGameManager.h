#pragma once
#include "Global.h"
#include "GlobalInfo.h"
#include "SingleTon.h"

class InGameThread
{
private:
	HANDLE m_event;
public:
	InGameThread();
	~InGameThread();

	void Initialize();
	int Run();
};

class InGameManager : public SingleTon<InGameManager>
{
private:
	InGameThread* ingameThread;
	thread ingame_thread;

public:
	InGameManager();
	virtual ~InGameManager();

	void Initialize();

	void Run();
};

#define INGAME_MANAGER InGameManager::getInstance()