#if defined vehicle_rent_included
    #endinput
#endif
#define vehicle_rent_included

#include <YSI_Coding\y_hooks>

static const RENT_PATH[64] = "Data/Rent/%d.ini";

const rent_pickups_max = 30;

static
    db_Admin[MAX_PLAYERS],
    db_Money[MAX_PLAYERS];

static
    rent_Pickup[MAX_PLAYERS],
    Text3D:rent_Label[MAX_PLAYERS],
    bool:rent_Owned[MAX_PLAYERS],
    rent_Vehicle[MAX_PLAYERS],

    rent_ID[rent_pickups_max],
    Float:rent_PosX[rent_pickups_max],
    Float:rent_PosY[rent_pickups_max],
    Float:rent_PosZ[rent_pickups_max];
    
HOOK__ data_load(playerid, const string: name[], const string: value[])
{
    INI_Int("Admin", db_Admin[playerid]);
    INI_Int("Money", db_Money[playerid]);

    return 1;
}

forward rent_load(const id, const string: name[], const string: value[]);
public rent_load(const id, const string: name[], const string: value[])
{
    INI_Int("ID", rent_ID[id]);
    INI_Float("Position_X", rent_PosX[id]);
    INI_Float("Position_Y", rent_PosY[id]);
    INI_Float("Position_Z", rent_PosZ[id]);

    return 1;
}

stock NextRentLocationID(const len)
{
    new id = (-1);
    for( new loop = ( 0 ), provjera = ( -1 ), Data_[ 64 ] = "\0"; loop != len; ++ loop ) {
       provjera = ( loop+1 );
       format( Data_, ( sizeof Data_ ), RENT_PATH, provjera );
       if(!fexist(Data_)) {
          id = ( provjera );
          break; } }
    return ( id );
}

stock rent_path(const id)
{
    new tmp_fmt[64];
    format(tmp_fmt, sizeof(tmp_fmt), RENT_PATH, id);
    return tmp_fmt;
}

HOOK__ OnGameModeInit()
{
    for(new id = 0; id < rent_pickups_max; id++)
    {
        static fm_string[256];

        if(fexist(rent_path(id)))
        {
            INI_ParseFile(rent_path(id), "rent_load", true, true, id);
            format(fm_string, sizeof(fm_string), ""c_blue"( Rent Vehicle )\nID: "c_white"%d\n"c_blue"( Pritisnite Y da iznajmite vozilo )", rent_ID[id]);
            rent_Label[id] = CreateDynamic3DTextLabel(fm_string, 0x3249A4FF, rent_PosX[id], rent_PosY[id], rent_PosZ[id], 30.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, -1, -1, -1, 30.0, -1, 0);
    		rent_Pickup[id] = CreateDynamicPickup(19135, 1, rent_PosX[id], rent_PosY[id], rent_PosZ[id], -1, -1, -1, 30.0, -1, 0);
        }
    }
}

YCMD:createrent(playerid, const string: params[], help)
{
    if(db_Admin[playerid] < 4) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Niste Ovlasceni.");
    
    new id = NextRentLocationID(rent_pickups_max);

    if(id == -1) 
        return SendClientMessage(playerid, y_ruby, "( ANTI-CHEAT ) Maksimalan broj rent lokacija je kreiran");

    static 
        Float:Position_X, 
        Float:Position_Y, 
        Float:Position_Z,
        fm_string[256];

    GetPlayerPos(playerid, Position_X, Position_Y, Position_Z);
    
    rent_ID[id] = id;
    rent_PosX[id] = Position_X; 
    rent_PosY[id] = Position_Y; 
    rent_PosZ[id] = Position_Z;

    format(fm_string, sizeof(fm_string), ""c_blue"( Rent Vehicle )\nID: "c_white"%d\n"c_blue"( Pritisnite Y da iznajmite vozilo )", rent_ID[id]);
    rent_Label[id] = CreateDynamic3DTextLabel(fm_string, 0x3249A4FF, rent_PosX[id], rent_PosY[id], rent_PosZ[id], 30.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, -1, -1, -1, 30.0, -1, 0);
    rent_Pickup[id] = CreateDynamicPickup(19135, 1, rent_PosX[id], rent_PosY[id], rent_PosZ[id], -1, -1, -1, 30.0, -1, 0);

    new INI:File = INI_Open(rent_path(id));
    INI_WriteInt(File,"ID", rent_ID[id]);
    INI_WriteFloat(File,"Position_X", rent_PosX[id]);
    INI_WriteFloat(File,"Position_Y", rent_PosY[id]);
    INI_WriteFloat(File,"Position_Z", rent_PosZ[id]);
    INI_Close(File);
    
    va_SendClientMessage(playerid, y_green, "( INFO ): "c_white"Kreirali ste rent, ( ID : %d ).", rent_ID[id]);

    return 1;
}

YCMD:deleterent(playerid, const string: params[], help)
{
    if(db_Admin[playerid] >= 4)
    {
        for(new i = 0; i < rent_pickups_max; i++)
        {
            new id = rent_ID[i];

            if(sscanf(params, "i", id)) return SendClientMessage(playerid, y_lblue, "( USAGE ) "c_white"/deleterent [id]");

            if(fexist(rent_path(id)))
            {
                fremove(rent_path(id));

                DestroyDynamic3DTextLabel(rent_Label[id]);
                DestroyDynamicPickup(rent_Pickup[id]);

                SendClientMessage(playerid, y_green, "( INFO ) "c_white"Obrisali ste rent lokaciju [ID:%d]", id);
            }
        }
    }
    else return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Niste Ovlasceni.");
    
    return 1;
}

YCMD:unrent(playerid, const string: params[], help)
{
	if(!rent_Owned[playerid]) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Ne rentate vozilo.");

	DestroyVehicle(rent_Vehicle[playerid]);
	rent_Owned[playerid] = false;
	rent_Vehicle[playerid] = -1;
	SendClientMessage(playerid, y_green, "( INFO ) "c_white"Unrentali ste vozilo");

	return 1;
}

Dialog:dialog_rent(playerid, response, listitem, string:inputtext[])
{
    if(!response) 
        return 0;

	if(response)
	{
		switch(listitem)
		{
			case 0:
			{
				if(GetPlayerMoney(playerid) < 300) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Nemate dovoljno novca.");

				static 
                    Float:Position_X, 
                    Float:Position_Y, 
                    Float:Position_Z;

		        GetPlayerPos(playerid, Position_X, Position_Y, Position_Z);

                DestroyVehicle(rent_Vehicle[playerid]);
		        rent_Vehicle[playerid] = CreateVehicle(462, Position_X, Position_Y, Position_Z, 0, 0, 0, -1 );
                PutPlayerInVehicle(playerid, rent_Vehicle[playerid], 0);

		        rent_Owned[playerid] = true;
		        GivePlayerMoney(playerid, -300);
                db_Money[playerid] -= 300;

                new INI:File = INI_Open(data_path(playerid));
                INI_SetTag(File, "data");
                INI_WriteInt(File, "Money", db_Money[playerid]);
                INI_Close(File);

		        return 1;
			}
            case 1:
			{
				if(GetPlayerMoney(playerid) < 700) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Nemate dovoljno novca.");

				static 
                    Float:Position_X, 
                    Float:Position_Y, 
                    Float:Position_Z;

		        GetPlayerPos(playerid, Position_X, Position_Y, Position_Z);

                DestroyVehicle(rent_Vehicle[playerid]);
		        rent_Vehicle[playerid] = CreateVehicle(445, Position_X, Position_Y, Position_Z, 0, 0, 0, -1 );
                PutPlayerInVehicle(playerid, rent_Vehicle[playerid], 0);

		        rent_Owned[playerid] = true;
		        GivePlayerMoney(playerid, -700);
                db_Money[playerid] -= 700;

                new INI:File = INI_Open(data_path(playerid));
                INI_SetTag(File, "data");
                INI_WriteInt(File, "Money", db_Money[playerid]);
                INI_Close(File);
                
		        return 1;
			}
        }
    }
    return 1;   
}

HOOK__ OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	if(GetPlayerState(playerid) == PLAYER_STATE_ONFOOT)
    {
        if(newkeys & KEY_YES)
        {

            for(new i = 0; i < rent_pickups_max; i++)
            {
                if(IsPlayerInRangeOfPoint(playerid, 3.0, rent_PosX[i], rent_PosY[i], rent_PosZ[i])) 
                {
                    if(rent_Owned[playerid]) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Vec rentate vozilo /unrent");

                    Dialog_Show(playerid, "dialog_rent", DIALOG_STYLE_TABLIST_HEADERS, 
                        "Rent Vehicle", 
                        "Vozilo\tCena\n\
                        Faggio\t300$\n\
                        Admiral\t700$", 
                        "Potvrdi", "Odustani"
                    );
                }
            }  
            return 1;
        }
    }
	return 1;
}