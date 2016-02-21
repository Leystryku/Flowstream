#include "vmthook.h"

VMTHook::VMTHook(void* instance)
{

	if( !instance)
		return;

	m_pInstance = (void***) instance;

	if ( !m_pInstance )
		return;

	m_pOriginalVTable = *m_pInstance;

	if ( !m_pOriginalVTable )
		return;

	//Count number of Pointers in the table
 
	int num = 0;
 
	while(m_pOriginalVTable[num])
	{
		num++;
	}

	m_iNumIndices = num;

	if ( m_iNumIndices == 0 )
		return;

	//Allocate memory on the heap for our own copy of the table
	int size = sizeof(void*) * m_iNumIndices;

	m_pNewVTable = (void**) malloc(size);

	if ( !m_pNewVTable )
		return;
	
	memcpy(m_pNewVTable, m_pOriginalVTable, size);
	*m_pInstance=m_pNewVTable;

}

VMTHook::~VMTHook()
{

	if ( !m_pOriginalVTable )
		return;

	//Rewrite old pointer
	free(m_pNewVTable);
	*m_pInstance = m_pOriginalVTable;

	return;

}

int VMTHook::tellCount( )
{
	/*
	int m_iNumIndices = 0;
 
	while(m_pOriginalVTable[m_iNumIndices])
	{
		m_iNumIndices++;
	}*/

	return m_iNumIndices;
}

void* VMTHook::hookFunction(size_t iIndex, void* pfnHook)
{

	//Valid index?
	if(iIndex >= m_iNumIndices)
		return NULL;

	if ( !m_pOriginalVTable[iIndex] )
		return NULL;

	//Write new pointer
	m_pNewVTable[iIndex] = pfnHook;
 
	//And return pointer to original function
	return m_pOriginalVTable[iIndex];
}

void* VMTHook::unhookFunction(size_t iIndex)
{

	//Valid index?
	if(iIndex >= m_iNumIndices)
		return NULL;

	if ( !m_pOriginalVTable[iIndex] )
		return NULL;

	//Rewrite old pointer
	m_pNewVTable[iIndex] = m_pOriginalVTable[iIndex];
	
	//And return pointer to original function
	return m_pOriginalVTable[iIndex];
}