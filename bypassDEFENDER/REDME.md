<h1>bypass windoews defender </h1>


<h1></b>antes não era possível passar pelo windows defender por causa do shellcode padrão, a gente era pego pela análise estática no nosso malware </b></h1>


<img width="1024" height="729" alt="Captura de tela 2026-08-10 110137" src="https://github.com/user-attachments/assets/31ea986a-1a36-4701-815d-8e9956fa5933" />





<>*Essa parte de bypass do windows defender teve que ter algumas alterações no código, principalmente na parte do shellcode, pois os bytes não passam da analise estática do windows defender
mudando a criptografia e adicionando uma chave no código é possível contornar essa primeira fase, da analíse estática*



<img width="1034" height="782" alt="image" src="https://github.com/user-attachments/assets/35c2e1db-21d5-4d10-8e21-36350555e298" />


<img width="1088" height="793" alt="Captura de tela 2026-08-10 105346" src="https://github.com/user-attachments/assets/542aabd5-62e8-4fbc-92e7-295820252995" />

