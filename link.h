#pragma once

#define GAME_DLL
#define CLIENT_DLL

#define M_RADPI 57.295779513082f
#define PI 3.141592653589793238462643383279502884197169399375105820974944592307816406286

#define SQRTFAST( x ) ( (x) * Q_rsqrt( x ) 

#define NET_MESSAGE_BITS 6

#pragma comment (lib, "mathlib.lib")
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "Dwmapi.lib")
#pragma comment(lib, "Psapi.lib")
#pragma comment(lib, "wininet")
#pragma comment(lib, "shell32")
#pragma comment(lib, "urlmon.lib")
#pragma comment(lib, "ws2_32.lib")

#pragma comment (linker, "/NODEFAULTLIB:LIBCMT")