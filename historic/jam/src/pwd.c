#include "jam.h"
#include "lists.h"
#include "newstr.h"

#ifdef NT
#include <direct.h>
#define PATH_MAX _MAX_PATH
#else
#include <unistd.h>
#endif

#include <limits.h>

#ifdef NT
#include <tchar.h>
#include <windows.h>

#ifndef INVALID_FILE_ATTRIBUTES
#define INVALID_FILE_ATTRIBUTES ((DWORD)-1)
#endif


DWORD ShortPathToLongPath(LPCTSTR lpszShortPath,LPTSTR lpszLongPath,DWORD
                          cchBuffer)
{
    DWORD i=0;
    TCHAR path[_MAX_PATH]={0};
    TCHAR ret[_MAX_PATH]={0};
    DWORD pos=0;
    DWORD len=_tcslen(lpszShortPath);

    // Is the string valid?
    if (!lpszShortPath) {
        SetLastError(ERROR_INVALID_PARAMETER);
        return 0;  
    }

    // Is the path valid?
    if (GetFileAttributes(lpszShortPath)==INVALID_FILE_ATTRIBUTES)
        return 0;

    // Convert "/" to "\"
    for (i=0;i<len;++i) {
        if (lpszShortPath[i]==_T('/')) 
            path[i]=_T('\\');
        else
            path[i]=lpszShortPath[i];
    }

    // UNC path?
    if (path[0]==_T('\\') && path[1]==_T('\\')) {
        pos=2;
        for (i=0;i<2;++i) {
            while (path[pos]!=_T('\\') && path[pos]!=_T('\0'))
                ++pos;
            ++pos;
        }
        _tcsncpy(ret,path,pos-1);
    } // Drive letter?
    else if (path[1]==_T(':')) {
        if (path[2]==_T('\\'))
            pos=3;
        if (len==3) {
            if (cchBuffer>3)
                _tcscpy(lpszLongPath,lpszShortPath);
            return len;
        }
        _tcsncpy(ret,path,2);
    }

    // Expand the path for each subpath, and strip trailing backslashes
    for (;pos<=len;++pos) {
        if (path[pos]==_T('\\') || (path[pos]==_T('\0') &&
                                    path[pos-1]!=_T('\\'))) {
            WIN32_FIND_DATA fd;
            HANDLE hf=0;
            TCHAR c=path[pos];
            path[pos]=_T('\0');

            hf=FindFirstFile(path, &fd);
            if (hf==INVALID_HANDLE_VALUE)
                return 0;

            // Append the result
            _tcscat(ret,_T("\\"));
            _tcscat(ret,fd.cFileName);
            path[pos]=c;
            FindClose(hf);
        }
    }
 
    len=_tcslen(ret)+1;
    if (cchBuffer>=len)
        _tcscpy(lpszLongPath,ret);
    
    return len;
}
#endif

LIST*
pwd(void)
{
    char buffer[PATH_MAX];
    if (getcwd(buffer, sizeof(buffer)) == NULL)
    {
        perror("can not get current directory");
        return L0;
    }
    else
    {
#ifdef NT
        char buffer2[_MAX_PATH];
        ShortPathToLongPath(buffer, buffer2, _MAX_PATH);
        return list_new(L0, newstr(buffer2));
#else
        return list_new(L0, newstr(buffer));
#endif
    }
}

