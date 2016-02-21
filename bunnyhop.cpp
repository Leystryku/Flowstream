#pragma once

#include "bunnyhop.h"

m_BunnyHop *BunnyHop = new m_BunnyHop;

void m_BunnyHop::DoJump( CUserCmd *pCmd, bool bPerfect )
{

	int iFlags = NetVar__GetFlags( g_pLocalEntity );

	if ( menu->M_AutoStrafe )
	{
		if ( pCmd->mousex < 0 )
			pCmd->move[1] = -10000;
		
		if ( pCmd->mousex > 0 )
			pCmd->move[1] = 10000;
	
	}

	if ( !bPerfect )
	{
		static bool firstjump = false, fakejmp;

		if ( (pCmd->buttons & IN_JUMP) && g_pLocalEntity->GetWaterLevel() < 2)
			if (!firstjump)
				firstjump = fakejmp = true;
		else if ( !( iFlags & FL_ONGROUND ) && !(iFlags & FL_PARTIALGROUND) )
			if ( fakejmp && NetVar__GetVelocity(g_pLocalEntity).z < 0.0f)
				fakejmp = false;
			else
				pCmd->buttons &= ~IN_JUMP;
			else
				fakejmp = true;
			else
				firstjump = false;

		return;
	}

	if ( (pCmd->buttons & IN_JUMP) && !( iFlags & FL_ONGROUND ) && !( iFlags & FL_PARTIALGROUND) )
		pCmd->buttons &= ~IN_JUMP;

}