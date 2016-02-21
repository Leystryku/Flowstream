#pragma once

#include "includes.h"

class m_Bots
{
public:

	void PredictTarget( C_BaseEntity* pBaseEntity, Vector &vecLocalVelocity, Vector &m_vecPosition, Vector &vEyePos );

	void RunCommand( CUserCmd *pCmd );

	void UpdateTarget( Vector &vecEyePos, Vector &vecLocalVelocity, Vector &vecTarget );

	void EstimateAbsVelocity( void* ent, Vector& vel );

	bool IsValidTarget( Vector &vecEyePos, Vector &vecLocalVelocity, Vector &HitVec, bool is_player, bool is_npc, const char *name, C_BaseEntity *pEnt );

	bool HoldingValidWep( );

	bool CanShoot( );

	Vector GetTarget(  Vector &eyePos, Vector &vecLocalVelocity, CUserCmd *pCmd );

	bool CalculateAimVector( C_BaseEntity *pEnt, Vector &vecEyePos, Vector &vec, int hitgroup= 0);

	float TargetRate( Vector localAbsOrigin, Vector vecLocalEyes, Vector vecLocalVelocity, C_BaseEntity *pEnt, bool is_npc, int target_mode );

	float GetFov( QAngle angle, Vector src, Vector dst );

	void FixMove(CUserCmd *cmd, QAngle& va);

	void Aimbot( CUserCmd *pCmd, bool* bSendPacket );

	void Trigger( CUserCmd *pCmd );

	C_BaseEntity *pTarget;
};

extern m_Bots *Bots;