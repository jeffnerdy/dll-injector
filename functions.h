#pragma once

#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <windows.h>
#include <tlhelp32.h>

DWORD GetProcID(const char* procName);