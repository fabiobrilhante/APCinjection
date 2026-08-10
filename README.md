# APC Code Injection com Payload XOR

Este projeto demonstra uma técnica de injeção de código no Windows utilizando **APC (Asynchronous Procedure Call)** em conjunto com **criptografia XOR** para ofuscar o payload no binário.

## Funcionamento

1. **Payload Criptografado:** O shellcode é armazenado no binário criptografado com a chave `"tept"` para dificultar a detecção estática por assinaturas.

2. **Descriptografia:** O payload é descriptografado em memória dentro do processo injetor usando a função `DecryptPayload()` antes da injeção.

3. **Processo Suspenso:** O processo alvo (`notepad.exe`) é criado em estado suspenso com `CreateProcessA` e a flag `CREATE_SUSPENDED`, permitindo a injeção antes da execução da thread principal.

4. **Alocação de Memória:** `VirtualAllocEx` aloca memória no processo alvo com permissões `PAGE_EXECUTE_READWRITE`.

5. **Escrita do Payload:** `WriteProcessMemory` escreve o payload **já descriptografado** na região de memória alocada.

6. **APC Injection:** `QueueUserAPC` enfileira uma APC que aponta para o endereço do payload injetado.

7. **Execução:** `ResumeThread` retoma a thread suspensa, que executa a APC e aciona o payload (MessageBox demonstrativo).

## Técnicas de Evasão

- XOR encryption para ocultar o payload estático no binário.
- Injeção em processo legítimo do Windows (notepad.exe).
- APCs para execução furtiva (não cria novas threads).
- Descriptografia em memória (não em disco).
