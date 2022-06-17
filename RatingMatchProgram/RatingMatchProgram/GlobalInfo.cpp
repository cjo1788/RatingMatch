#include "GlobalInfo.h"
#include "CriticalSection.h"
#include "User.h"
#include "Room.h"
#include "Enum.h"

GlobalInfo::GlobalInfo()
{

}

GlobalInfo::~GlobalInfo()
{
	{
		SYNC(m_UserCS);

		for (auto itor = m_UserList.begin(); itor != m_UserList.end(); )
		{
			if (itor != m_UserList.end())
			{
				User* temp = (*itor);
				delete temp;
				itor = m_UserList.erase(itor);
			}
		}

		m_UserList.clear();
	}

	{
		SYNC(m_MatchCS);
		for (auto itor = m_MatchList.begin(); itor != m_MatchList.end(); )
		{
			if (itor != m_MatchList.end())
			{
				WorkInfo* temp = (*itor);
				delete temp;
				itor = m_MatchList.erase(itor);
			}
		}
	}

	{
		SYNC(m_UseRoomCS);
		for (auto itor = m_UseRoomList.begin(); itor != m_UseRoomList.end(); )
		{
			if (itor != m_UseRoomList.end())
			{
				Room* temp = (*itor);
				delete temp;
				itor = m_UseRoomList.erase(itor);
			}
		}
	}

	{
		SYNC(m_FreeRoomCS);
		for (auto itor = m_FreeRoomList.begin(); itor != m_FreeRoomList.end(); )
		{
			if (itor != m_FreeRoomList.end())
			{
				Room* temp = (*itor);
				delete temp;
				itor = m_FreeRoomList.erase(itor);
			}
		}
	}

	{
		SYNC(m_FreeMatchCS);
		for (auto itor = m_FreeMatchList.begin(); itor != m_FreeMatchList.end(); )
		{
			if (itor != m_FreeMatchList.end())
			{
				WorkInfo* temp = (*itor);
				delete temp;
				itor = m_FreeMatchList.erase(itor);
			}
		}
	}
}

void GlobalInfo::Initialize()
{
	srand(time(0));

	int sn = 0;
	int rating = 0;
	int win = 0;
	int lose = 0;

	SYNC( m_UserCS );
	// 유저 정보 랜덤 세팅
	for (int i = 0; i < 100; i++)
	{
		User* user = new User();
		sn = i;
		rating = rand() % (iRating_Max - iRating_Min) + iRating_Min;

		user->SetUserInfo(sn, rating, win, lose, UserState::READY);

		m_UserList.push_back(user);
	}

	printf("======= UserList ======\n");
	for (auto itor = m_UserList.begin(); itor != m_UserList.end(); ++itor )
	{
		printf("SN : %d, Rating : %d\n", (*itor)->GetSN(), (*itor)->GetRating());
	}
	printf("=======================\n");

	// Room 메모리 풀
	for (int i = 0; i < iMatch_Room_Count; i++)
	{
		Room* room = new Room();
		{
			CS(m_FreeRoomCS);
			m_FreeRoomList.push_back(room);
		}
	}

	// 매칭 메모리 풀
	for (int i = 0; i < iFree_Match_Count; i++)
	{
		WorkInfo* work = new WorkInfo();
		CS(m_FreeMatchCS);
		m_FreeMatchList.push_back(work);
	}
}

Room* GlobalInfo::CreateRoom()
{
	Room* pRoom = nullptr;
	{
		CS(m_FreeRoomCS);
		pRoom = m_FreeRoomList.front();

		if (pRoom == nullptr)
		{
			pRoom = new Room();
		}
		else
		{
			m_FreeRoomList.pop_front();
		}
	}

	return pRoom;
}

void GlobalInfo::InitRoom(Room* pRoom)
{
	Room* pUseRoom = pRoom;

	if( pUseRoom != nullptr)
	{
		{
			CS(m_UseRoomCS);
			for (auto itor = m_UseRoomList.begin(); itor != m_UseRoomList.end(); ++itor)
			{
				if (*(itor) == pUseRoom)
				{
					m_UseRoomList.erase(itor);
					break;
				}
				else
				{
					return;
				}
			}
		}

		pUseRoom->Initialize();

		{
			CS(m_FreeRoomCS);
			m_FreeRoomList.push_back(pUseRoom);
		}
	}
	else
	{
		return;
	}
}

WorkInfo* GlobalInfo::CreateWorkInfo()
{
	WorkInfo* pWork = nullptr;
	CS(m_FreeMatchCS);
	pWork = m_FreeMatchList.front();

	if (pWork == nullptr)
	{
		pWork = new WorkInfo();
	}
	else
	{
		m_FreeMatchList.pop_front();
	}

	return pWork;
}

using return_itor = typename list<WorkInfo*>::iterator;
return_itor GlobalInfo::InitWorkInfo(WorkInfo* work)
{
	WorkInfo* pUseWork = work;
	return_itor temp_itor = m_MatchList.end();

	if (pUseWork != nullptr)
	{
		{
			CS(m_MatchCS);
			for (auto itor = m_MatchList.begin(); itor != m_MatchList.end(); itor++)
			{
				if (*(itor) == pUseWork)
				{
					temp_itor = m_MatchList.erase(itor);
					break;
				}
			}
		}

		pUseWork->Initialize();

		{
			CS(m_FreeMatchCS);
			m_FreeMatchList.push_back(pUseWork);
		}
	}

	return temp_itor;
}

void GlobalInfo::SortMatchList()
{
	CS(m_MatchCS);
	m_MatchList.sort([](WorkInfo* first, WorkInfo* second) -> bool
	{
		return ( first->m_User->GetRating() < second->m_User->GetRating() ? true : false );
	} );
}

void GlobalInfo::PrintMatchList()
{
	CS(m_matchCS);
	for (const auto& match : m_MatchList)
	{
		printf("Rating : %d, User : %d\n", match->m_User->GetRating(), match->m_User->GetSN());
	}
}