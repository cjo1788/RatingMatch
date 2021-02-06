#include "User.h"

User::User()
{
	m_dwSN = 0;
	m_dwRating = 0;
	m_dwWin = 0;
	m_dwLose = 0;
	m_byState = 0;
}

User::~User()
{

}

void User::GetUserInfo(DWORD& dwSN, DWORD& dwRating, DWORD& dwWin, DWORD& dwLose, BYTE& byState)
{
	dwSN = m_dwSN;
	dwRating = m_dwRating;
	dwWin = m_dwWin;
	dwLose = m_dwLose;
	byState = m_byState;
}

void User::SetUserInfo(DWORD dwSN, DWORD dwRating, DWORD dwWin, DWORD dwLose, BYTE byState)
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

BYTE User::GetState()
{
	return m_byState;
}

void User::SetState(BYTE byState)
{
	m_byState = byState;
}

void User::PVPWin()
{
	m_dwWin += 1;
	m_dwRating += DF_MATCH_RATING_DEFAULT;
}

void User::PVPLose()
{
	m_dwLose += 1;
	if (m_dwRating - DF_MATCH_RATING_DEFAULT < DF_RATING_MIN)
		m_dwRating = DF_RATING_MIN;
	else
		m_dwRating -= DF_MATCH_RATING_DEFAULT;
}