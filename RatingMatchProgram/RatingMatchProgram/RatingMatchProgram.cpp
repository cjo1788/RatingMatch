#include "Global.h"
#include "GlobalInfo.h"
#include "JoinManager.h"
#include "MatchingManager.h"
#include "InGameManager.h"

int main()
{
    // initialize
    GLOBALINFO->Initialize();
    JOIN_MANAGER->Initialize();
    MATCH_MANAGER->Initialize();
    INGAME_MANAGER->Initialize();
    
    // run
    JOIN_MANAGER->Run();
    MATCH_MANAGER->Run();
    INGAME_MANAGER->Run();

    return 0;
}