#include "JoinManager.h"
#include "GlobalInfo.h"
#include "User.h"

JoinThread::JoinThread()
{
	m_event = INVALID_HANDLE_VALUE;
	Initialize();
}

JoinThread::~JoinThread()
{
	CloseHandle(m_event);
}

void JoinThread::Initialize()
{
	m_event = CreateEvent(NULL, TRUE, FALSE, NULL);
}

int JoinThread::Run()
{
	while (WaitForSingleObject(m_event, 500) != 0)
	{
		CS(GLOBALINFO->m_UserCS);

		for (auto itor = GLOBALINFO->m_UserList.begin(); itor != GLOBALINFO->m_UserList.end(); ++itor)
		{
			User* pUser = (*itor);

			if (pUser->GetState() == READY)
			{
				int iRatingMax = 0;
				int iRatingMin = 0;
				int iRating = 0;
				int iUserRating = 0;

				iUserRating = pUser->GetRating();

				WorkInfo* work = GLOBALINFO->CreateWorkInfo();

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

				pUser->SetState(UserState::MATCHING);
				work->m_User = pUser;
				work->m_MatchRatingMax = iRatingMax;
				work->m_MatchRatingMin = iRatingMin;
				work->m_dwMatchingTime = timeGetTime();

				{
					CS(GLOBALINFO->m_MatchCS);
					GLOBALINFO->m_MatchList.push_back(work);
				}
			}
		}
	}

	return 0;
}

JoinManager::JoinManager()
{
	joinThread = NULL;
}

JoinManager::~JoinManager()
{
	delete joinThread;
	joinThread = NULL;
}

void JoinManager::Initialize()
{
	joinThread = new JoinThread();
	join_thread = thread(&JoinThread::Run, joinThread);
}

void JoinManager::Run()
{
	join_thread.join();
}