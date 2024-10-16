#include <iostream>
#include <windows.h>
#include <lm.h>

#pragma comment(lib, "ntdll.lib")

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
   //FARPROC addr = GetProcAddress(LoadLibraryA("ntdll"), "NtTerminateProcess");

    //const wchar_t* username = L"NewUser";  // 要添加的用户名
    //const wchar_t* password = L"Password123";  // 用户密码

    //DWORD targetProcessId = 3860;

    // 如果参数不足或用户请求帮助
    if (argc != 2 || strcmp(argv[1], "--help") == 0) {
        printUsage();
        return 1;
    }

    // 将传入的参数转换为目标进程ID
    DWORD targetProcessId = atoi(argv[1]);

    if (targetProcessId == 0) {
        std::cerr << "Invalid ProcessID. Please provide a valid numeric ProcessID.\n";
        return 1;
    }

    // 打开目标进程的句柄，具有终止权限才行，杀不了比如360  火绒==，需要另外的方式，这里是为了学习直接syscall技术，kill av这里不做讨论
    //通过这个思路，这个可以做"其他好玩"的事情
    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, targetProcessId);
    if (hProcess == NULL) {
        std::cerr << "Failed to open process. Error: " << GetLastError() << std::endl;
        return 1;
    }

    // 调用汇编函数终止目标进程
    NTSTATUS status = SysNtTerminateProcessFunc(hProcess, 0); // 0 表示正常退出

    if (status == 0) { // STATUS_SUCCESS
        std::cout << "Process terminated successfully." << std::endl;
    }
    else {
        std::cerr << "Failed to terminate process. NTSTATUS: " << std::hex << status << std::endl;
    }

    // 关闭进程句柄
    CloseHandle(hProcess);

   // SysNtTerminateProcessFunc();

    system("pause");

    return 0;
}
