# include "jam.h"
# include "lists.h"
# include "newstr.h"
# include "parse.h"
# include "frames.h"

# define WIN32_LEAN_AND_MEAN
# include <windows.h>

# define  MAX_REGISTRY_DATA_LENGTH  4096

typedef struct _KeyMap
{
    LPCSTR  szName;
    HKEY    hValue;
} KeyMap;

static const KeyMap  dlRootKeys[] = {
    { "HKLM", HKEY_LOCAL_MACHINE },
    { "HKCU", HKEY_CURRENT_USER },
    { "HKCR", HKEY_CLASSES_ROOT },
    { "HKEY_LOCAL_MACHINE", HKEY_LOCAL_MACHINE },
    { "HKEY_CURRENT_USER", HKEY_CURRENT_USER },
    { "HKEY_CLASSES_ROOT", HKEY_CLASSES_ROOT },
    { 0, 0 }
};

LIST*
builtin_system_registry(
    PARSE    *parse,
    FRAME    *frame )
{
    HKEY   hKey, hRootKey = 0;
    LIST   *dlResult = L0;
    LPSTR  szKey;

    {
    const KeyMap  *pdSearchKey = dlRootKeys;
    LPSTR  szRootKey = newstr( lol_get(frame->args, 0)->string );

    szKey = strchr(szRootKey , '\\');
    *szKey = '\0'; ++szKey;

    do {
        if ( !strcmp(szRootKey, pdSearchKey->szName) )
        { hRootKey = pdSearchKey->hValue; break; }
        else  ++pdSearchKey;
    } while (pdSearchKey->szName);
    }

    if ( hRootKey && ERROR_SUCCESS ==
         RegOpenKeyEx(hRootKey, szKey, 0, KEY_QUERY_VALUE, &hKey) )
    {
        DWORD  dwType, dwSize;
        BYTE   bsDataValue[MAX_REGISTRY_DATA_LENGTH];
        LPSTR  szKeyValue;
        LIST  *lKeyValue = lol_get(frame->args, 1);
        
        if (lKeyValue)  szKeyValue = lKeyValue->string;
        else  szKeyValue = NULL;

        if ( ERROR_SUCCESS ==
             RegQueryValueEx(hKey, szKeyValue, 0, &dwType, bsDataValue, &dwSize) )
        {
            switch (dwType)
            {
            case REG_EXPAND_SZ:
                 {
                 CHAR  szResult[MAX_REGISTRY_DATA_LENGTH];
                 ExpandEnvironmentStrings( (LPCSTR)bsDataValue,
                     szResult, MAX_REGISTRY_DATA_LENGTH );

                 dlResult = list_new( dlResult, newstr(szResult) ); break;
                 }
            case REG_MULTI_SZ:
                 {
                 LPSTR  pszString = (LPSTR)bsDataValue;
                 while (*pszString)
                 {
                     dlResult = list_new( dlResult, newstr(pszString) );
                     pszString = pszString + strlen(pszString);
                     ++pszString;
                 }
                 break;
                 }
            case REG_DWORD: sprintf( bsDataValue, "%u", *(PDWORD)bsDataValue );
            default: dlResult = list_new( dlResult, newstr(bsDataValue) ); break;
            }
        }
        RegCloseKey(hKey);
    }
    return  dlResult;
}