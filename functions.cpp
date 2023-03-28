#include "b_functions.h"

DWORD GetProcID(const char* procName)
{
    DWORD procID = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hSnap == INVALID_HANDLE_VALUE)
    {
        return procID;
    }
    
    PROCESSENTRY32 procEntry;
    procEntry.dwSize = sizeof(procEntry);

    if (Process32First(hSnap, &procEntry))
    {
        do
        {
            if (!strcmp(procEntry.szExeFile, procName))
            {
                procID = procEntry.th32ProcessID;
                break;
            }
        } while (Process32Next(hSnap, &procEntry));

    }

    CloseHandle(hSnap);
    return procID;
}
