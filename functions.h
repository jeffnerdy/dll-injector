#pragma once

#include <iostream>
#include <vector>
#include <cstring>
#include <filesystem>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tlhelp32.h>

DWORD GetProcID(const char* procName);
