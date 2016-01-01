// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

//#pragma once

//#include "targetver.h"

#include <stdio.h>
//#include <tchar.h>
#include <string.h>
#include <Windows.h>
#include <TlHelp32.h>

DWORD getPIDByProcessName(char*);
BOOL inject(DWORD, char*);

// TODO: reference additional headers your program requires here
