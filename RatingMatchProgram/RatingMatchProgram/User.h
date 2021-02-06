#pragma once
#include "Global.h"

enum UserState
{
	NONE = 0,
	READY = 1,
	MATCHING = 2,
	START = 3,
};

class User
{
private:
	DWORD m_dwSN;
	DWORD m_dwRating;
	DWORD m_dwWin;
	DWORD m_dwLose;
	BYTE m_byState;

public:
	User();
	~User();

	void GetUserInfo( DWORD& dwSN, DWORD& dwRating, DWORD& dwWin, DWORD& dwLose, BYTE& byState);
	void SetUserInfo(DWORD dwSN, DWORD dwRating, DWORD dwWin, DWORD dwLose, BYTE byState);

	DWORD GetSN();
	DWORD GetRating();
	DWORD GetWin();
	DWORD GetLose();
	BYTE GetState();

	void PVPWin();
	void PVPLose();

	void SetState(BYTE byState);
};

