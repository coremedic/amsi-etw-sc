#include "Common.h"

EXTERN_C
SECTION(B)
VOID PreMain() {
    INSTANCE instance = {0};

    ZeroMem(&instance, sizeof(INSTANCE));

    if ((instance.Win32.Modules.Ntdll = LdrModulePeb(HASHW(L"ntdll.dll")))) {
        if (!(instance.Win32.Api.RtlAddVectoredExceptionHandler = LdrFunction(instance.Win32.Modules.Ntdll, HASHA("RtlAddVectoredExceptionHandler")))) {
            return;
        }
    }

    if ((instance.Win32.Modules.Kernel32 = LdrModulePeb(HASHW(L"kernel32.dll")))) {
#ifdef DEBUG
        if (!(instance.Win32.Api.LoadLibraryA = LdrFunction(instance.Win32.Modules.Kernel32, HASHA("LoadLibraryA")))) {
            return;
        }
#endif
    }

#ifdef DEBUG
    instance.Win32.Modules.User32 = instance.Win32.Api.LoadLibraryA("user32.dll");
    instance.Win32.Api.MessageBoxA = LdrFunction(instance.Win32.Modules.User32, HASHA("MessageBoxA"));

#endif

    auto stackInstance = GlobalData(instance);
    auto error = GlobalInstance::Init(&stackInstance);

    Main();
}
