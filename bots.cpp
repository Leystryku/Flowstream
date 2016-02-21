#pragma once

#include "bots.h"

m_Bots *Bots = new m_Bots;

static DWORD flags_offset = NULL;


typedef void ( __thiscall* OriginalSetupMove )( void* sex, void *ent, void* usercmd, void* shit, void* md );
typedef void ( __thiscall* OriginalFinishMove )( void* sex, void *ent, void* usercmd, void* md );
typedef void ( __thiscall* OriginalProcessMovement )( void* sex, void *ent, void* md );

static CMoveData g_FlowMoveData;
CMoveData *g_pFlowMoveData = &g_FlowMoveData;
IMoveHelper *g_pMoveHelper;

void m_Bots::RunCommand( CUserCmd* pCmd )
{

	if ( !flags_offset )
		flags_offset = g_pNetworkMan->GetOffset( "DT_BasePlayer", "m_fFlags" );

	int *ptrflags = (int*)((DWORD)g_pLocalEntity+flags_offset);
	int flags = *ptrflags;

	QAngle *ptrvecPunchAng = NetVar__GetViewPunch( g_pLocalEntity );
	QAngle vecPunchAng = *ptrvecPunchAng;

	static int o_predcmd = 0;

	if (!o_predcmd )
	{
		DWORD method = utils->GetVirtualMethod(g_pPrediction, 17);
		DWORD dwoPredCmd;

		if ( method )
			dwoPredCmd = utils->FindPattern(method, "\x89\x00\x00\x00\x00\x00\xE8", "x?????x");

		if ( dwoPredCmd )
			o_predcmd = *(int*)(dwoPredCmd + 2);

		if ( !o_predcmd )
		{
			MessageBox( NULL, "nope","k",MB_OK);
			return;
		}
	}

	if ( menu->S_PredictionType == 3 )
	{
		g_pPrediction->RunCommand( g_pLocalEntity, pCmd, g_pMoveHelper );

		*ptrflags = flags;
		*ptrvecPunchAng = vecPunchAng;

	}else{

		CUserCmd *predcmd = (CUserCmd*)((DWORD)g_pLocalEntity+o_predcmd);
		predcmd = pCmd;

		utils->GetVirtualFunc<OriginalSetupMove>( g_pPrediction, 18 )( g_pPrediction, g_pLocalEntity, pCmd, 0, g_pFlowMoveData );

		utils->GetVirtualFunc<OriginalProcessMovement>( g_pMovement, 1 )( g_pMovement, g_pLocalEntity, g_pFlowMoveData );

		utils->GetVirtualFunc<OriginalFinishMove>( g_pPrediction, 19 )( g_pPrediction, g_pLocalEntity, pCmd, g_pFlowMoveData );

		*ptrflags = flags;
		*ptrvecPunchAng = vecPunchAng;

	}

}


typedef void ( __thiscall* OriginalEstimateAbsVelocity )( void *ent, Vector& vel );
OriginalEstimateAbsVelocity EstimateAbsVelocityFn;

void m_Bots::EstimateAbsVelocity( void* ent, Vector& vel )
{
	

	if ( !EstimateAbsVelocityFn )
		EstimateAbsVelocityFn = (OriginalEstimateAbsVelocity)utils->FindPattern((DWORD)g_ClientDLL, "\x55\x8B\xEC\x83\xEC\x0C\x56\x8B\xF1\xE8\x00\x00\x00\x00\x3B\xF0", "xxxxxxxxxx????xx");

	if ( !EstimateAbsVelocityFn )
	{
		Msg("COULD NOT FIND EstimateAbsVelocityFn\n");
		return;
	}

	//utils->GetVirtualFunc<OriginalEstimateAbsVelocity>( ent, 360 )( ent, vel );
	EstimateAbsVelocityFn( ent, vel );

}

float gravity = 0;

void m_Bots::PredictTarget( C_BaseEntity* pBaseEntity, Vector &vecLocalVel, Vector &m_vecPosition, Vector &vEyePos )
{

	Vector vEnemyVel = Vector(0,0,0);// = NetVar__GetVelocity( pBaseEntity );

	EstimateAbsVelocity( pBaseEntity, vEnemyVel );

	//Msg("MYVEL: %f %f %f - HISVEL: %f %f %f\n", vecLocalVel.x, vecLocalVel.y, vecLocalVel.z, vEnemyVel.x, vEnemyVel.y, vEnemyVel.z );

	if( !( NetVar__GetFlags(pBaseEntity) & FL_ONGROUND ) )
	{
		vEnemyVel.z -= gravity * 0.5f * g_pGlobals->frametime;
		vEnemyVel.z += NetVar__GetVelocity(pBaseEntity).z * g_pGlobals->frametime;
	}

//	if( !( NetVar__GetFlags(g_pLocalEntity) & FL_ONGROUND ) )
//	{
//		vecLocalVel.z -= gravity * 0.5f * g_pGlobals->frametime;
//		vecLocalVel.z += NetVar__GetVelocity(g_pLocalEntity).z * g_pGlobals->frametime;
//	}

	Vector vLocalVelocity = vecLocalVel * g_pGlobals->interval_per_tick;
	Vector vTargetVelocity= vEnemyVel * g_pGlobals->interval_per_tick;
	Vector vDelta = vTargetVelocity - vLocalVelocity;
	m_vecPosition += vDelta;
	vEyePos -= vLocalVelocity;

}

float m_Bots::GetFov( QAngle angle, Vector src, Vector dst ) 
{ 
	QAngle ang,aim;
	float fov; 

	utils->CalcAngle(src, dst, ang); 
	utils->MakeVector(angle, aim); 
	utils->MakeVector(ang, ang);      

	float mag_s = sqrt((aim[0]*aim[0]) + (aim[1]*aim[1]) + (aim[2]*aim[2])); 
	float mag_d = sqrt((aim[0]*aim[0]) + (aim[1]*aim[1]) + (aim[2]*aim[2])); 

	float u_dot_v = aim[0]*ang[0] + aim[1]*ang[1] + aim[2]*ang[2]; 

	fov = acos(u_dot_v / (mag_s*mag_d)) * (180.0 / M_PI); 

	return fov; 
}

/*
enum hitboxes
{
HITGROUP_GENERIC= 0,
HITGROUP_HEAD= 1,
HITGROUP_CHEST= 2,
HITGROUP_STOMACH= 3,
HITGROUP_LEFTARM= 4,
HITGROUP_RIGHTARM= 5,
HITGROUP_LEFTLEG= 6,
HITGROUP_RIGHTLEG= 7,
HITGROUP_GEAR= 10,
};*/

mstudiobbox_t* hitbox;
studiohdr_t* studiohdr;

Vector defaultretVec(-999999.0f, -999999.0f, -999999.0f);


bool m_Bots::CalculateAimVector( C_BaseEntity *pEnt, Vector &vecEyePos, Vector &vec, int hitgroup ) 
{
	//Msg("CALCAIMVEC1\n");

	vec.x = 0;
	vec.y = 0;
	vec.z = 0;

	const model_t* model = pEnt->GetModel();

	if (!model)
		return false;

	studiohdr = g_pModelInfo->GetStudiomodel( model );

	if (!studiohdr)
		return false;

	matrix3x4_t boneMatrix[MAXSTUDIOBONES];

	float flSimulationTime = NetVar__GetSimulationTime( pEnt );

	if ( !flSimulationTime )
		return false;

	if ( !pEnt->SetupBones(boneMatrix, 128, BONE_USED_BY_HITBOX, flSimulationTime) )
		return false;

	if ( !boneMatrix )
		return false;

	//if ( menu->S_HitGroup )
	//	hitgroup = menu->S_HitGroup;

	//if ( menu->M_Aim_RandomHitgroup == 1 )
	//	hitgroup = RandomInt(0, 7);

	int iHitboxCount = studiohdr->iHitboxCount(0);

	bool found = false;

	defaultretVec.x = defaultretVec.y = defaultretVec.z = -999999.0f;

	Msg( "%i :: HITBOXCOUNTER\n", studiohdr->iHitboxCount(0) );

	if ( hitgroup )
	{
		for (int i = 0; i <studiohdr->iHitboxCount(0); ++i)
		{

			hitbox = studiohdr->pHitbox(i, 0);

			if ( !hitbox )
				continue;
		
			if ( hitbox->group != hitgroup )
				continue;

			Vector min_vec, max_vec;

			matrix3x4_t matrix = boneMatrix[hitbox->bone];

			VectorTransform(hitbox->bbmin, matrix, min_vec);
			VectorTransform(hitbox->bbmax, matrix, max_vec);

			if ( min_vec.x > 200000000000000000 )
				continue;

			if ( max_vec.x < -200000000000000000 )
				continue;

			Vector tmpVec = (min_vec + max_vec) * 0.5f;
		
			if ( utils->GetVisible( vecEyePos, tmpVec, pEnt ) )
			{

				vec = tmpVec;
				return true;
			}
		}
	}else{
		for (int i = 0; i <studiohdr->iHitboxCount(0); ++i)
		{

			hitbox = studiohdr->pHitbox(i, 0);

			if ( !hitbox )
				continue;

			if ( hitbox->group > 7)
				continue;

			Vector min_vec, max_vec;

			matrix3x4_t matrix = boneMatrix[hitbox->bone];

			VectorTransform(hitbox->bbmin, matrix, min_vec);
			VectorTransform(hitbox->bbmax, matrix, max_vec);

			if ( min_vec.x > 200000000000000000 )
				continue;

			if ( max_vec.x < -200000000000000000 )
				continue;

			Vector tmpVec = (min_vec + max_vec) * 0.5f;
		
			if ( utils->GetVisible( vecEyePos, tmpVec, pEnt ) )
			{
				if ( hitbox->group == 1 )
				{
					found = true;
					defaultretVec = tmpVec;
					break;
				}

				if (tmpVec.z> defaultretVec.z) 
				{
					found = true;
					defaultretVec = tmpVec;
				}
			}

		}
	}

	if ( hitgroup )
	{
		vec.x = 0;
		vec.y = 0;
		vec.z = 0;
		return false;
	}
	else
	{
		vec = defaultretVec;
		return found;
	}

	/*
	if ( resVec.x < 0.0001 && resVec.x > -0.0001 && resVec.y < 0.0001 && resVec.y > -0.0001 && resVec.z < 0.0001 && resVec.z > -0.0001 )
		return false;

	if ( resVec.x < -999998 && resVec.y < -999998 && resVec.z < -999998 )
		return false;

	if ( found )
		Msg("%f - %f - %f\n", defaultretVec.x, defaultretVec.y, defaultretVec.z);

	*/


	return false;
}

bool m_Bots::HoldingValidWep( )
{

	if ( !g_bIsReady )
		return false;
	
	if ( !g_pLocalEntity )
		return false;

	if ( !g_pLocalWeapon )
		return false;

	if ( g_pLocalWeapon->IsDormant() )
		return false;

	return true;
}

bool m_Bots::CanShoot( )
{

	if ( !HoldingValidWep() )
		return false;

	if ( g_flNextPrimaryAttack >= g_pGlobals->curtime )
		return false;

	int m_iClip = NetVar__GetClip1( g_pLocalWeapon );

	if ( !menu->M_Aim_IgnoreAmmo )
		if ( 1 > m_iClip )
			return false;

	return true;
			
}

bool m_Bots::IsValidTarget( Vector &vecEyePos, Vector &vecLocalVelocity, Vector &HitVec, bool is_player, bool is_npc, const char *name, C_BaseEntity *pEnt )
{
	if ( !pEnt )
		return false;

	if ( pEnt->IsDormant() )
	{
		return false;
	}

	if ( !utils->IsAlive(pEnt) )
		return false;

	if ( g_pLocalEntity == pEnt )
	{
		return false;
	}

	ClientClass *pClientClass = pEnt->GetClientClass();

	if ( !pClientClass )
		return false;
		
	name = pClientClass->m_pNetworkName;
	
	if ( !name )
		return false;

	if ( menu->M_Aim_KillPlayer && utils->IsPlayerIndex(pEnt->entindex()) )
	{
		is_player = true;
	}else{

		if ( menu->M_Aim_KillNPC )
		{

			if ( strstr(name, "NPC") )
				is_npc = true;

			if ( g_AppID == 440 && strstr(name, "Object") )
				is_npc = true;

		}

	}

	if ( !is_npc && !is_player )
		return false;

	if ( is_player && menu->M_Aim_TeamKill == 0 )
	{
		if ( utils->IsSameTeam(g_pLocalEntity, pEnt) )
			return false;
	}

	if ( menu->S_SpawnProtection && is_player && utils->IsSpawnProtected(pEnt) )
		return false;

	if ( g_AppID == 440 )
	{
		DWORD *plyCond = TF2__NetVar__GetPlayerCond( pEnt );

		if (*(plyCond) & TFCond_Ubercharged )
		{
			//works
			return false;
		}
	}

	bool gothitbox = false;

	if ( menu->M_Aim_Hitscan )
	{
		gothitbox = CalculateAimVector( pEnt, vecEyePos, HitVec );

		if ( !gothitbox ) return false;

	}else{

		if ( !menu->M_Aim_Snipe || !g_pLocalWeapon )
		{
			gothitbox = CalculateAimVector( pEnt, vecEyePos, HitVec, 1);

			if ( !gothitbox ) return false;

		}else{

			if ( menu->M_Aim_Snipe )
			{
				const char* wepclass = "";

				if ( g_AppID != 4000 )
				{
					ClientClass *cc = g_pLocalWeapon->GetClientClass();

					if ( cc && cc->m_pNetworkName )
					{
						wepclass = cc->m_pNetworkName;

						if ( strstr(wepclass, "AWP") || strstr(wepclass, "Snip") )
							gothitbox = CalculateAimVector( pEnt, vecEyePos, HitVec, 2);
						else
							gothitbox = CalculateAimVector( pEnt, vecEyePos, HitVec, 1);
					}
				}else{

					wepclass = NetVar__GetClassname( g_pLocalWeapon );


					if ( strstr(wepclass, "awp") || strstr(wepclass, "snip") )
						gothitbox = CalculateAimVector( pEnt, vecEyePos, HitVec, 2);
					else
						gothitbox = CalculateAimVector( pEnt, vecEyePos, HitVec, 1);
				}
			}

		}
	}

	if ( menu->S_Aim_FOV > 2 )
	{
		float fov = (float)menu->S_Aim_FOV;

		if ( GetFov( g_pLocalEntity->GetAbsAngles(), vecEyePos, HitVec ) > fov )
			return false;
	}

	return true;
}

void m_Bots::UpdateTarget( Vector &vecLocalEyePos, Vector &vecLocalVelocity, Vector &vecTarget )
{

	float bestRate = 9999999999999999;
	float plyRate = 0;

	Vector HitVec = Vector(0,0,0);

	bool is_npc = false;
	bool is_player = false;
	
	const char *name = "";

	int targetMode = menu->S_TargetMode;

	Vector tmpTarVec = Vector(0,0,0);
	C_BaseEntity *tmpWinner = NULL;

	int MaxIterate = 0;

	if ( menu->M_Aim_KillPlayer )
		MaxIterate = g_pEngine->GetMaxClients();

	if ( menu->M_Aim_KillNPC )
		MaxIterate = g_pEntList->GetHighestEntityIndex();

	Vector vecLocalAbsOrigin = g_pLocalEntity->GetAbsOrigin(); //+ NetVar__GetVelocity( g_pLocalEntity ) * g_pGlobals->interval_per_tick;

	for (int i = 0; i <MaxIterate+1; ++i)
	{

		IClientEntity *iCLent = g_pEntList->GetClientEntity(i);
		
		if ( !iCLent )
		{
			continue;
		}

		C_BaseEntity *targetEnt = (C_BaseEntity*)iCLent;

		if ( !targetEnt )
			continue;

		if ( !IsValidTarget(vecLocalEyePos, vecLocalVelocity, HitVec, is_player, is_npc, name, targetEnt) )
			continue;

		plyRate = TargetRate( vecLocalAbsOrigin, vecLocalEyePos, vecLocalVelocity, targetEnt, is_npc, targetMode);

		if ( plyRate < bestRate )
		{
			tmpWinner = targetEnt;
			bestRate = plyRate;
			tmpTarVec = HitVec;
		}

		is_npc = false;
		is_player = false;
		name = "";
	}

	if ( tmpTarVec.x == 0 && tmpTarVec.y == 0 && tmpTarVec.z == 0 )
		return;

	vecTarget = tmpTarVec;
	pTarget = tmpWinner;
}

int ShouldUpdate = 10;

Vector m_Bots::GetTarget( Vector &eyePos, Vector &vecLocalVelocity, CUserCmd *pCmd )
{

	Vector vecTarget = Vector(0,0,0);

	if ( menu->S_SpawnProtection && menu->S_SpawnProtection != 2 )
	{
		if ( utils->IsSpawnProtected(g_pLocalEntity) )
			return vecTarget;
	}

	ShouldUpdate -= 1;

	bool t_isplayer = false;
	bool t_isnpc = false;

	const char *name = "";

	if ( !gravity || gravity == 0.0f )
		gravity = g_pCVar->FindVar("sv_gravity")->GetFloat();

	if ( ShouldUpdate < 1 )
	{
		UpdateTarget( eyePos, vecLocalVelocity, vecTarget );
		ShouldUpdate = 10;
		gravity = g_pCVar->FindVar("sv_gravity")->GetFloat();
		return vecTarget;

	}

	if ( pTarget && IsValidTarget(eyePos, vecLocalVelocity, vecTarget, t_isplayer, t_isnpc, name, pTarget) )
	{

		if ( menu->S_PredictionType == 2 )
			PredictTarget(pTarget, vecLocalVelocity, vecTarget, eyePos);

		return vecTarget;
	}else{
		pTarget = NULL;

		ShouldUpdate = 10;
		UpdateTarget( eyePos, vecLocalVelocity, vecTarget );

		if ( pTarget )
		{
			if ( menu->S_PredictionType == 2 )
				PredictTarget(pTarget, vecLocalVelocity, vecTarget, eyePos); 

			return vecTarget;
		}
		else
			return Vector(0,0,0);
	}

	return vecTarget;
}

float m_Bots::TargetRate( Vector vecLocalAbsOrigin, Vector vecLocalEyes, Vector vecLocalVelocity, C_BaseEntity *pEnt, bool is_npc, int target_mode )
{

	float rate = 0.f;

	Vector plAbsOrigin = pEnt->GetAbsOrigin(); //+ NetVar__GetVelocity( g_pLocalEntity ) * g_pGlobals->interval_per_tick;

	if ( menu->S_PredictionType == 2 )
		plAbsOrigin += vecLocalVelocity;

	if ( target_mode == 0 )
	{
		rate = utils->flGetDistance(vecLocalAbsOrigin, plAbsOrigin);

		return rate;
	}
	
	if ( target_mode == 1 )
	{
		rate = NetVar__GetHealth( pEnt );

		return rate;
	}

	if ( target_mode == 2 )
	{
		Vector path = plAbsOrigin - vecLocalAbsOrigin;

		utils->NormalizeVectors( vecLocalEyes );

		rate = utils->flGetDistance(path, vecLocalEyes);

		return rate;
	}

	if ( target_mode == 3 )
	{
		Vector path = vecLocalAbsOrigin - plAbsOrigin;

		utils->NormalizeVectors( vecLocalEyes );

		rate = utils->flGetDistance(path, vecLocalEyes);

		C_BaseEntity *pWeapon = NULL;

		if ( is_npc )
			rate *= 10.f;
		else{
			pWeapon = NetVar__C_GetActiveWeapon( pEnt );
			if ( !pWeapon )
				rate *= 5.f;

			if ( g_AppID == 240 )
			{

				if ( pWeapon )
				{
					ClientClass *cc = pWeapon->GetClientClass();

				
					const char *classname;

					if ( cc )
						classname = cc->m_pNetworkName;

					if ( classname )
					{
						if ( !strcmp(classname, "CWeaponAWP") || !strcmp(classname, "CDEagle") || !strcmp(classname, "CWeaponScout") )
							rate *= 0.01f;
					}
				}

			}
		}

		return rate;

	}


	return 0.0f;
}

C_BaseEntity *lastTarg;

QAngle SmoothAimDelta;

QAngle bak_angView;
QAngle bak_angMove;

bool should_restore = false;

void m_Bots::FixMove(CUserCmd *cmd, QAngle& va)
{


	float yaw, speed;
	Vector& move = *(Vector*) &cmd->move[0];
	Vector move2 = *(Vector*) &cmd->move;

	speed = move2.Length2D();

	yaw = RAD2DEG(atan2(move.y, move.x));
	yaw = DEG2RAD(cmd->viewangles.y - va.y + yaw);

	cmd->move[0] = cos(yaw) * speed;
	cmd->move[1] = sin(yaw) * speed;

	if (cmd->viewangles.x < -90.f || cmd->viewangles.x > 90.f)
	{
		cmd->move.x = FloatNegate(cos(FloatNegate(yaw)) * speed);
		cmd->move.y = FloatNegate(sin(FloatNegate(yaw)) * speed);
	}

}

void m_Bots::Aimbot( CUserCmd *pCmd, bool* bSendPacket )
{


	if ( menu->M_Aim_Silent && !CanShoot() )
	{
			
		if ( should_restore )
		{
			if ( !menu->M_Aim_PSilent )
				return;

			*bSendPacket = true;

			pCmd->viewangles = bak_angView;
			pCmd->move = bak_angMove;

			bak_angView.x = 0;
			bak_angView.y = 0;
			bak_angView.z = 0;

			bak_angMove.x = 0;
			bak_angMove.y = 0;
			bak_angMove.z = 0;

			//FixMove( pCmd, pCmd->viewangles );
			should_restore = false;
			return;
		}


		return;
	}
	

	Vector vecLocalVelocity(0,0,0);
	Vector vecLocalEyePos = NetVar__GetEyePosition( g_pLocalEntity );

	if ( menu->S_PredictionType == 2 )
	{
		EstimateAbsVelocity( g_pLocalEntity, vecLocalVelocity );
		vecLocalEyePos += vecLocalVelocity * g_pGlobals->interval_per_tick;
	}

	Vector targetPos = GetTarget( vecLocalEyePos, vecLocalVelocity, pCmd );

	Vector vecAimPos = targetPos - vecLocalEyePos;
	QAngle angAimAngles(0,0,0);

	VectorAngles( vecAimPos, angAimAngles );

	if ( menu->S_Aim_Smooth > 1 )
	{

		if ( !lastTarg )
			lastTarg = pTarget;

		if ( !pTarget || lastTarg != pTarget || (pCmd->buttons & IN_ATTACK) )
		{
			lastTarg = pTarget;
			SmoothAimDelta.x = 0;
			SmoothAimDelta.y = 0;
			SmoothAimDelta.z = 0;
		}

		if ( !pTarget || menu->M_Aim_Silent && (pCmd->buttons & IN_ATTACK) )
			return;

		if ( ( angAimAngles[0] != pCmd->viewangles[0] || angAimAngles[1] != pCmd->viewangles[1] || angAimAngles[2] != pCmd->viewangles[2] ) )
		{
			float sh = (float)menu->S_Aim_Smooth;

			if ( SmoothAimDelta.x == 0 && SmoothAimDelta.y == 0 && SmoothAimDelta.z == 0 )
			{
				SmoothAimDelta.x = pCmd->viewangles.x;
				SmoothAimDelta.y = pCmd->viewangles.y;
				SmoothAimDelta.z = pCmd->viewangles.z;
			}

			SmoothAimDelta.x = ApproachAngle( angAimAngles[0], SmoothAimDelta[0], sh );
			SmoothAimDelta.y = ApproachAngle( angAimAngles[1], SmoothAimDelta[1], sh );
			SmoothAimDelta.z = ApproachAngle( angAimAngles[2], SmoothAimDelta[2], sh );

			angAimAngles = SmoothAimDelta;
		}else{
			SmoothAimDelta.x = 0;
			SmoothAimDelta.y = 0;
			SmoothAimDelta.z = 0;
		}

	}

	if ( !pTarget || menu->M_Aim_Silent && (pCmd->buttons & IN_ATTACK) )
	{
		return;
	}

	utils->NormalizeAngles( angAimAngles );

	if ( menu->M_Aim_Silent )
	{

		if ( CanShoot() )
		{
			pCmd->buttons |= IN_ATTACK;

			if ( menu->M_Aim_PSilent && !menu->M_SafeMode )
			{

				should_restore = true;

				*bSendPacket = false;
			}

			bak_angView = pCmd->viewangles;
			bak_angMove = pCmd->move;

			pCmd->viewangles = angAimAngles;

			FixMove( pCmd, bak_angView );

		}
		
		return;

	}
	
	pCmd->viewangles = angAimAngles;
	SetViewAnglesFn( pCmd->viewangles );


}

void m_Bots::Trigger( CUserCmd *pCmd )
{
	if ( (pCmd->buttons & IN_ATTACK) )
		return;

	if ( !utils->IsAlive( g_pLocalEntity ) )
		return;

	Vector Eyes, Forward;
	QAngle Angles;
	trace_t pTrace;

	Eyes = NetVar__GetEyePosition( g_pLocalEntity );
    
	Angles = pCmd->viewangles;

	Vector vDirection;

	AngleVectors( Angles, &vDirection );

	vDirection = vDirection * 8192 + Eyes;

	Ray_t pRay;
	pRay.Init( Eyes, vDirection );

	g_pEngineTrace->TraceRay( pRay, 0x46004003, NULL, &pTrace);

	if ( !pTrace.m_pEnt )
		return;
		
	if ( pTrace.m_pEnt->IsDormant() )
		return;

	if ( !utils->IsAlive( pTrace.m_pEnt ) )
		return;

	ClientClass *cc = pTrace.m_pEnt->GetClientClass();

	bool is_living = false;
	bool is_npc = false;

	ClientClass *pClientClass = pTrace.m_pEnt->GetClientClass();

	if ( !pClientClass )
		return;
		
	const char *classname = pClientClass->m_pNetworkName;
	
	if ( !classname )
		return;

	if ( menu->M_Aim_KillPlayer && utils->IsPlayerIndex(pTrace.m_pEnt->entindex()) )
		is_living = true;

	if ( menu->M_Aim_KillNPC && ( strstr(classname, "NPC") || strstr(classname, "NextBot") || ( g_AppID == 440 && strstr(classname, "Object") ) ) )
	{
		is_living = true;
		is_npc = true;
	}

	if ( !is_npc && menu->M_Aim_TeamKill == 0 )
	{

		if ( utils->IsSameTeam(g_pLocalEntity, pTrace.m_pEnt) )
			return;
	}

	if ( !is_living )
		return;

	pCmd->buttons |= IN_ATTACK;

}
