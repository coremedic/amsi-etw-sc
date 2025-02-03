#include "Common.h"

SECTION(B)
VOID Main() {
    PINSTANCE pInstance = GlobalInstance::Get();

    pInstance->Win32.Api.MessageBoxA(NULL, "AYOO", "AYOOO", MB_OK | MB_ICONINFORMATION);
    return;
}