#if defined UnkownCommand
    #endinput
#endif
#define UnkownCommand

public e_COMMAND_ERRORS:OnPlayerCommandReceived(playerid, cmdtext[], e_COMMAND_ERRORS:success)
{
    if(success != COMMAND_OK) 
	{
		va_SendClientMessage(playerid, y_red, "( ERROR ) "c_white"`%s` je ne postojeca komanda, za listu komandi upisite ( /commands )", cmdtext);

        return COMMAND_OK;
    }

    return COMMAND_OK;
}