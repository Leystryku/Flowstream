#pragma once

#include "netvars.h"

CNetworkedVariableManager *g_pNetworkMan;

// calls GetProp wrapper to get the absolute offset of the prop
int CNetworkedVariableManager::GetOffset( const char *tableName, const char *propName )
{
    int offset = FindProp( tableName, propName );
 
    if ( !offset )
    {
		Msg( "Failed to find offset for prop: %s from table: %s\n", propName, tableName );
        return 0;
    }

    return offset;
}
 
 
// calls GetProp wrapper to get prop and sets the proxy of the prop
bool CNetworkedVariableManager::HookProp( const char *tableName, const char *propName, RecvVarProxyFn function )
{
	DWORD dwOffset = FindProp( tableName, propName, function );
	
 
    if ( !dwOffset )
    {
        Msg( "Failed to hook prop: %s from table: %s\n", propName, tableName );
        return false;
    }
 
    return true;
}
 
 
// wrapper so we can use recursion without too much performance loss
DWORD CNetworkedVariableManager::FindProp( const char *tableName, const char *propName, RecvVarProxyFn function )
{
    RecvTable *recvTable = GetTable( tableName );
 
    if ( !recvTable && !( !strcmp(tableName, "*") ) )
    {
		Msg( "Failed to find table: %s\n", tableName );

        return 0;
    }
 
 
    DWORD offset = RetProp( tableName, propName, function );
 
    if ( !offset )
    {
		Msg( "Failed to find prop: %s from table: %s\n", propName, tableName );
        return 0;
    }
 
 
    return offset;
}
 
 
// uses recursion to return the relative offset to the given prop and sets the prop param

DWORD GetNetOffset( const char* pClassName, const char* pVarName, RecvTable* pTable, RecvVarProxyFn function )
{

	DWORD dwRet = NULL;

	for( int i = 0; i < pTable->m_nProps; i++ )
	{
		RecvProp* pProp = &pTable->m_pProps[i];

		if( !pProp )
			continue;

		RecvTable* pChild = pProp->m_pDataTable;

		if( pChild && pChild->m_nProps )
		{
			dwRet += GetNetOffset( pClassName, pVarName, pChild, function );
		}

		if( strcmp( pTable->m_pNetTableName, pClassName ) == 0 || !strcmp(pClassName, "*") )
		{
			if ( strcmp( pProp->m_pVarName, pVarName ) == 0 )
			{
				dwRet += pProp->GetOffset( );

				if ( pProp && function )
				{
					pProp->m_ProxyFn = function;
				}

				return dwRet;
			}
		}
	}

	return dwRet;
}

DWORD CNetworkedVariableManager::RetProp( const char *pClassName, const char *propName, RecvVarProxyFn function )
{

	ClientClass* clientClass = g_pClient->GetAllClasses( );

	DWORD dwRet = NULL;

	if( !clientClass )
		return dwRet;

	for( ; clientClass; clientClass = clientClass->m_pNext )
	{
		RecvTable* recTable = clientClass->m_pRecvTable;

		if( !recTable )
			continue;

		if( !recTable->m_nProps )
			continue;

		DWORD offset = GetNetOffset( pClassName, propName, recTable, function);

		if( offset )
		{
			dwRet = offset;
			break;
		}
	}

	return dwRet;
}

RecvTable *GetTable2( RecvTable* table, const char *tableName )
{
	RecvTable *retTable = 0;

	if ( strcmp( table->m_pNetTableName, tableName ) == 0 )
		return table;

	for( int i = 0; i < table->m_nProps; i++ )
	{
		RecvProp* pProp = &table->m_pProps[i];

		if( !pProp )
			continue;

		RecvTable* pChild = pProp->GetDataTable();
		
		if ( !pChild )
			continue;

		 if ( strcmp( pChild->m_pNetTableName, tableName ) == 0 )
			 retTable = pChild;

		 if ( pChild->m_nProps )
		 {
			RecvTable *gettbl = GetTable2( pChild, tableName );
			if ( gettbl )
				retTable = gettbl;
		 }

	}

	return retTable;
}

RecvTable *CNetworkedVariableManager::GetTable( const char *tableName )
{

    ClientClass *clientClass = g_pClient->GetAllClasses();
 
    if ( !clientClass )
    {
		Msg( "ClientClass was not found\n" );
        return 0;
    }
 
 	for( ; clientClass; clientClass = clientClass->m_pNext )
	{
		RecvTable *recvTable = clientClass->m_pRecvTable;

		if ( !recvTable )
			continue;

		if( !recvTable->m_nProps )
			continue;

        if ( strcmp( recvTable->m_pNetTableName, tableName ) == 0 )
        {
            return recvTable;
        }

		RecvTable *inside = GetTable2( recvTable, tableName );

		if ( inside )
			return inside;

	}

    return 0;
}