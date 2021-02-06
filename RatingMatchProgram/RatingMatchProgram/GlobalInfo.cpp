#include "GlobalInfo.h"
#include "CriticalSection.h"
#include "User.h"
#include "Room.h"

GlobalInfo::GlobalInfo()
{

}

GlobalInfo::~GlobalInfo()
{
	{
		SYNC(m_UserCS);
		list<User*>::iterator itor;
		for (itor = m_UserList.begin(); itor != m_UserList.end(); )
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
		list<WorkInfo*>::iterator itor;
		for (itor = m_MatchList.begin(); itor != m_MatchList.end(); )
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
}

void GlobalInfo::Initialize()
{
	srand(time(0));

	int sn = 0;
	int rating = 0;
	int win = 0;
	int lose = 0;

	SYNC( m_UserCS );
	for (int i = 0; i < 100; i++)
	{
		User* user = new User();
		sn = i;
		rating = rand() % ( DF_RATING_MAX - DF_RATING_MIN ) + DF_RATING_MIN;

		user->SetUserInfo(sn, rating, win, lose, UserState::READY);

		m_UserList.push_back(user);
	}

	printf("======= UserList ======\n");
	for (auto itor = m_UserList.begin(); itor != m_UserList.end(); ++itor )
	{
		printf("SN : %d, Rating : %d\n", (*itor)->GetSN(), (*itor)->GetRating());
	}
	printf("=======================\n");

	for (int i = 0; i < DF_MAX_ROOM_COUNT; i++)
	{
		Room* room = new Room();
		{
			CS(m_FreeRoomCS);
			m_FreeRoomList.push_back(room);
		}
	}
}

void GlobalInfo::Join()
{
	for (auto itor = m_UserList.begin(); itor != m_UserList.end(); ++itor)
	{
		int iRatingMax = 0;
		int iRatingMin = 0;
		int iRating = 0;
		int iUserRating = 0;

		User* pUser = (*itor);

		iUserRating = pUser->GetRating();

		WorkInfo* work = new WorkInfo();
		if (iUserRating - DF_MATCH_RATING_DEFAULT < 0)
		{
			iRatingMin = 0;
		}
		else
		{
			iRatingMin = iUserRating - DF_MATCH_RATING_DEFAULT;
		}

		iRating = iUserRating / 1000;

		iRatingMax = iUserRating + DF_MATCH_RATING_DEFAULT;

		work->m_User = pUser;
		work->m_MatchRatingMax = iRatingMax;
		work->m_MatchRatingMin = iRatingMin;
		work->m_dwMatchingTime = timeGetTime();

		{
			CS(m_MatchCS);
			m_MatchList.push_back(work);
		}
	}

	printf("match List====\n");
	for (auto itor : m_MatchList)
	{
		printf("%d %d\n", itor->m_User->GetSN(), itor->m_User->GetRating());
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
					// ???
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