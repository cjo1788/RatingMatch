#include "MatchingManager.h"
#include "CriticalSection.h"
#include "GlobalInfo.h"
#include "User.h"
#include "Room.h"

MatchingThread::MatchingThread()
{
	m_event = INVALID_HANDLE_VALUE;
	Initialize();
}

MatchingThread::~MatchingThread()
{
	CloseHandle(m_event);
}

void MatchingThread::Initialize()
{
	m_event = CreateEvent(NULL, TRUE, FALSE, NULL);
}



int MatchingThread::Run()
{
	while (WaitForSingleObject(m_event, 500) != 0)
	{
		DWORD dwTime = timeGetTime();

#pragma region 리스트 사용
		{
			CS(GLOBALINFO->m_MatchCS);
			if (!GLOBALINFO->m_MatchList.empty())
			{
				BYTE byCheck = 0;
				WorkInfo* pWork1 = nullptr;
				WorkInfo* pWork2 = nullptr;

				// GLOBALINFO->m_MatchList.sort(&WorkInfo::Compare);

				auto itor = GLOBALINFO->m_MatchList.begin();
				auto itor2 = GLOBALINFO->m_MatchList.begin();

				for (; itor != GLOBALINFO->m_MatchList.end(); ++itor)
				{
					for (; itor2 != GLOBALINFO->m_MatchList.end(); ++itor2)
					{
						if (itor != itor2 && (*itor)->m_User != nullptr && (*itor2)->m_User != nullptr && (*itor)->m_User != (*itor2)->m_User)
						{
							DWORD dwRating = (*itor2)->m_User->GetRating();
							if (dwRating >= (*itor)->m_MatchRatingMin && dwRating <= (*itor)->m_MatchRatingMax)
							{
								pWork1 = (*itor);
								pWork2 = (*itor2);
								byCheck = 1;
								//printf("==============\n");
								//printf("매칭찾음!! UserSN1 : %d, RatingMax : %d, RatingMin : %d, Rating : %d\n", pWork1->m_User->GetSN(), pWork1->GetRatingMax(), pWork1->GetRatingMin(), pWork1->m_User->GetRating());
								//printf("매칭찾음!! UserSN2 : %d, RatingMax : %d, RatingMin : %d, Rating : %d\n", pWork2->m_User->GetSN(), pWork2->GetRatingMax(), pWork2->GetRatingMin(), pWork2->m_User->GetRating());
								//printf("==============\n");
								break;
							}
						}
					}
					if (byCheck == 1)
					{
						break;
					}
					else
					{
						//printf("매칭못찾음!! UserSN : %d, RatingMax : %d, RatingMin : %d, Rating : %d\n", (*itor)->m_User->GetSN(), (*itor)->GetRatingMax(), (*itor)->GetRatingMin(), (*itor)->m_User->GetRating());
						(*itor)->AddRatingMax(DF_MATCH_RATING_DEFAULT);
						if ((*itor)->GetRatingMin() > 10)
						{
							(*itor)->DisRatingMin(DF_MATCH_RATING_DEFAULT);
						}
					}
				}

				if (byCheck == 1)
				{
					//join
					Room* pRoom = nullptr;
					pRoom = GLOBALINFO->CreateRoom();
					pRoom->Enter((*itor)->m_User);
					pRoom->Enter((*itor2)->m_User);

					(*itor)->m_User->SetState(UserState::START);
					(*itor2)->m_User->SetState(UserState::START);

					{
						CS(GLOBALINFO->m_UseRoomCS);						
						GLOBALINFO->m_UseRoomList.push_back(pRoom);
					}

					pRoom->SetState(RoomState::ROOM_ENTER);

					// erase
					//printf("삭제!!! UserSN1 : %d, UserSN2 : %d\n", (*itor)->m_User->GetSN(), (*itor2)->m_User->GetSN());
					GLOBALINFO->InitWorkInfo(*itor);
					GLOBALINFO->InitWorkInfo(*itor2);
					//GLOBALINFO->m_MatchList.erase(itor);
					//GLOBALINFO->m_MatchList.erase(itor2);
				}
			}
		}
#pragma endregion 리스트 사용
	}

	return 0;
}

MatchingManager::MatchingManager()
{
	matchThread = NULL;
}

MatchingManager::~MatchingManager()
{
	delete matchThread;
	matchThread = NULL;
}

void MatchingManager::Initialize()
{
	matchThread = new MatchingThread();
	match_thread = thread(&MatchingThread::Run, matchThread);
}

void MatchingManager::Run()
{
	match_thread.join();
}

