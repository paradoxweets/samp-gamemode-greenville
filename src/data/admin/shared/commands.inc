#if defined admin_cmd
    #endinput
#endif
#define admin_cmd

#include <YSI_Coding\y_hooks>

static
    db_admin[MAX_PLAYERS],
    db_skin[MAX_PLAYERS],
    db_owner[MAX_PLAYERS],
    db_duty[MAX_PLAYERS],
	  db_supporter[MAX_PLAYERS];

new bool:admin_Duty[MAX_PLAYERS];
new admin_Vehicle[MAX_PLAYERS] = { INVALID_VEHICLE_ID, ... };

timer DutyTimer[60000](playerid)
{
	if(admin_Duty[playerid])
	{
		db_duty[playerid]++;
		defer DutyTimer(playerid);
	}
    return 1;
}

timer KickTimer[500](playerid)
{
  Kick(playerid);
  return 1;
}
HOOK__ data_load(playerid, const string: name[], const string: value[])
{
    INI_Int("Admin", db_admin[playerid]);
	INI_Int("Skin", db_skin[playerid]);
    INI_Int("Owner", db_owner[playerid]);
    INI_Int("DutyTime", db_duty[playerid]);
	INI_Int("Supporter", db_supporter[playerid]);

    return 1;
}

HOOK__ OnPlayerDisconnect(playerid, reason)
{
    new INI:File = INI_Open(data_path(playerid));
    INI_SetTag(File,"data");
    INI_WriteInt(File, "Admin", db_admin[playerid]);
    INI_WriteInt(File, "Owner", db_owner[playerid]);
    INI_WriteInt(File, "DutyTime", db_duty[playerid]);
    INI_Close(File);   

    DestroyVehicle(admin_Vehicle[playerid]);
	admin_Vehicle[playerid] = INVALID_PLAYER_ID;
    return 1;
}

HOOK__ OnPlayerConnect(playerid)
{
	admin_Vehicle[playerid] = INVALID_VEHICLE_ID;
    return Y_HOOKS_CONTINUE_RETURN_1;
}

HOOK__ OnVehicleDeath(vehicleid, killerid)
{
	DestroyVehicle(admin_Vehicle[vehicleid]);
	admin_Vehicle[vehicleid] = INVALID_PLAYER_ID;
    return Y_HOOKS_CONTINUE_RETURN_1;
}

YCMD:ah(playerid, const string:params[], help) = acommands;
YCMD:acommands(playerid, const string:params[], help) 
{
	if(db_admin[playerid] >= 1 || db_owner[playerid])
	{
		new tm_string[4000];

		if(db_admin[playerid] >= 1)
		{
			strcat(tm_string, ""c_red"ADMINISTRATOR 1 | "c_white"/a /ao /cc /aduty /jpack /fv /nitro /goto /respawn\n");
			strcat(tm_string, ""c_red"__________________________________________________________________________________________________\n");
		}
		if(db_admin[playerid] >= 2)
		{
			strcat(tm_string, ""c_red"ADMINISTRATOR 2 | "c_white"/freeze /unfreeze /slap /gethere /sethp /setarmor\n");
			strcat(tm_string, ""c_red"__________________________________________________________________________________________________\n");
		}
		if(db_admin[playerid] >= 3)
		{
			strcat(tm_string, ""c_red"ADMINISTRATOR 3 | "c_white"/setskin /kick /settime /setweather /kill\n");
			strcat(tm_string, ""c_red"__________________________________________________________________________________________________\n");
		}
		if(db_admin[playerid] >= 4)
		{
			strcat(tm_string, ""c_red"HEAD ADMIN | "c_white"/givegun /createrent /deleterent\n");
			strcat(tm_string, ""c_red"__________________________________________________________________________________________________\n");
		}
		if(db_admin[playerid] >= 5)
		{
			strcat(tm_string, ""c_red"DIREKTOR | "c_white"/makeadmin /setduty /killall /healall /createport /deleteport\n");
			strcat(tm_string, ""c_red"__________________________________________________________________________________________________\n");
		}
		if(db_admin[playerid] >= 6)
		{
			strcat(tm_string, ""c_red"MENAGER | "c_white"/createhouse /deletehouse\n");
			strcat(tm_string, ""c_red"__________________________________________________________________________________________________\n");
		}
		if(db_admin[playerid] >= 7)
		{
			strcat(tm_string, ""c_red"SU-VLASNIK | "c_white"/restart\n");
			strcat(tm_string, ""c_red"__________________________________________________________________________________________________\n");
		}
		Dialog_Show(playerid, "dialog_adminhelp", DIALOG_STYLE_MSGBOX, "Admin komande", tm_string, "Potvrdi", "Odustani");
	}
    else return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Niste Ovlasceni.");

	return 1;
}


YCMD:restart(playerid, const string: params[], help) 
{
	if(db_admin[playerid] < 7)
		return SendClientMessage(playerid, y_red, "( ERROR )  "c_white"Niste Ovlasceni.");
	 
	SendRconCommand("gmx");

	SendClientMessageToAll(-1, ""c_server"( SERVER ) "c_white"Restart servera je u toku");

	return 1;
}
YCMD:respawn(playerid, const string:params[], help)
{
	if(!db_admin[playerid]) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Niste Ovlasceni.");

	if(!IsPlayerInAnyVehicle(playerid)) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Niste u vozilu.");

	new vehicleid = GetPlayerVehicleID(playerid),
		fm_string[128];

	SetVehicleToRespawn(vehicleid);
	RemovePlayerFromVehicle(playerid);
	LinkVehicleToInterior(vehicleid, 0);

	format(fm_string, sizeof(fm_string), ""c_ruby"( ANTI-CHEAT ) %s je respawnovao vozilo id %d", ReturnPlayerName(playerid), vehicleid);

	foreach(new i: Player)
		if(db_admin[i])
			SendClientMessage(i, y_ruby, fm_string);

	return 1;
}

YCMD:kill(playerid, const string:params[], help)
{
    if(db_admin[playerid] < 3) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Niste Ovlasceni."); 

	static targetid;

    if(sscanf(params, "u", targetid)) return SendClientMessage(playerid, y_lblue, "( USAGE ) "c_white"/kill [targetid]");
	
	if(!IsPlayerConnected(targetid)) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Taj igrac nije na serveru.");

    if(db_admin[playerid] < db_admin[targetid]) return SendClientMessage(playerid, y_red, "( ERROR )  "c_white"Ne mozete ubiti staff-a veceg od sebe");

	SetPlayerArmour(targetid, 0);
    SetPlayerHealth(targetid, 0);
 	
    va_SendClientMessage(playerid, y_green, "( INFO ) "c_white"Ubili ste igraca %s.", ReturnPlayerName(targetid));
    va_SendClientMessage(targetid, y_yellow, "( KILL ) "c_white"Admin %s vas je ubio.", ReturnPlayerName(playerid));

	static fm_string[128];
	format(fm_string, sizeof(fm_string), "~r~( ANTI-CHEAT ) ~w~%s je ubio igraca ~r~%s", ReturnPlayerName(playerid), ReturnPlayerName(targetid));
	SideChatMessage(fm_string);

	return 1;
}

YCMD:podesihp(playerid, const string:params[], help) = sethp;
YCMD:sethp(playerid, const string:params[], help)
{
    if(db_admin[playerid] < 2) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Niste Ovlasceni.");

	static targetid, health;

	if(sscanf(params, "ud", targetid, health)) return SendClientMessage(playerid, y_lblue, "( USAGE ) "c_white"/sethp [targetid] [health]");

	if(!IsPlayerConnected(targetid)) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Taj igrac nije na serveru.");

    if(db_admin[playerid] < db_admin[targetid]) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Ne mozete podesiti health adminu vecem od sebe");

	SetPlayerHealth(targetid, health);

	va_SendClientMessage(playerid, y_green, "( INFO ) "c_white"Postavili ste helte igracu %s na %d.", ReturnPlayerName(targetid), health);
	
	return 1;
}

YCMD:setarmor(playerid, const string:params[], help)
{
    if(db_admin[playerid] < 2) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Niste Ovlasceni."); 

	static targetid, armour;

	if(sscanf(params, "ud", targetid, armour)) return  SendClientMessage(playerid, y_lblue, "( USAGE ) "c_white"/setarmor [targetid] [armor]");

	if(!IsPlayerConnected(targetid)) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Taj igrac nije na serveru.");

    if(db_admin[playerid] < db_admin[targetid]) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Ne mozete podesiti armor adminu vecem od sebe");

	SetPlayerArmour(targetid, armour);

	va_SendClientMessage(playerid, y_green, "( INFO ) "c_white"Postavili ste armor igracu %s na %d.", ReturnPlayerName(targetid), armour);
	
	return 1;
}

YCMD:healall(playerid, const string:params[], help)
{
    if(db_admin[playerid] < 5) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Niste Ovlasceni.");
		
	foreach(new i : Player)
		SetPlayerHealth(i, 100);
	
	static fm_string[128];
	format(fm_string, sizeof(fm_string), "~r~( ANTI-CHEAT ) ~w~%s je napunio health svim igracima.", ReturnPlayerName(playerid));
	SideChatMessage(fm_string);
			
	return 1;
}

YCMD:killall(playerid, const string:params[], help)
{
    if(db_admin[playerid] < 5) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Niste Ovlasceni.");
		
	foreach(new i : Player)
		SetPlayerHealth(i, 0);

	static fm_string[128];
	format(fm_string, sizeof(fm_string), "~r~( ANTI-CHEAT ) ~w~%s je ubio sve igrace na serveru.", ReturnPlayerName(playerid));
	SideChatMessage(fm_string);
			
	return 1;
}

YCMD:setweather(playerid, const string:params[], help)
{
    if(db_admin[playerid] < 3) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Niste Ovlasceni.");

    static weatherid;

    if(sscanf(params, "u", weatherid)) return SendClientMessage(playerid, y_lblue, "( USAGE ) "c_white"/setweather [weatherid]");
 
    va_SendClientMessageToAll(-1,""c_orange"( WEATHER ) Admin %s je podesio vreme na %s", ReturnPlayerName(playerid), weatherid);
   
    SetWeather(weatherid);

    return 1;
}

YCMD:settime(playerid, const string:params[], help)
{
    if(db_admin[playerid] < 3) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Niste Ovlasceni.");

    static timeid;

    if(sscanf(params, "u", timeid)) return SendClientMessage(playerid, y_lblue, "( USAGE ) "c_white"/settime [timeid]");
	
    va_SendClientMessageToAll(-1,""c_orange"( TIME ) Admin %s je podesio sat na %s", ReturnPlayerName(playerid), timeid);
   
    SetWorldTime(timeid);

    return 1;
}

YCMD:kick(playerid, const string:params[], help)
{
    if(db_admin[playerid] < 3) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Niste Ovlasceni.");

	static targetid;

	if(!IsPlayerConnected(targetid)) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Taj igrac nije na serveru.");

	if(sscanf(params, "ud", targetid, params)) return SendClientMessage(playerid, y_lblue, "( ? ) "c_white"/kick [targetid] [razlog]");

	va_SendClientMessage(targetid, y_ruby, "( KICK ) Kikovani ste sa servera od strane: %s | Razlog: %s ", ReturnPlayerName(playerid), params);

	static fm_string[128];
	format(fm_string, sizeof(fm_string), "~r~( ANTI-CHEAT ) ~w~%s je kikovao igraca %s | Razlog: %s.", ReturnPlayerName(playerid), ReturnPlayerName(targetid), params);
	SideChatMessage(fm_string);

	defer KickTimer(targetid);

	return 1;
}

YCMD:goto(playerid, const string:params[], help) 
{
    if(db_admin[playerid] >= 1 || db_supporter[playerid] == 1)
	{
		static
			targetid,
			Float:Position_X, 
			Float:Position_Y, 
			Float:Position_Z;

		new interiorid = GetPlayerInterior(targetid);

		if(sscanf(params, "u", targetid)) return SendClientMessage(playerid, y_lblue, "( USAGE ) "c_white"/goto [targetid]");

		if(!IsPlayerConnected(targetid)) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Taj igrac nije na serveru.");

		GetPlayerPos(targetid, Position_X, Position_Y, Position_Z);

		SetPlayerPos(playerid, Position_X, Position_Y+1, Position_Z+1);

		SetPlayerInterior(playerid, interiorid);
	}
	else return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Niste Ovlasceni.");

	return 1;
}

YCMD:gethere(playerid, const string:params[], help) 
{
    if(db_admin[playerid] < 2) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Niste Ovlasceni.");

	new interiorid = GetPlayerInterior(playerid);

	static
		targetid,
		Float:Position_X, 
		Float:Position_Y, 
		Float:Position_Z;

	if(sscanf(params, "u", targetid)) return SendClientMessage(playerid, y_lblue, "( USAGE ) "c_white"/gethere [targetid]");

	if(!IsPlayerConnected(targetid)) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Taj igrac nije na serveru.");

	GetPlayerPos(playerid, Position_X, Position_Y, Position_Z);

	SetPlayerPos(targetid, Position_X, Position_Y+1, Position_Z+1);

	SetPlayerInterior(targetid, interiorid);

	va_SendClientMessage(playerid, y_green, "( INFO )" c_white"Teleportovali ste igraca %s do sebe", ReturnPlayerName(targetid));
	va_SendClientMessage(targetid, y_green, "( INFO )" c_white"Admin %s vas je teleportovao do sebe", ReturnPlayerName(playerid));

	return 1;
}

YCMD:slap(playerid, const string:params[], help) 
{
    if(db_admin[playerid] >= 2 || db_supporter[playerid] >= 1)
	{

		static 
			targetid,
			reason,
			Float:Position_X, 
			Float:Position_Y, 
			Float:Position_Z;

		if(sscanf(params,"u", targetid, reason)) return SendClientMessage(playerid, y_lblue, "( USAGE ) "c_white"/slap [targetid] [razlog]." );

		if(!IsPlayerConnected(targetid)) 
			return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Taj igrac nije na serveru.");

		GetPlayerPos(targetid, Position_X, Position_Y, Position_Z );
		SetPlayerPos(targetid, Position_X, Position_Y, Position_Z+5 );

		va_SendClientMessage(playerid, y_green, "( INFO ) "c_white"Osamario si igraca: %s", ReturnPlayerName(targetid));
		va_SendClientMessage(targetid, y_green, "( INFO ) "c_white"Admin %s vas je osamario. Razlog: %s", ReturnPlayerName(playerid), reason);
	}
	else return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Niste Ovlasceni.");

    return 1;
}

YCMD:nitro(playerid, const string:params[], help)
{
	if(!db_admin[playerid]) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Niste Ovlasceni.");

	AddVehicleComponent(GetPlayerVehicleID(playerid), 1010);

	SendClientMessage(playerid, y_green, "( INFO ) "c_white"Dodali ste nitro u vozilo");

	return 1;
}

YCMD:freeze(playerid, const string:params[], help)
{
	if(db_admin[playerid] >= 2 || db_supporter[playerid] >= 1)
	{
		static targetid;

		if(sscanf(params, "u", targetid)) return SendClientMessage(playerid, y_lblue, "( USAGE ) "c_white"/freeze [targetid]");
		
		if(!IsPlayerConnected(targetid)) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Taj igrac nije na serveru.");

		if(db_admin[playerid] < db_admin[targetid]) return SendClientMessage(playerid, y_red, "( ERROR )  "c_white"Ne mozete zalediti admina veceg od sebe");
	
		TogglePlayerControllable(targetid, true);
	
		va_SendClientMessage(playerid, y_green, "( INFO ) "c_white"Zaledili ste igraca %s.", ReturnPlayerName(targetid));
		va_SendClientMessage(targetid, y_green, "( INFO ) "c_white"Admin %s vas je zaledio.", ReturnPlayerName(playerid));
	}
	else return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Niste Ovlasceni."); 

	return 1;
}

YCMD:unfreeze(playerid, const string:params[], help)
{
    if(db_admin[playerid] >= 2 || db_supporter[playerid] >= 1)
	{
		static targetid;

		if(sscanf(params, "u", targetid)) return SendClientMessage(playerid, y_lblue, "( USAGE ) "c_white"/unfreeze [targetid]");
		
		if(!IsPlayerConnected(targetid)) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Taj igrac nije na serveru.");

		if(db_admin[playerid] < db_admin[targetid]) return SendClientMessage(playerid, y_red, "( ERROR )  "c_white"Ne mozete odlediti admina veceg od sebe");
	
		TogglePlayerControllable(targetid, false);
	
		va_SendClientMessage(playerid, y_green, "( INFO ) "c_white"Odledili ste igraca %s.", ReturnPlayerName(targetid));
		va_SendClientMessage(targetid, y_green, "( INFO ) "c_white"Admin %s vas je odledio.", ReturnPlayerName(playerid));
	}
	else return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Niste Ovlasceni."); 
	return 1;
}

YCMD:setduty(playerid, const string:params[], help)
{
	if(db_admin[playerid] < 5) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Niste Ovlasceni."); 
	
	static targetid, time;
    
	if(sscanf(params, "ud", targetid, time)) return  SendClientMessage(playerid, y_lblue, "( USAGE ) "c_white"/setduty [targetid] [time]");

    if(time < 1 || time > 999) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Dutytime ne moze biti manji od 1 i veci od 999");

    if(!IsPlayerConnected(targetid)) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Taj igrac nije na serveru.");

    db_duty[targetid] += time;

    va_SendClientMessage(playerid, y_amber, "( INFO ) "c_white"Igracu %s si postavio dutytime na %d min.", ReturnPlayerName(targetid), time);
    va_SendClientMessage(targetid, y_green, "( INFO ) "c_white"Admin %s vam je podesio dutytime na %d min.", ReturnPlayerName(playerid), time);

	new INI:File = INI_Open(data_path(playerid));
	INI_SetTag(File, "data");
	INI_WriteInt(File, "DutyTime", db_duty[playerid]);
	INI_Close(File);

    return 1;

}

YCMD:givegun(playerid, const string: params[], help)
{
	if(db_admin[playerid] < 4 ) return SendClientMessage(playerid, y_red, "( ERROR )  "c_white"Niste Ovlasceni."); 

    static targetid, weaponid, ammo;

    if(sscanf(params, "uiu", targetid, weaponid, ammo)) return SendClientMessage(playerid, y_lblue, "( USAGE ) "c_white"/givegun [targetid] [weaponid] [ammo]");
	
	if(weaponid < 1 || weaponid > 46) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Weapon ID ne moze biti veci od 46 i manji od 1.");

    va_SendClientMessage(playerid, y_green, "( INFO ) "c_white"Dali ste oruzije igracu %s", ReturnPlayerName(targetid));
    va_SendClientMessage(targetid, y_green, "( INFO ) "c_white"Admin %s vam je dao oruzije id %d", ReturnPlayerName(playerid), weaponid);
   
    GivePlayerWeapon(targetid, weaponid, ammo);

    return 1;
}

YCMD:makeadmin(playerid, const string: params[], help) 
{
    static targetid, level;

    if(db_owner[playerid] || db_admin[playerid] > 5)
	{
		if(!IsPlayerConnected(targetid)) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Igrac nije na serveru.");

		if(sscanf(params, "ui", targetid, level)) return SendClientMessage(playerid, y_lblue, "( USAGE ) "c_white"/makeadmin [igrac] [level]");

		if(level < 0 || level > 7) return SendClientMessage(playerid, y_blue, "( HELP ) "c_white"Netacan admin level ( 0-7 ).");

		if(!level)
		{
			db_admin[targetid] = 0;
			db_skin[targetid] = 240;
			db_duty[targetid] = 0;
			SetPlayerSkin(playerid, 240);

			SendClientMessage(targetid, y_green, "------------------------------------------------------------------------------------------------");
			va_SendClientMessage(targetid, y_green, "( INFO ) "c_white"Owner %s vam je skinuo poziciju admina.", ReturnPlayerName(playerid));
			SendClientMessage(targetid, y_green, "( INFO ) "c_white"Ukoliko mislite da je doslo do greske postavite zalbu na forum.");
			SendClientMessage(targetid, y_green, "------------------------------------------------------------------------------------------------");

		}
		else
		{
			db_admin[targetid] = level;
			db_skin[targetid] = 294;
			db_duty[targetid] = 0;
			SetPlayerSkin(playerid, 294);

			SendClientMessage(targetid, y_green, "------------------------------------------------------------------------------------------------");
			va_SendClientMessage(targetid, y_green, "( INFO ) "c_white"Owner %s vam je postavio poziciju admina, level: %d.", ReturnPlayerName(playerid), level);
			SendClientMessage(targetid, y_green, "------------------------------------------------------------------------------------------------");
		}

		new INI:File = INI_Open(data_path(playerid));
		INI_SetTag(File, "data" );
		INI_WriteInt(File, "Admin", db_admin[playerid]);
		INI_WriteInt(File, "Skin", db_skin[playerid]);
		INI_WriteInt(File, "DutyTime", db_duty[playerid]);
		INI_Close(File);
	}
	else return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Niste Ovlasceni.");

    return 1;
}

YCMD:a(playerid, const string: params[], help) 
{
    if(!db_admin[playerid]) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Niste Ovlasceni.");

    if(isnull(params)) return SendClientMessage(playerid, y_lblue, "( USAGE ) "c_white"/a [text]");

    static fm_string[256];

    format(fm_string, sizeof(fm_string), "[ A ][ %s ] %s: %s", GetPlayerRank(playerid), ReturnPlayerName(playerid), params);

    foreach(new i: Player)
		if(db_admin[i])
			SendClientMessage(i, y_red, fm_string);

    return 1;
}

YCMD:ao(playerid, const string: params[], help) 
{
    if(!db_admin[playerid]) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Niste Ovlasceni.");

    if(isnull(params)) return SendClientMessage(playerid, y_lblue, "( USAGE ) "c_white"/ao [text]");

    va_SendClientMessageToAll(-1, ""c_red"(( %s: "c_white"%s "c_red"))", GetPlayerRank(playerid), params);

    return 1;
}

YCMD:cc(playerid, const string: params[], help) 
{
    if(!db_admin[playerid]) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Niste Ovlasceni.");

    for(new i; i < 110; i++) SendClientMessageToAll(-1, "");

	va_SendClientMessageToAll(-1, ""c_server"Chat je ociscen od strane admina %s", ReturnPlayerName(playerid));
	SendClientMessageToAll(-1, ""c_server"www.warsaw-community.detth");

    return 1;
}

YCMD:adminduty(playerid, const string:parms[], help) = aduty;
YCMD:aduty(playerid, const string:params[], help)
{
	if(db_admin[playerid] >= 1 || db_supporter[playerid] >= 1) 
	{

		if(admin_Duty[playerid] == false )
		{
			SetPlayerHealth(playerid, 100);
			SetPlayerArmour(playerid, 99);
			
			admin_Duty[playerid] = true;
			defer DutyTimer(playerid);
			va_SendClientMessageToAll(-1, ""c_red"( DUTY ) "c_white"%s %s "c_red"(Duty: %d min) "c_white"je sada na duznosti", GetPlayerRank(playerid), ReturnPlayerName(playerid), db_duty[playerid]);
		}
		else if(admin_Duty[playerid] == true)
		{
			admin_Duty[playerid] = false;
			va_SendClientMessageToAll(-1, ""c_red"( DUTY ) "c_white"%s %s "c_red"(Duty: %d min) "c_white"vise nije na duznosti", GetPlayerRank(playerid), ReturnPlayerName(playerid), db_duty[playerid]);	

		}
		new INI:File = INI_Open(data_path(playerid));
		INI_SetTag(File, "data");
		INI_WriteInt(File, "DutyTime", db_duty[playerid]);
		INI_Close(File);
	}
	else return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Niste Ovlasceni.");

    return 1;
}

YCMD:adminvozilo(playerid, const string:params[], help) = aveh;
YCMD:aveh(playerid, const string:params[], help) 
{
	if(!db_admin[playerid]) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Niste Ovlasceni.");

    if(admin_Vehicle[playerid] == INVALID_VEHICLE_ID) 
	{
        Dialog_Show(playerid, "dialog_adminvehicle", DIALOG_STYLE_LIST, 
		"Admin Vozila", 
		"Infernus\nNrg-500\nSultan\nHuntley\nCheetah\nSandKing", 
		"Potvrdi", "Odustani");
    }
    else
	{
        va_SendClientMessage(playerid, y_green, "( INFO ) "c_white"Vozilo [ ID: %d ] je unisteno.", admin_Vehicle[playerid]);
        DestroyVehicle(admin_Vehicle[playerid]);
        admin_Vehicle[playerid] = INVALID_VEHICLE_ID;
    }

    return 1;
}

YCMD:setskin(playerid, const string:params[], help) 
{
	if(db_admin[playerid] < 3) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Niste Ovlasceni."); 
		
	static targetid, skinid;

	if(sscanf(params, "ui", targetid, skinid)) return SendClientMessage(playerid, y_lblue, "( USAGE ) "c_white"/setskin [targetid] [skinid]");

	if(!IsPlayerConnected(targetid)) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Taj igrac nije na serveru.");

	if(skinid < 1 || skinid > 311) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"ID Skina ne moze biti manji od 1 i veci od 311.");

	db_skin[playerid] = skinid; 
	
	SetPlayerSkin(targetid, skinid);
	
    va_SendClientMessage(targetid, y_green, "( INFO ) "c_white"Admin %s vam je postavio skin [ID: %d].", ReturnPlayerName(playerid), skinid);
	va_SendClientMessage(playerid, y_green, "( INFO ) "c_white"Postavili ste igracu %s skin [ID: %d].", ReturnPlayerName(targetid), skinid);

	new INI:File = INI_Open(data_path(playerid));
	INI_SetTag(File, "data");
    INI_WriteInt(File, "Skin", db_skin[playerid]);
	INI_Close(File);

	return 1;
}

YCMD:jpack(playerid, const string:params[], help) = jp;
YCMD:jetpack(playerid, const string:params[], help) = jp;
YCMD:jp(playerid, const string:params[], help)
{
	if(!db_admin[playerid]) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Niste Ovlasceni.");

	static 
		Float:Position_X,
		Float:Position_Y,
		Float:Position_Z;

	if(GetPlayerSpecialAction(playerid) == SPECIAL_ACTION_USEJETPACK) 
	{
		GetPlayerPos(playerid, Position_X, Position_Y, Position_Z);
		SetPlayerPos(playerid, Position_X, Position_Y, Position_Z+2);

		SendClientMessage(playerid, y_green, "( INFO ) "c_white"Skinuo si jetpack");
	}
	else 
	{
		SetPlayerSpecialAction(playerid, SPECIAL_ACTION_USEJETPACK);

		SendClientMessage(playerid, y_green, "( INFO ) "c_white"Uzeo si jetpack.");
	}
   
    return 1;
}

YCMD:fv(playerid, const string:params[], help) = fixveh;
YCMD:fixveh(playerid, const string:params[], help)
{
	if(!db_admin[playerid]) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Niste Ovlasceni.");

	new vehicleid = GetPlayerVehicleID(playerid);

	static
		Float:Position_X, 
		Float:Position_Y, 
		Float:Position_Z, 
		Float:Position_A;

	if(!IsPlayerInAnyVehicle(playerid)) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Niste u vozilu.");
		
	GetPlayerPos(playerid, Position_X, Position_Y, Position_Z);
	GetVehicleZAngle(vehicleid, Position_A);
	SetVehicleZAngle(vehicleid, Position_A);
	RepairVehicle(vehicleid);
	SetVehicleHealth(vehicleid, 1000.0);
	
	return 1;

}

Dialog:dialog_adminvehicle(const playerid, response, listitem, string: inputtext[])
{
	if (!response)
		return 0;
	
	if(admin_Vehicle[playerid] == INVALID_VEHICLE_ID) 
	{
        new vehicleid;

        if(listitem == 0) { vehicleid = 411; }
        if(listitem == 1) { vehicleid = 522; }
        if(listitem == 2) { vehicleid = 560; }
        if(listitem == 3) { vehicleid = 579; }
        if(listitem == 4) { vehicleid = 415; }
        if(listitem == 5) { vehicleid = 451; }
        if(listitem == 6) { vehicleid = 490; }
        if(listitem == 7) { vehicleid = 495; }

        new 
			Float:Position_X,
			Float:Position_Y,
			Float:Position_Z;
			
        GetPlayerPos(playerid, Position_X, Position_Y, Position_Z);
        DestroyVehicle(admin_Vehicle[playerid]);
        admin_Vehicle[playerid] = CreateVehicle(vehicleid, Position_X, Position_Y, Position_Z, 0.0, 0, 0, -1);
        PutPlayerInVehicle(playerid, admin_Vehicle[playerid], 0);
        
        va_SendClientMessage(playerid, y_green, "( INFO ) "c_white"Vozilo [ ID: %d ] je stvoreno.", admin_Vehicle[playerid]);
	}
	return 1;
}