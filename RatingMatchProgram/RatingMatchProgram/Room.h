#pragma once
#include "Global.h"

class User;
enum class RoomState;

struct Slot
{
	User* m_pUser;
	BYTE m_byWinCount;
};

class Room
{
private:
	// user
	Slot m_Slot[2];
	DWORD m_dwTime;
	RoomState m_byState;
	BYTE m_byRound;

public:
	Room();
	~Room();

	void Initialize();

	void SetState(RoomState byState);
	RoomState GetState();

	void Enter(User* pUser);
	void PVP();
	void PVPResult();

	void EnterProcess();
	void ReadyProcess();
	void StartProcess();
	void EndProcess();
};

