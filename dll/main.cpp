#include <windows.h>
#include "injector/injector.h"
#include "jvm/jni.h"

DWORD WINAPI InjectorThread(LPVOID module_lpvoid) {
    auto module = static_cast<HMODULE>(module_lpvoid);
    FindJVMsAndInject();
    FreeLibraryAndExitThread(module, 0);
}


BOOL APIENTRY DllMain(HMODULE module, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        CreateThread(NULL, 0, &InjectorThread, module, 0, NULL);
    }

    return TRUE;
}
