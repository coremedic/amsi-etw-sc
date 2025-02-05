#include "Common.h"

SECTION(B)
PBYTE GetFunctionArgument(IN PCONTEXT pThreadCtx, IN DWORD dwParmIndex) {

    // The first 4 arguments in x64 are in the "RCX - RDX - R8 - R9" registers
    switch (dwParmIndex) {
        case 0x01:
            return (ULONG_PTR)pThreadCtx->Rcx;
        case 0x02:
            return (ULONG_PTR)pThreadCtx->Rdx;
        case 0x03:
            return (ULONG_PTR)pThreadCtx->R8;
        case 0x04:
            return (ULONG_PTR)pThreadCtx->R9;
        default:
            break;
    }

    // Else more arguments are pushed to the stack
    return *(ULONG_PTR*)(pThreadCtx->Rsp + (dwParmIndex * sizeof(PVOID)));
}

SECTION(B)
VOID SetFunctionArgument(IN PCONTEXT pThreadCtx, IN ULONG_PTR uValue, IN DWORD dwParmIndex) {

    // The first 4 arguments in x64 are in the "RCX - RDX - R8 - R9" registers
    switch (dwParmIndex) {
        case 0x01:
            (ULONG_PTR)pThreadCtx->Rcx = uValue; return;
        case 0x02:
            (ULONG_PTR)pThreadCtx->Rdx = uValue; return;
        case 0x03:
            (ULONG_PTR)pThreadCtx->R8 = uValue; return;
        case 0x04:
            (ULONG_PTR)pThreadCtx->R9 = uValue; return;
        default:
            break;
    }

    // Else more arguments are pushed to the stack
    *(ULONG_PTR*)(pThreadCtx->Rsp + (dwParmIndex * sizeof(PVOID))) = uValue;
}

SECTION(B)
DWORD _GetCurrentProcessId() {
    return (DWORD)(__readgsdword(0x40));
}

SECTION(B)
DWORD _GetCurrentThreadId() {
    return (DWORD)(__readgsdword(0x48));
}

SECTION(B)
HANDLE _GetProcessHeap() {
    auto pPeb = NtCurrentPeb();
    return (HANDLE)pPeb->ProcessHeap;
}

SECTION(B)
unsigned long long SetDr7Bits(unsigned long long CurrentDr7Register, int StartingBitPosition, int NmbrOfBitsToModify, unsigned long long NewBitValue) {
    unsigned long long mask = (1UL << NmbrOfBitsToModify) - 1UL;
    unsigned long long NewDr7Register = (CurrentDr7Register & ~(mask << StartingBitPosition)) | (NewBitValue << StartingBitPosition);

    return NewDr7Register;
}

