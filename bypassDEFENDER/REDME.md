<img width="1034" height="782" alt="image" src="https://github.com/user-attachments/assets/35c2e1db-21d5-4d10-8e21-36350555e298" />


<img width="1088" height="793" alt="Captura de tela 2026-08-10 105346" src="https://github.com/user-attachments/assets/542aabd5-62e8-4fbc-92e7-295820252995" />


Essa parte de bypass do windows defender teve que ter algumas alterações no código, principalmente na parte do shellcode, pois os bytes não passam da analise estática do windows defender
mudando a criptografia e adicionando uma chave no código é possível contornar essa primeira fase, da analíse estática 



Lembrando essa primeira etapa a gente apenas conseguiu a execução do nosso código executando o calc.exe, antes não era possível por conta do shellcode padrão





<img width="1024" height="729" alt="Captura de tela 2026-08-10 110137" src="https://github.com/user-attachments/assets/31ea986a-1a36-4701-815d-8e9956fa5933" />
