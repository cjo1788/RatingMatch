#pragma once
#include "Global.h"

enum class UserState;

class User
{
private:
	DWORD m_dwSN;
	DWORD m_dwRating;
	DWORD m_dwWin;
	DWORD m_dwLose;
	UserState m_byState;

public:
	User();
	~User();

	void GetUserInfo( DWORD& dwSN, DWORD& dwRating, DWORD& dwWin, DWORD& dwLose, UserState& byState);
	void SetUserInfo(DWORD dwSN, DWORD dwRating, DWORD dwWin, DWORD dwLose, UserState byState);

	DWORD GetSN();
	DWORD GetRating();
	DWORD GetWin();
	DWORD GetLose();
	UserState GetState();

	void PVPWin();
	void PVPLose();

	void SetState(UserState byState);
};

