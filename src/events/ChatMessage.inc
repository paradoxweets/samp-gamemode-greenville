#if defined ChatMessage
    #endinput
#endif
#define ChatMessage

#include <YSI_Coding\y_hooks>

HOOK__ OnPlayerText(playerid, const string: text[])
{
    static fm_string[128];

    format(fm_string, sizeof(fm_string), ""c_greey"%s "c_lblue"[ ID:%d ] "c_greey"kaze: "c_white"%s", ReturnPlayerName(playerid), playerid, text);

	__proxy(playerid, Float:30.0, y_greey, fm_string);

    return 0;
}

YCMD:b(playerid, const string:params[], help) 
{
	if(isnull(params)) 
		return SendClientMessage(playerid, y_lblue, "( ? ) "c_white"/b [text]");

	static fm_string[256];

	format(fm_string, sizeof(fm_string), ""c_blue"( OOC | %s: "c_white"%s "c_blue")", ReturnPlayerName(playerid), params);

	__proxy(playerid, 30.0, -1, fm_string);
    
	return 1;
}

YCMD:d(playerid, const string:params[], help) 
{
	if(isnull(params)) 
		return SendClientMessage(playerid, y_lblue, "( ? ) "c_white"/d [text]");

	static fm_string[256];

	format(fm_string, sizeof(fm_string), ""c_orange"%s se dere: "c_white"%s", ReturnPlayerName(playerid), params);

	__proxy(playerid, 40.0, -1, fm_string);

	ApplyAnimation(playerid, "PED", "IDLE_CHAT", 4.0, 0, 0, 0, 0, 1000, 1);
	return 1;
}

YCMD:t(playerid, const string:params[], help) 
{
	if(isnull(params)) 
		return SendClientMessage(playerid, y_lblue, "( ? ) "c_white"/t [text]");

	static fm_string[256];

	format(fm_string, sizeof(fm_string), ""c_orange"%s kaze tiho: "c_white"%s", ReturnPlayerName(playerid), params);

	__proxy(playerid, 10.0, -1, fm_string);

	return 1;
}