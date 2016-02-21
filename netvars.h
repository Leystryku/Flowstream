#pragma once

#include "includes.h"


class CNetworkedVariableManager
{
public:

    int GetOffset( const char *tableName, const char *propName );
 
 
    bool HookProp( const char *tableName, const char *propName, RecvVarProxyFn function );
 
 
private:
 
    DWORD FindProp( const char *tableName, const char *propName, RecvVarProxyFn function = 0 );
 
 
    DWORD RetProp( const char *pClassName, const char *propName, RecvVarProxyFn function = 0 );
 
 
	RecvTable *GetTable( const char *tableName );
};


extern CNetworkedVariableManager *g_pNetworkMan; 