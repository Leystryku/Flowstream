#pragma once

#include "includes.h"
#include "d3d.h"
#include "menu.h"

VMTHook *baseClientHook;

void __fastcall hooked_Disconnect( void* thisptr, int edx, const char *dc_string );

class FlowEventHandle : public IGameEventListener2
{
	void FireGameEvent( IGameEvent* event )
	{
		if(Q_strcmp(event->GetName(), "player_hurt") == 0)
		{
			int victimID = event->GetInt( "userid", 0 );
			int attackerID = event->GetInt( "attacker", 0 );
			int hp = event->GetInt( "health", 0 );

			player_info_s pVictimInfo;
			player_info_s pAttackerInfo;

			int vicEntNum = g_pEngine->GetPlayerForUserID(victimID);
			int atkEntNum = g_pEngine->GetPlayerForUserID(attackerID);

			g_pEngine->GetPlayerInfo( vicEntNum, &pVictimInfo ); 

			g_pEngine->GetPlayerInfo( atkEntNum, &pAttackerInfo );

			C_BaseEntity *vicEnt = (C_BaseEntity*)g_pEntList->GetClientEntity(vicEntNum);
			C_BaseEntity *atkEnt = (C_BaseEntity*)g_pEntList->GetClientEntity(atkEntNum);
			
			if ( vicEnt == g_pLocalEntity && utils->IsAlive(atkEnt) )
			{
				Bots->pTarget = atkEnt;
			}

			if ( menu->M_VoiceChatSpam )
				Msg( "[(Flowstream)] %s damaged %s to %i HP\n", pAttackerInfo.name, pVictimInfo.name, hp );
		}

		if(Q_strcmp(event->GetName(), "player_say") == 0)
		{

			int userId = event->GetInt( "userid");
			const char* text = event->GetString( "text");
			
			if ( menu->M_TextChatImitator )
			{

				if ( !menu->M_TextChatTeam )
					utils->ChatSay( text, false );
				else
					utils->ChatSay( text, true );

			}

		}

		if(Q_strcmp(event->GetName(), "game_newmap") == 0)
		{
			//33->37, 34+

			 g_pNetChan = (INetChannel *)g_pEngine->GetNetChannelInfo();

			if ( !g_pNetChan )
				return;

			baseClientHook = new VMTHook( g_pNetChan );
			DisconnectFn = (OriginalDisconnect)baseClientHook->hookFunction( 36, hooked_Disconnect );

		}

	}
} GameEventHandler;