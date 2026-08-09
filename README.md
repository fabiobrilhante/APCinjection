O Que Acontece no Código

Sequência de Eventos:



<img width="544" height="402" alt="image" src="https://github.com/user-attachments/assets/09fbf4dc-82bf-427e-9781-fd9c57067d4c" />

 
 
 
 Quando a Thread Fica Alertable?
Quando você chama ResumeThread(threadHandle), o Windows:

Retoma a execução da thread principal do Notepad.exe

Inicia o processo de inicialização do Notepad (carregar DLLs, configurar ambiente, etc.)

Durante essa inicialização, a thread entra automaticamente em estado alertable em vários pontos

O Windows verifica a fila de APC e encontra a sua APC enfileirada

Executa seu payload antes mesmo do Notepad.exe mostrar a janela!





Early Bird Injection (Injeção do Pássaro Madrugador)
Essa técnica é chamada de "Early Bird" porque:

A APC é executada antes do processo alvo terminar sua inicialização

O código malicioso roda antes de muitos mecanismos de segurança serem ativados

É como se você estivesse "madrugando" para executar antes dos outros

Detalhes Técnicos:
Durante a inicialização de um processo, a thread principal:

Carrega DLLs do sistema

Inicializa o CRT (C Runtime)

Chama funções como GetMessage ou PeekMessage para criar a janela

Essas funções podem colocar a thread em estado alertable

É nesse momento que o Windows processa a APC



Resumo Final
No seu código específico:
Thread está SUSPENSA - Criada com CREATE_SUSPENDED

APC é enfileirada - Fica na fila aguardando

ResumeThread é chamada - Thread começa a executar

Durante INITIALIZATION - Windows coloca thread em estado alertable

APC é executada - Seu payload roda antes do Notepad mostrar a janela

Notepad continua - Depois da APC, o Notepad termina sua inicialização




Por que isso é poderoso?


 Execução garantida - Não depende de a thread chamar SleepEx

 Rápida - Executa segundos após criar o processo

 Silenciosa - O processo malicioso roda antes de muitos hooks de segurança

 Não cria novas threads - Menos detecção




 Essa é a essência da técnica "Early Bird APC Injection !!
