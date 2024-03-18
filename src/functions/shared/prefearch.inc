#if defined prefearch
    #endinput
#endif
#define prefearch

stock SetEngineStatus(playerid, type)
{
    new vehicleid = GetPlayerVehicleID(playerid);
    
    static engine, lights, alarm, doors, bonnet, boot, objective;

    GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);

    return SetVehicleParamsEx(vehicleid, type, lights, alarm, doors, bonnet, boot, objective);
}

stock VehicleBicycleModel(modelid)
{
    if(modelid == 481 || modelid == 509 || modelid == 510) return 1;
 
    return 0;
}