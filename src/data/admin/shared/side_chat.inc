#if defined side_chat_dat
    #endinput
#endif
#define side_chat_dat

#include <YSI_Coding\y_hooks>

static
    db_admin[MAX_PLAYERS];

HOOK__ data_load(playerid, const string: name[], const string: value[])
{
    INI_Int("Admin", db_admin[playerid]);

    return 1;
}
const max_lines = 8;

new bool:side_chat[MAX_PLAYERS];

static
    PlayerText: side_chat_TD[max_lines],
	side_chat_text[MAX_PLAYERS][max_lines][128];

HOOK__ OnPlayerConnect(playerid)
{
    side_chat[playerid] = true;

    new Float: pos_y = 229.573348,
		Float: spacint_Y = 240.026733 - 229.573348;

	for(new i = 0; i < max_lines; i ++)
 	{
		side_chat_TD[i] = CreatePlayerTextDraw(playerid, 35.599971, pos_y, " ");
		PlayerTextDrawLetterSize(playerid, side_chat_TD[i], 0.21, 0.9);
		PlayerTextDrawAlignment(playerid, side_chat_TD[i], 1);
		PlayerTextDrawColor(playerid, side_chat_TD[i], -1);
		PlayerTextDrawSetShadow(playerid, side_chat_TD[i], 0);
		PlayerTextDrawSetOutline(playerid, side_chat_TD[i], 1);
		PlayerTextDrawBackgroundColor(playerid, side_chat_TD[i], 255);
		PlayerTextDrawFont(playerid, side_chat_TD[i], 1);
		PlayerTextDrawSetProportional(playerid, side_chat_TD[i], 1);
		PlayerTextDrawSetShadow(playerid, side_chat_TD[i], 0);
 
		strmid(side_chat_text[playerid][i], " ", 0, 1);
		pos_y += spacint_Y;
	}
    return 1;
}

HOOK__ OnPlayerSpawn(playerid)
{
    if(db_admin[playerid] >= 1) 
    {
	    for(new i = 0; i < max_lines; i ++)
	    {
	    	PlayerTextDrawShow(playerid, side_chat_TD[i]);
	    }
    }
    return 1;
}
stock SideChatMessage(const message[]) 
{
	for(new i = 0; i < MAX_PLAYERS; i++) 
	{
		if(IsPlayerConnected(i)) 
		{
		    if(db_admin[i] >= 1 || IsPlayerAdmin(i)) 
		    {
				for(new a = 0; a < max_lines; a ++)
			 	{
				    if (a == max_lines - 1)
				 	{
				        strmid(side_chat_text[i][a], message, 0, strlen(message));
				        break;
				    }
					strmid(side_chat_text[i][a], side_chat_text[i][a + 1], 0, strlen(side_chat_text[i][a + 1]));
				}
			 
				for(new b = 0; b < max_lines; b ++)
			 	{
				    PlayerTextDrawSetString(i, side_chat_TD[b], side_chat_text[i][b]);
				}
			}
		}
	}
	return 1;
}
