// DllInjector.cpp : Defines the entry point for the console application.
//

#include "DllInjector.h"

DWORD getPIDByProcessName(char * name)
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,NULL);
	if(hSnapshot)
	{
		PROCESSENTRY32 lppe;
		lppe.dwSize = sizeof(PROCESSENTRY32);
		while(Process32Next(hSnapshot,&lppe))
		{
			if(strcmp(lppe.szExeFile,name)==0)
				return lppe.th32ProcessID;
		}
		CloseHandle(hSnapshot);
	}
	return 0;
}

BOOL inject(DWORD processID, char * filename)
{
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, processID);
	if(hProcess)
	{
		FARPROC LoadLibAddr = GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");
		if(!LoadLibAddr)
			printf("Failed to load library");

		LPVOID baseAddr = VirtualAllocEx(hProcess, NULL, 4096, MEM_COMMIT, PAGE_READWRITE);
		if(!baseAddr)
			printf("Failed to allocate memory");

		if(!WriteProcessMemory(hProcess,baseAddr,filename,4096,NULL))
			printf("Failed to write memory");

		if(!CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLibAddr, baseAddr, 0, NULL))
			printf("Failed to create thread");

		CloseHandle(hProcess);
		return true;
	}
	return false;
}

int main(int argc, char* argv[])
{
	char * fileName = NULL;
	char * processName = NULL;
	if(argc)
	{
		for(int i = 0; i < argc; i++)
		{
			if(strcmp(argv[i],"-f") == 0)
				fileName = argv[i+1];
			else if(strcmp(argv[i],"-p") == 0)
				processName = argv[i+1];
		}
		if(fileName && processName)
		{
			DWORD pid = getPIDByProcessName(processName);
			if(pid)
			{
				printf("found %s with a process id of: %d\n", processName, pid);
				if(inject(pid,fileName))
					printf("Successfully injected process %s with %s\n",processName,fileName);
			}
			else
				printf("Process %s not found",processName);
		}
	}
	return 0;
}

