#pragma once

#include "utils.h"

m_utils *utils = new m_utils;

std::string m_utils::formatString( string fmt, ... )
{
	int n, size = 100;
    std::string text;
    va_list marker;
    bool b = false;
    while (!b)
    {
        text.resize(size);
        va_start(marker, fmt);
        n = vsnprintf((char*)text.data(), size, fmt.c_str(), marker);
        va_end(marker);
        if ((n > 0) && ((b = (n < size)) == true)) text.resize(n); else size*=2;
    }

	return text.c_str();
}

bool m_utils::DataCompare(const BYTE* pData, const BYTE* bMask, const char* szMask)
{
	for(;*szMask;++szMask,++pData,++bMask)
		if(*szMask=='x' && *pData!=*bMask )
			return false;

	return (*szMask) == NULL;
}

DWORD m_utils::FindPattern( DWORD dwAddress, const char *c_bMask, char * szMask )
{
	BYTE *bMask = (PBYTE)c_bMask;
	DWORD dwMask = (DWORD)bMask;
	DWORD dwRet = 0;

	if ( !dwMask )
		return 0;

	if ( !bMask )
		return 0;

	DWORD dwSize = (DWORD)(dwAddress - dwMask);

	if ( !dwSize )
		return 0;

	__try {

		for(DWORD i=0; i < dwSize; i++)
		{
			if( DataCompare( (BYTE*)( dwAddress+i ),bMask,szMask) )
			{
				dwRet = (DWORD)(dwAddress+i);
				break;
			}
		}
	
	}

    __except(EXCEPTION_EXECUTE_HANDLER)
    {
		return 0;
    }

	return dwRet;
}


bool m_utils::WorldToScreen( Vector v, Vector &s )
{
	if ( !g_pClient )
		return false;

	if ( !g_pEngine )
		return false;

	if ( !(DWORD)g_EngineDLL )
		return false;

	float w;
	
	s.x = g_WorldToScreenMatrix[0][0] * v[0] + g_WorldToScreenMatrix[0][1] * v[1] + g_WorldToScreenMatrix[0][2] * v[2] + g_WorldToScreenMatrix[0][3];
	s.y = g_WorldToScreenMatrix[1][0] * v[0] + g_WorldToScreenMatrix[1][1] * v[1] + g_WorldToScreenMatrix[1][2] * v[2] + g_WorldToScreenMatrix[1][3];
	w	= g_WorldToScreenMatrix[3][0] * v[0] + g_WorldToScreenMatrix[3][1] * v[1] + g_WorldToScreenMatrix[3][2] * v[2] + g_WorldToScreenMatrix[3][3];

	// Just so we have something valid here
	s.z = 0.0f;

	bool behind;
	if( w < 0.001f )
	{
		behind = true;
		s.x *= 100000;
		s.y *= 100000;
	}
	else
	{
		behind = false;
		float invw = 1.0f / w;
		s.x *= invw;
		s.y *= invw;
	}

	if ( behind )
		return false;

	if ( !&v )
	{
		v[0] = 0;
		v[1] = 0;
		v[2] = 0;
	}

	if ( !v[0] )
		v[0] = 0;

	if ( !v[1] )
		v[1] = 0;

	if ( !v[2] )
		v[2] = 0;

	float x = g_ScrW / 2;
	float y = g_ScrH / 2;

	x += 0.5 * s.x * g_ScrW + 0.5;
	y -= 0.5 * s.y * g_ScrH + 0.5;

	s.x = x;
	s.y = y;

	return true;
}

void m_utils::NormalizeAngles( QAngle& angles )
{
	int i;
	
	// Normalize angles to -180 to 180 range
	for ( i = 0; i < 3; i++ )
	{
		if ( angles[i] > 180.0 )
		{
			angles[i] -= 360.0;
		}
		else if ( angles[i] < -180.0 )
		{
			angles[i] += 360.0;
		}
	}
}

void m_utils::NormalizeVectors( Vector& vectors )
{
	int i;
	
	// Normalize vectors to -180 to 180 range
	for ( i = 0; i < 3; i++ )
	{
		if ( vectors[i] > 180.0 )
		{
			vectors[i] -= 360.0;
		}
		else if ( vectors[i] < -180.0 )
		{
			vectors[i] += 360.0;
		}
	}
}

float m_utils::flGetDistance( Vector vecOriginx,Vector vecOriginy )
{
	double x = vecOriginx[0] - vecOriginy[0];
	double y = vecOriginx[1] - vecOriginy[1];
	double z = vecOriginx[2] - vecOriginy[2];
 
	return sqrtf( x*x + y*y + z*z);
}

float m_utils::NormalizeAngle( float angle )
{
	if ( angle < 0.0 )
		angle += 360.0;
	else if ( angle >= 360.0 )
		angle -= 360.0;
	return angle;
}

float m_utils::NormalizeVector( float vector )
{
	if ( vector < 0.0 )
		vector += 360.0;
	else if ( vector >= 360.0 )
		vector -= 360.0;
	return vector;
}

void m_utils::StringCommand( const char *stringcmd )
{

	if ( !g_pEngine->IsConnected() )
		return;
	
	INetChannel *ch = (INetChannel*)g_pEngine->GetNetChannelInfo();

	if ( !ch )
		return;

	char pckBuf[256];
	bf_write pck( pckBuf, sizeof( pckBuf ) );
	

	pck.WriteUBitLong( 4, NET_MESSAGE_BITS );

	pck.WriteString( stringcmd );
	
	ch->SendData( pck );
}


void m_utils::SetName( const char *name )
{

	if ( !g_pEngine->IsConnected() )
		return;

	INetChannel *ch = (INetChannel*)g_pEngine->GetNetChannelInfo();

	if ( !ch )
		return;

	char pckBuf[256];
	bf_write pck( pckBuf, sizeof( pckBuf ) );
	

	pck.WriteUBitLong( 5, NET_MESSAGE_BITS );
	pck.WriteByte( 0x01 );
	pck.WriteString( "name" );
	pck.WriteString( name );
	
	ch->SendData( pck );

}

void m_utils::ChatSay( const char *tosay, bool is_team )
{

	string saystr;

	if ( !is_team )
		saystr = string( "say " ) + string( tosay );
	else
		saystr = string( "say_team " ) + string( tosay );

	const char *c_saystr = saystr.c_str();

	StringCommand( c_saystr );

}

void m_utils::MakeVector( QAngle angle, QAngle& vector ) 
{ 
	float pitch; 
	float yaw; 
	float tmp;           

	pitch	= (float)(angle[0] * M_PI/180); 
	yaw		= (float)(angle[1] * M_PI/180); 
	tmp		= (float) cos(pitch);

	vector[0] = (float) (-tmp * -cos(yaw)); 
	vector[1] = (float) (sin(yaw)*tmp);
	vector[2] = (float) -sin(pitch);
}

void m_utils::CalcAngle( Vector &src, Vector &dst, QAngle &angles )
{

	double delta[3] = { (src[0]-dst[0]), (src[1]-dst[1]), (src[2]-dst[2]) };
	double hyp = sqrt(delta[0]*delta[0] + delta[1]*delta[1]);

	angles[0] = (float) (atan(delta[2]/hyp) * M_RADPI);
	angles[1] = (float) (atan(delta[1]/delta[0]) * M_RADPI);
	angles[2] = 0.0f;

	if(delta[0] >= 0.0) { angles[1] += 180.0f; }
}

void* m_utils::getInterface( CreateInterfaceFn* fn, PCHAR name )
{
	void *ret = 0;
	for( int i = 100; i > 0; --i )
	{
		PCHAR fmt = ( i < 10 ) ? "%s00%d" : "%s0%d";

		string swag = formatString( fmt, name, i );
		void *ptr = ( *fn )( swag.c_str(), 0 );
		if( ptr )
		{
			Msg(swag.c_str());
			Msg("\n");

			ret = ptr;
			break;
		}

		if ( i == 0 || i < 0 )
			break;

	}

	return ret;
}

bool m_utils::IsPlayerIndex( int index )
{
	return ( index >= 1 && index <= g_pGlobals->maxClients ) ? true : false;
}

bool m_utils::IsAlive( C_BaseEntity *pEnt )
{
	
	if ( !pEnt )
		return false;

	BYTE *plifeState = NetVar__GetLifeState(pEnt);
	BYTE lifeState = 0;

	if ( plifeState && *plifeState )
		lifeState = *plifeState;

	if ( lifeState != 0 )
		return false;

	//if ( NetVar__GetHealth(pEnt) < 0 )
	//	return false;

	//if ( pEnt->GetMoveType() == MOVETYPE_OBSERVER )
	//	return false;

	return true;
}

CTraceFilter filter;
Ray_t pRay;
trace_t tr;

bool m_utils::GetVisible( Vector& vecAbsStart, Vector& vecAbsEnd, C_BaseEntity* target ) 
{


	pRay.Init( vecAbsStart, vecAbsEnd );

	g_pEngineTrace->TraceRay( pRay, 0x46004003, &filter, &tr ); /*0x4600400B */

	if ( tr.fraction == 1.0f )
	{
		return true;
	}

	if ( tr.m_pEnt && target )
	{
		if ( tr.m_pEnt == target )
			return true;
	}

	return false;
}

bool m_utils::IsSameTeam( C_BaseEntity *pEnt1, C_BaseEntity *pEnt2 )
{

	return NetVar__GetTeamNumber(pEnt1) == NetVar__GetTeamNumber(pEnt2);

}

bool m_utils::IsSpawnProtected( C_BaseEntity *pEnt )
{

	if ( menu->S_SpawnProtection == 1 || menu->S_SpawnProtection == 2 )
	{
		int colorexpon = NetVar__GetColor(pEnt).exponent;

		if ( colorexpon != -1 )
			return true;
	}

	if ( menu->S_SpawnProtection == 3 )
	{
		const char *modelname = g_pModelInfo->GetModelName( pEnt->GetModel() );

		if ( !strcmp(modelname, "models/crow.mdl") )
			return true;
	}

	return false;
}

D3DCOLOR m_utils::GetTeamColor( C_BaseEntity *pEntity )
{
	D3DCOLOR ReturnColor = D3DCOLOR_RGBA( 255, 255, 255, 255 );

	if ( !g_pLocalEntity )
		return ReturnColor;

	if ( !g_bIsReady )
		return ReturnColor;


	int teamnum = NetVar__GetTeamNumber( pEntity );

	int localteamnum = NetVar__GetTeamNumber( g_pLocalEntity );
	

	if ( localteamnum == 0 && teamnum == 0 )
		return ReturnColor;

	if ( teamnum == localteamnum )
	{
		ReturnColor = D3DCOLOR_RGBA( 16, 255, 16, 255 );
	}else{
		ReturnColor = D3DCOLOR_RGBA( 255, 16, 16, 255 );
	}

	return ReturnColor;
}