#pragma once

template<typename T> inline T get_BP()
{
__asm mov eax, ebp
}

template<typename T> inline T get_SI()
{
__asm mov eax, esi
}


//Linker stuff
#include "link.h"

//Standard libs
#include <windows.h>
#include <wininet.h>
#include <shellapi.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <d3d9.h>
#include <d3dx9.h>
#include <Dwmapi.h> 
#include <stdlib.h>
#include <iostream>
#include <Lmcons.h>
#include <math.h>
#include <Psapi.h>
#include <windowsx.h>
#include <vector>
#include <time.h>

//Engine Stuff
#include <cbase.h>
#include <checksum_md5.h>
#include <in_buttons.h>
#include <game/server/iplayerinfo.h>
#include <inetchannel.h>
#include <inetmsghandler.h>
#include <iclient.h>

#include <input.h>
#include <igamemovement.h>

//Our Stuff
#include "jmphook.h"
#include "vmthook.h"
#include "utils.h"
#include "typedefs.h"
#include "netvar_defs.h"
#include "process_usercmd.h"
#include "prediction.h"
#include "def_globals.h"

#include "tf2.h"

using namespace std;

