#pragma once
#include "includes.h"

/*
	
	Virtual Hooks
	
	Version ALPHA (26.01.2014):
		No function pointer mod
		No recursion support


	Version ALPHA (29.01.2014):
		No function pointer mod
		No recursion support
		Added ability for all __stdcall-like functions (__thiscall, __fastcall) whatever

	DEPRECATED (02.02.2014):
		JMP Hooks are detected by VAC2 since it was created

	Copyleft (C) NanoCat 2014

*/

//Thank you Catscan :3

class JMPHook
{
public:
	void* _target;
	void* _this;
	
	unsigned char org[5];
	unsigned char mod[5];

	DWORD jmp;
	DWORD oldp;


	JMPHook(void* pointer, void* hook);
	~JMPHook();
	
	void SetECX(void* ecx = 0);
	void* Call(int n = 0, ...);
};