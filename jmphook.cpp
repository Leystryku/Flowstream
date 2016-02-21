#include "JMPHook.h"

#define UNHOOK()	memcpy(_target, org, 5)
#define REHOOK()	memcpy(_target, mod, 5)

JMPHook::JMPHook(void* pointer, void* hook)
{
	_this	= 0;
	_target = pointer;

	jmp = 0 - ((DWORD)_target - (DWORD)hook) - 5;
	mod[0] = 0xE9;

	memcpy(org, _target, 5);
	memcpy((void*)((DWORD)mod + 1), &jmp, 4);

	VirtualProtect(_target, 5, PAGE_EXECUTE_READWRITE, &oldp);
	REHOOK();

}

JMPHook::~JMPHook()
{
	UNHOOK();
	VirtualProtect(_target, 5, oldp, &oldp);
}

void JMPHook::SetECX(void* ecx)
{
	_this = ecx;
}

void* JMPHook::Call(int n, ...)
{
	UNHOOK();
	
	va_list v;
	va_start(v, n);

	v += n * 4 + 4;

	for (int i = 0; i < n; i++)
	{
		void* a = *(void**)((v -= 4) - 4);
		__asm PUSH a
	}
	
	void* m = _target;
	void* t = _this;
	void* r;

	if (t)
		__asm MOV ECX, t

	__asm
	{
		CALL m
		MOV r, EAX
	}

	va_end(v);
	REHOOK();

	return r;
}