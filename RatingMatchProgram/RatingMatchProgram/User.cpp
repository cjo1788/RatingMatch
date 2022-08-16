#include "User.h"
#include "Enum.h"

User::User()
{
	m_dwSN = 0;
	m_dwRating = 0;
	m_dwWin = 0;
	m_dwLose = 0;
	m_byState = UserState::NONE;
}

User::~User()
{

}

void User::GetUserInfo(DWORD& dwSN, DWORD& dwRating, DWORD& dwWin, DWORD& dwLose, UserState& byState)
{
	dwSN = m_dwSN;
	dwRating = m_dwRating;
	dwWin = m_dwWin;
	dwLose = m_dwLose;
	byState = m_byState;
}

void User::SetUserInfo(DWORD dwSN, DWORD dwRating, DWORD dwWin, DWORD dwLose, UserState byState)
{
	m_dwSN = dwSN;
	m_dwRating = dwRating;
	m_dwWin = dwWin;
	m_dwLose = dwLose;
	m_byState = byState;
}

DWORD User::GetSN()
{
	return m_dwSN;
}

DWORD User::GetRating()
{
	return m_dwRating;
}

DWORD User::GetWin()
{
	return m_dwWin;
}

DWORD User::GetLose()
{
	return m_dwLose;
}

UserState User::GetState()
{
	return m_byState;
}

void User::SetState(UserState byState)
{
	m_byState = byState;
}

void User::PVPWin()
{
	m_dwWin += 1;
	m_dwRating += iMatch_Rating_Default;
}

void User::PVPLose()
{
	m_dwLose += 1;
	if (m_dwRating - iMatch_Rating_Default < iRating_Min)
		m_dwRating = iRating_Min;
	else
		m_dwRating -= iMatch_Rating_Default;
}