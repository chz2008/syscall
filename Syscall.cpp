#include <iostream>
#include <windows.h>

#pragma comment(lib, "ntdll")

EXTERN_C NTSTATUS SysNtTerminateProcessFunc(
    HANDLE ProcessHandle,
    NTSTATUS ExitStatus
);


// 打印帮助信息
void printUsage() {
    std::cout << "Usage: terminate_process <ProcessID>\n";
    std::cout << "Terminate a process by specifying its ProcessID.\n\n";
    std::cout << "Options:\n";
    std::cout << "  --help       Show this help message\n";
}

int main(int argc, char* argv[]) {
    if (argc != 2 || strcmp(argv[1], "--help") == 0) {
        printUsage();
        return 1;
    }

    DWORD targetProcessId = atoi(argv[1]);

    if (targetProcessId == 0) {
        std::cerr << "Invalid ProcessID. Please provide a valid numeric ProcessID.\n";
        return 1;
    }

    // 具有终止权限才行，杀不了比如360  火绒==，需要另外的方式，这里是为了学习syscall，kill av这里不做讨论
    //通过这个思路，这个可以做"其他好玩"的事情
    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, targetProcessId);
    if (hProcess == NULL) {
        std::cerr << "Failed to open process. Error: " << GetLastError() << std::endl;
        return 1;
    }

    NTSTATUS status = SysNtTerminateProcessFunc(hProcess, 0); 

    if (status == 0) { 
        std::cout << "Process terminated successfully." << std::endl;
    }
    else {
        std::cerr << "Failed to terminate process. NTSTATUS: " << std::hex << status << std::endl;
    }

    // 关闭进程句柄
    CloseHandle(hProcess);
    return 0;
}
