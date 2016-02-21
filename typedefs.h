#pragma once

#include "includes.h"

typedef void ( __thiscall *OriginalFireBullets )( void* thisptr, int edx, const FireBulletsInfo_t &info   );

typedef void ( __stdcall *OriginalCreateMove )( int sequence_number, float input_sample_frametime, bool active);
typedef void ( __stdcall *OriginalSetViewAngles )( QAngle &va );
typedef void ( __fastcall *OriginalDisconnect )( void *thisptr, int edx, const char *fmt );
typedef CUserCmd* ( __stdcall *OriginalGetUserCmd )( int sequence_number );

typedef void* ( __fastcall* OriginalFrameStageNotify )( void* thisptr, int edx, ClientFrameStage_t );
typedef void* ( __fastcall* OriginalRunCommand )( void* thisptr, int edx, C_BasePlayer *player, CUserCmd *ucmd, void *moveHelper );

typedef const char* (__thiscall* OriginalGetClassname)( void* thisptr );

extern OriginalFireBullets FireBulletsFn;
extern OriginalGetUserCmd GetUserCmdFn;
extern OriginalSetViewAngles SetViewAnglesFn;
extern OriginalDisconnect DisconnectFn;
extern OriginalCreateMove CreateMoveFn;
extern OriginalFrameStageNotify FrameStageNotifyFn;
extern OriginalRunCommand RunCommandFn;

extern OriginalGetClassname GetClassnameFn;