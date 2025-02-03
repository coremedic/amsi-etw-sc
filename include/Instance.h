#ifndef AMSI_ETW_SC_INSTANCE_H
#define AMSI_ETW_SC_INSTANCE_H

#include <windows.h>
#include "Ntdll.h"
#include "Macros.h"

typedef struct _INSTANCE {
    struct {
        struct {
            API(RtlAddVectoredExceptionHandler)
#ifdef DEBUG
            API(LoadLibraryA)
            API(MessageBoxA)
#endif
        } Api;

        struct {
            PVOID Ntdll;
            PVOID Kernel32;
#ifdef DEBUG
            PVOID User32;
#endif
        } Modules;
    } Win32;
} INSTANCE, *PINSTANCE;

template<typename T>
struct GlobalData {
    UINT32 magic;
    T context;

    static constexpr
    UINT32 MAGIC = 0x17171717;

    explicit
    GlobalData(const T &context) : magic{MAGIC}, context{context} {}

    static
    NTSTATUS Init(_Inout_ GlobalData<T>* data) {
        if (!data) {
            return STATUS_INVALID_PARAMETER;
        }

        auto peb = NtCurrentPeb();
        if (peb->NumberOfHeaps == peb->MaximumNumberOfHeaps) {
            return STATUS_NO_MEMORY;
        }

        peb->ProcessHeaps[peb->NumberOfHeaps++] = data;

        return STATUS_SUCCESS;
    }

    static
    T* Get() {
        auto peb = NtCurrentPeb();
        for (UINT32 i = 0; i < peb->NumberOfHeaps; ++i) {
            auto* data = reinterpret_cast<GlobalData*>(peb->ProcessHeaps[i]);
            if (data->magic == MAGIC) {
                return &data->context;
            }
        }
        return NULL;
    }
};

using GlobalInstance = GlobalData<INSTANCE>;

#endif //AMSI_ETW_SC_INSTANCE_H
