#pragma once
#include "Global.h"
#include "GlobalInfo.h"
#include "SingleTon.h"



class MatchingThread
{
private:
	HANDLE m_event;
public:
	MatchingThread();
	~MatchingThread();

	void Initialize();
	int Run();
};

class MatchingManager: public SingleTon< MatchingManager >
{
private:
	MatchingThread* matchThread;
	thread match_thread;
public:
	MatchingManager();
	virtual ~MatchingManager();

	void Initialize();

	void Run();
};

#define MATCH_MANAGER MatchingManager::getInstance()