#if defined GetRank
    #endinput
#endif
#define GetRank

#include <YSI_Coding\y_hooks>

static 
  db_admin[MAX_PLAYERS],
  db_owner[MAX_PLAYERS];

HOOK__ data_load(playerid, const string: name[], const string: value[])
{
    INI_Int("Admin", db_admin[playerid]);
    INI_Int("Owner", db_owner[playerid]);

    return 1;
}

stock GetPlayerRank(playerid)
{
    static rank_Name[46];

    if(db_owner[playerid]) rank_Name = "Vlasnik";
    
    else
    {
        if(db_admin[playerid] < 4) rank_Name = "Administrator";
        else if(db_admin[playerid] == 4) rank_Name = "Head Admin";
        else if(db_admin[playerid] == 5) rank_Name = "Direktor";
        else if(db_admin[playerid] == 6) rank_Name = "Menager";
        else if(db_admin[playerid] == 7) rank_Name = "Su-Vlasnik";
    }
    
    return rank_Name;
}