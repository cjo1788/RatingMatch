#pragma once
#include "Global.h"
#include "GlobalInfo.h"
#include "SingleTon.h"

class JoinThread
{
private:
	HANDLE m_event;
public:
	JoinThread();
	~JoinThread();

	void Initialize();
	int Run();
};

class JoinManager : public SingleTon<JoinManager>
{
private:
	JoinThread* joinThread;
	thread join_thread;

public:
	JoinManager();
	virtual ~JoinManager();

	void Initialize();

	void Run();
};

#define JOIN_MANAGER JoinManager::getInstance()