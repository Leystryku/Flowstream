#pragma once

#include "includes.h"


class m_utils
{
public:

	std::string formatString( std::string fmt, ... );

	bool DataCompare(const BYTE* pData, const BYTE* bMask, const char* szMask);

	DWORD FindPattern( DWORD dwAddress,const char *c_bMask,char * szMask );

	bool WorldToScreen( Vector v, Vector &s );

	void NormalizeAngles( QAngle &angle );

	void NormalizeVectors( Vector &angle );

	inline float NormalizeAngle( float ang );

	inline float NormalizeVector( float vec );

	void MakeVector( QAngle angle, QAngle& vector );

	void CalcAngle( Vector &src, Vector &dst, QAngle &angles );

	float flGetDistance( Vector vecOriginx,Vector vecOriginy );

	bool GetVisible( Vector& vecAbsStart, Vector& vecAbsEnd, C_BaseEntity* target=0  );

	bool IsSameTeam( C_BaseEntity *pEnt1, C_BaseEntity *pEnt2 );

	D3DCOLOR GetTeamColor( C_BaseEntity *pEntity );

	void StringCommand( const char *stringcmd );

	void SetName( const char *name );

	void ChatSay( const char *tosay, bool is_team );

	bool IsAlive( C_BaseEntity *pEnt );

	bool IsSpawnProtected( C_BaseEntity *pEnt );
	
	bool IsPlayerIndex( int iIndex );

	void* getInterface( CreateInterfaceFn* fn, PCHAR name );

	inline DWORD GetVirtualMethod(void* instance, unsigned int index)
	{
		int *ptr = *(int**)&instance;
		int *vtbl = (int *)*ptr; 

		return vtbl[index];
	}

	template<typename Fn>
    inline Fn GetVirtualFunc(void* v, int i)
    {
		return reinterpret_cast<Fn>((*(unsigned long**)v)[i]);
    }

};

extern m_utils *utils;
