#pragma once

#include "includes.h"

class m_AntiAim
{
public:

	void DoAntiAim( CUserCmd *pCmd, bool* bSendPacket );
	
};

extern m_AntiAim *AntiAim;