#if defined vehicle_engine
    #endinput
#endif
#define vehicle_engine

#include <scripts\GetVehicleName>

HOOK__ OnVehicleSpawn(vehicleid)
{
	static engine, lights, alarm, doors, bonnet, boot, objective;

    GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
 
    if(VehicleBicycleModel(GetVehicleModel(vehicleid)))
    {
        SetVehicleParamsEx(vehicleid, 1, 0, 0, doors, bonnet, boot, objective);
    }
    else 
    {
        SetVehicleParamsEx(vehicleid, 0, 0, 0, doors, bonnet, boot, objective);
    }
 
	return 1;
}

HOOK__ OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
    {
        if(newkeys & KEY_YES)
        {
            new vehicleid = GetPlayerVehicleID(playerid);
    
            static engine, lights, alarm, doors, bonnet, boot, objective;

            static fm_string[256];
            
            if(VehicleBicycleModel(GetVehicleModel(vehicleid)))
            {
                return 1;
            }
 
            GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
 
            if(engine == VEHICLE_PARAMS_OFF)
            {
                SetEngineStatus(playerid, VEHICLE_PARAMS_ON);

                format(fm_string, sizeof(fm_string), ""c_purple"* %s Pali motor na vozilu marke %s", ReturnPlayerName(playerid), GetVehicleName(GetVehicleModel(vehicleid)));
            }
            else
            {
                SetEngineStatus(playerid, VEHICLE_PARAMS_OFF);

                format(fm_string, sizeof(fm_string), ""c_purple"* %s Gasi motor na vozilu marke %s", ReturnPlayerName(playerid), GetVehicleName(GetVehicleModel(vehicleid)));
            }

            __proxy(playerid, 20.0, -1, fm_string);
 
            return 1;
        }
    }
	return 1;
}

HOOK__ OnPlayerStateChange(playerid, newstate, oldstate)
{

    if(newstate == PLAYER_STATE_DRIVER) 
    { 
        SendClientMessage(playerid, y_yellow, "( engine ) "c_white"Pritisnite `Y` da upalite vozilo ili upisite `/engine`.");
    }
    return 1;
}

YCMD:engine(playerid, const string:params[], help) 
{
    if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
    {
        new vehicleid = GetPlayerVehicleID(playerid);
    
        static engine, lights, alarm, doors, bonnet, boot, objective;

        static fm_string[256];
        
        if(VehicleBicycleModel(GetVehicleModel(vehicleid)))
        {
            return 1;
        }
 
        GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
 
        if(engine == VEHICLE_PARAMS_OFF)
        {
            SetEngineStatus(playerid, VEHICLE_PARAMS_ON);

            format(fm_string, sizeof(fm_string), ""c_purple"* %s Pali motor na vozilu marke %s", ReturnPlayerName(playerid), GetVehicleName(GetVehicleModel(vehicleid)));
        }
        else
        {
            SetEngineStatus(playerid, VEHICLE_PARAMS_OFF);

            format(fm_string, sizeof(fm_string), ""c_purple"* %s Gasi motor na vozilu marke %s", ReturnPlayerName(playerid), GetVehicleName(GetVehicleModel(vehicleid)));
        }

        __proxy(playerid, 20.0, -1, fm_string);
 
        return 1;
    }
    return 1;
}