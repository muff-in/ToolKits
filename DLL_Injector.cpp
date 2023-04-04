#include <iostream>
#include <Windows.h>


using namespace std;

int main()
{

	HMODULE hKernel32 { GetModuleHandleA("kernel32.dll") };
	LPVOID lpLoadLibraryA{ GetProcAddress(hKernel32, "LoadLibraryA") };
   
	cout << "DLL Injector" << endl;
	cout << "Enter the path of the dll you want to inject: ";
	char dll_path[MAX_PATH];
	cin >> dll_path;

	cout << "Enter the pid of the process you want to inject the dll into: ";
	DWORD pid;
	cin >> pid;

	HANDLE hOpenProcess{

		OpenProcess(
			PROCESS_ALL_ACCESS,
			FALSE,
			pid
		)
	};

	if (hOpenProcess == NULL) {

		cerr << "Opening process failed with error code: " << GetLastError() << endl;
	}

	LPVOID lpAllocating{

		VirtualAllocEx(

			hOpenProcess,
			NULL,
			strlen(dll_path)+1,
			MEM_COMMIT | MEM_RESERVE,
			PAGE_EXECUTE_READWRITE
		)
	};

	if (lpAllocating == NULL) {
		cerr << "Failed to allocate memory in the target process with error code: " << GetLastError() << endl;
	}

	BOOL bWriteProcessMemory{

		WriteProcessMemory(
			hOpenProcess,
			lpAllocating,
			dll_path,
			strlen(dll_path)+1,
			NULL
		)
	};

	if (bWriteProcessMemory == 0) {

		cerr << "Writing to process failed with error code: " << GetLastError() << endl;
	}

	HANDLE hCreateRemoteThread{

		CreateRemoteThread(
			hOpenProcess,
			NULL,
			0,
			(PTHREAD_START_ROUTINE)lpLoadLibraryA,
			lpAllocating,
			0,
			NULL
			)
	};

	if (hCreateRemoteThread == NULL) {

		cerr << "Failed to create remote thread in target process with error code: " << GetLastError() << endl;

	}

	WaitForSingleObject(hCreateRemoteThread, INFINITE);

	VirtualFreeEx(hOpenProcess, lpAllocating, 0, MEM_RELEASE);
	CloseHandle(hOpenProcess);
	CloseHandle(hCreateRemoteThread);

	cout << "DLL injected successfully!" << endl;

	system("PAUSE");
	return 0;
}
