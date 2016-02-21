#pragma once

#include "includes.h"

class VMTHook
{
public:
	void** m_pOriginalVTable;
	void** m_pNewVTable;
	void*** m_pInstance;

	size_t m_iNumIndices;

	//funcs

	VMTHook(void* instance);
	~VMTHook();

	//bool unhookTable( );

	int tellCount( );

	void* hookFunction(size_t iIndex, void* pfnHook);
	void* unhookFunction(size_t iIndex);
};