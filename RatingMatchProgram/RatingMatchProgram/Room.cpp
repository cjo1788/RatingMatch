#include "Room.h"
#include "User.h"
#include "Enum.h"

Room::Room()
{
	Initialize();
}

Room::~Room()
{

}

void Room::Initialize()
{
	for (int i = 0; i < 2; i++)
	{
		m_Slot[i].m_pUser = nullptr;
		m_Slot[i].m_byWinCount = 0;
	}
	m_dwTime = 0;
	m_byState = RoomState::ROOM_NONE;
	m_byRound = 0;
}

void Room::SetState(RoomState byState)
{
	m_byState = byState;
}

RoomState Room::GetState()
{
	return m_byState;
}

void Room::Enter(User* pUser)
{
	for (BYTE i = 0; i < 2; i++)
	{
		if (m_Slot[i].m_pUser == nullptr)
		{
			m_Slot[i].m_pUser = pUser;
			break;
		}
	}
}

void Room::EnterProcess()
{
	// printf("ENTER Room %p\n", this);
	m_dwTime = timeGetTime();
	m_byState = RoomState::ROOM_READY;
}

void Room::ReadyProcess()
{
	// printf("READY Room %p\n", this);
	DWORD dwTime = timeGetTime();

	if (dwTime - m_dwTime > 3000)
	{
		m_byState = RoomState::ROOM_STARTING;
		m_dwTime = timeGetTime();
	}
}

void Room::StartProcess()
{
	// printf("START Room %p, Round : %d\n", this, m_byRound);
	if (m_byRound == 2)
	{
		PVPResult();
		m_byState = RoomState::ROOM_END;
		return;
	}

	DWORD dwTime = timeGetTime();
	if (dwTime - m_dwTime > 5000)
	{
		PVP();
		m_byRound++;
		m_dwTime = timeGetTime();
	}
}

void Room::EndProcess()
{
	printf("END Room %p\n", this);
	for (int i = 0; i < 2; i++)
	{
		if (m_Slot[i].m_pUser != nullptr)
		{
			m_Slot[i].m_pUser->SetState(UserState::READY);
		}
	}

	Initialize();

	m_byState = RoomState::ROOM_OVER;
}

void Room::PVP()
{
	int iValue = rand() % 2;
	if (iValue == 0)
		m_Slot[0].m_byWinCount += 1;
	else
		m_Slot[1].m_byWinCount += 1;
}

void Room::PVPResult()
{
	printf("PVPResult Start =================\n");


	if (m_Slot[0].m_byWinCount > m_Slot[1].m_byWinCount)
	{
		m_Slot[0].m_pUser->PVPWin();
		m_Slot[1].m_pUser->PVPLose();

		printf("WinUserSN : %d, WinCount : %d, LoseCount : %d, Rating : %d\n", m_Slot[0].m_pUser->GetSN(), m_Slot[0].m_pUser->GetWin(), m_Slot[0].m_pUser->GetLose(), m_Slot[0].m_pUser->GetRating());
		printf("LoseUserSN : %d, WinCount : %d, LoseCount : %d, Rating : %d\n", m_Slot[1].m_pUser->GetSN(), m_Slot[1].m_pUser->GetWin(), m_Slot[1].m_pUser->GetLose(), m_Slot[1].m_pUser->GetRating());
	}
	else
	{
		m_Slot[0].m_pUser->PVPLose();
		m_Slot[1].m_pUser->PVPWin();

		printf("WinUserSN : %d, WinCount : %d, LoseCount : %d, Rating : %d\n", m_Slot[1].m_pUser->GetSN(), m_Slot[1].m_pUser->GetWin(), m_Slot[1].m_pUser->GetLose(), m_Slot[1].m_pUser->GetRating());
		printf("LoseUserSN : %d, WinCount : %d, LoseCount : %d, Rating : %d\n", m_Slot[0].m_pUser->GetSN(), m_Slot[0].m_pUser->GetWin(), m_Slot[0].m_pUser->GetLose(), m_Slot[0].m_pUser->GetRating());
	}
	
	printf("PVPResult End =================\n");
}