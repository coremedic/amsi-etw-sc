#include "Common.h"

EXTERN_C
SECTION(B)
VOID PreMain() {
    INSTANCE instance = {0};

    if ((instance.Win32.Modules.Ntdll = LdrModulePeb(HASHW(L"ntdll.dll")))) {
        if (!(instance.Win32.Api.RtlAddVectoredExceptionHandler = LdrFunction(instance.Win32.Modules.Ntdll, HASHA("RtlAddVectoredExceptionHandler")))) {
            return;
        }
    }

    auto stackInstance = GlobalData(instance);
    auto error = GlobalInstance::Init(&stackInstance);

    Main();
}
