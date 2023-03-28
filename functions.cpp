#include "functions.h"

DWORD GetProcID(const char* procName)
{
    DWORD procID;
    
    const auto snapHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (snapHandle != INVALID_HANDLE_VALUE)
    {
        PROCESSENTRY32 procEntry;
        procEntry.dwSize = sizeof(procEntry);

        if (Process32First(snapHandle, &procEntry))
        {
            do
            {
                if (!strcmp(procEntry.szExeFile, procName))
                {
                    procID = procEntry.th32ProcessID;
                    break;
                }
            } while (Process32Next(snapHandle, &procEntry));

        }
    }

    CloseHandle(snapHandle);
    return procID;
}
