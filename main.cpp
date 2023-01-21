#include "functions.h"

int main()
{
	// getting the process name
	std::string procName;
	std::cout << "Process name (example.exe): \n";
	std::cin >> procName;
	std::cout << "\n";

	// getting the process id
	const auto procID = GetProcID(procName.c_str());

	// procID check
	if (procID)
	{
		// getting the directory path
		std::string dirPath, dllName, temp;
		char buffer[MAX_PATH];

		GetModuleFileNameA(NULL,
				   buffer,
				   MAX_PATH);

		temp = buffer;
		const auto lastSlash = temp.rfind('\\');

		if (std::string::npos != lastSlash)
		{
			temp = temp.substr(0, lastSlash);
		}

		dirPath = temp;
		temp.clear();

		// getting the dll path
		std::cout << "Dll name (example.dll): \n";
		std::cin >> dllName;
		std::cout << "\n";
		std::cin.ignore();

		temp = dirPath + "\\" + dllName;
		const auto dllPath = temp.c_str();

		// getting a handle on the process
		const auto procHandle = OpenProcess(PROCESS_ALL_ACCESS,
						    0,
						    procID);

		// procHandle check
		if (procHandle != INVALID_HANDLE_VALUE)
		{
			// allocating memory for dllPath
			const auto memRegion = VirtualAllocEx(procHandle,
							      0,
							      strlen(dllPath) + 1,
							      MEM_COMMIT | MEM_RESERVE,
							      PAGE_READWRITE);

			// memRegion check
			if (memRegion)
			{
				// storing dllPath in memRegion
				WriteProcessMemory(procHandle,
						   memRegion,
						   reinterpret_cast<LPCVOID>(dllPath),
						   strlen(dllPath) + 1,
						   0);

				// creating a remote thread to load the dll within 
				// the targeted process using 'LoadLibraryA'
				const auto threadHandle = CreateRemoteThread(procHandle,
									     0,
									     0,
									     reinterpret_cast<LPTHREAD_START_ROUTINE>(LoadLibrary),
									     memRegion,
									     0,
									     0);

				if (threadHandle != INVALID_HANDLE_VALUE)
				{
					// waiting till the thread loads the dll
					WaitForSingleObject(threadHandle, INFINITE);

					Sleep(2000);
					std::cout << "\n";
					std::cout << "Press ENTER to exit.." << std::endl;
					std::cin.get();

					// closing threadHandle
					CloseHandle(threadHandle);
				}
				else {
					std::cout << "Failed to get a handle on the thread." << std::endl;
					Sleep(2000);
				}
				// resetting/releasing memRegion
				VirtualFreeEx(procHandle,
					      reinterpret_cast<LPVOID>(memRegion),
					      0,
					      MEM_RELEASE);
			}
			else {
				std::cout << "Failed to allocate memory." << std::endl;
				Sleep(2000);
			}
			// closing procHandle
			CloseHandle(procHandle);
		}
		else {
			std::cout << "Failed to get a handle on the process." << std::endl;
			Sleep(2000);
		}
	}
	else {
		std::cout << "Failed to find the process." << std::endl;
		Sleep(2000);
	}

	return 0;
}