#include "b_functions.h"

int main()
{
	// getting the process id
	std::string procName;
	std::cout << "process name (example.exe): \n";
	std::getline(std::cin, procName);
	std::cout << "\n";

	const auto procID = GetProcID(procName.c_str());

	if (!procID)
	{
		std::cout << "error: failed to find the process." << std::endl;
		Sleep(2000);

		return 0;
	}

	// getting the directory path
	std::string dllName, temp;
	std::cout << "dll name (example.dll): \n";
	std::getline(std::cin, dllName);
	std::cout << "\n";

	temp = std::filesystem::current_path().string() + '\\' + dllName;
	auto dllPath = temp.c_str();

	// getting a handle on the process
	const auto procHandle = OpenProcess(PROCESS_ALL_ACCESS, 0, procID);

	if (procHandle == INVALID_HANDLE_VALUE)
	{
		std::cout << "error: failed to get a handle on the process." << std::endl;
		Sleep(2000);

		return 0;
	}

	// allocating memory for dllPath
	const auto memRegion = VirtualAllocEx(procHandle, 0, strlen(dllPath) + 1, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

	if (!memRegion)
	{
		std::cout << "error: failed to allocate memory." << std::endl;
		Sleep(2000);

		return 0;
	}
	
	// storing dllPath in memRegion
	WriteProcessMemory(procHandle, memRegion, reinterpret_cast<LPCVOID>(dllPath), strlen(dllPath) + 1, 0);

	// loading the dll
	const auto threadHandle = CreateRemoteThread(procHandle, 0, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(LoadLibrary), memRegion, 0, 0);

	if (threadHandle == INVALID_HANDLE_VALUE)
	{
		std::cout << "error: failed to get a handle on the thread." << std::endl;
		Sleep(2000);

		return 0;
	}
	
	// epilogue
	WaitForSingleObject(threadHandle, INFINITE);
	CloseHandle(threadHandle);
	VirtualFreeEx(procHandle, reinterpret_cast<LPVOID>(memRegion), 0, MEM_RELEASE);
	CloseHandle(procHandle);

	std::cout << "bye bye!" << std::endl;
	Sleep(500);

	return 0;
}
