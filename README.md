# Projeto: Geração e envio de dados do sensor BMX055 através do LARA R211, usando HTTPS para o Firebase, com ESP32
# Autor: Diogo Ferreira
# Data de Criação: 05/05/2025

## Estrutura de Pastas

📁 LARA_R211_Servidor_BMX055  
├── 📁 BMX055  
│    ├── 📁 guarda_valores_serial  
│    │    ├── guarda_valores_excel.py  
│    │    └── guarda_valores_txt.py  
│    │  
│    ├── 📁 myLibrary_BMX055  
│    │    ├── i2c_address_finder.cpp  
│    │    ├── initialization.cpp  
│    │    ├── orientacao.cpp  
│    │    ├── readBMX055.cpp  
│    │    └── self_test.cpp  
│    │  
│    ├── config.h  
│    ├── includes_myLibrary_BMX055.h  
│    └── print_guarda.h  
│  
├── 📁 LARA_R211  
│    ├── 📁 codigo_cpp_r211  
│    │    ├── certificado.cpp  
│    │    ├── cria_apaga_arquivos_r211.cpp  
│    │    ├── horas_r211.cpp  
│    │    ├── http_https.cpp  
│    │    ├── ligar_4g_3g_2g_automatico.cpp  
│    │    ├── mandar_comandos.cpp  
│    │    ├── power_r211.cpp  
│    │    └── sms.cpp  
│    │  
│    ├── 📁 codigo_h_r211  
│    │    ├── certificado.h  
│    │    ├── cria_apaga_arquivos_r211.h  
│    │    ├── horas_r211.h  
│    │    ├── http_https.h  
│    │    ├── ligar_4g_3g_2g_automatico.h  
│    │    ├── mandar_comandos.h  
│    │    ├── power_r211.h  
│    │    └── sms.h  
│    ├── includes_codigo_cpp_r211.h  
│    └── includes_codigo_h_r211.h   
├── 📁 RTC  
│    ├── horas_rtc.cpp  
│    └── horas_rtc.h  
├── definicoes.h
├── LARA_R211_Servidor_BMX055.ino
└── README.txt

## Descrição do Projeto
Este projeto realiza a aquisição e o envio de dados do sensor BMX055 para o Firebase utilizando o módulo LARA R211 via HTTPS. A comunicação é realizada de forma segura, com os dados sendo enviados periodicamente para o banco de dados em tempo real. O sistema integra a leitura do sensor BMX055, o controlo do módulo LARA R211 e a utilização do RTC (Relógio de Tempo Real) para o controlo de horários de transmissão.


## Como Usar
1. Conecte o BMX055,LARA R211,RTC ao microcontrolador conforme a definição dos pinos em `definicao_pinos.h`.
2. Caso esteja a usar a logica de 3.3v no LARA, devera fornecer-lhe uma tensão de 5V (tem de dar para 2A), e também 
uma tensão de 3.3V, pode usar um divisor de tensão.
3. Ler o arquivo `BMX055/config.h`, ele tem os parâmetros necessários para configurar o sensor BMX055,
esses parâmetros são colocados no LARA_R211_Servidor_BMX055.ino.
4. É necessário configurar o numero de telefone para qual o LARA mandará confirmações e receberá sms no ficheiro definições.h
5. Abra o http_https.cpp, tem de configurar o certificado a usar (se ainda não o carregou usar o ## a baixo)
6. Configurar link da base de dados, trocar ou manter a APN do cartão sim no definições.h
7. Configurar o caminho, que quer para os dados a mandar no http_https.cpp 
8. Compile e faça o upload do código `LARA_R211_Servidor_BMX055.ino` para o microcontrolador.
9. O sistema ira mandar uma mensagem a dizer "Setup concluído!", neste momento podemos enviar o sms "ligar https", ele aqui vai configurar o lara para a comunicao https,
iremos ter de receber o sms "https iniciado!". Depois podemos enviar o sms "enviar https", iremos receber um sms, "A enviar dados para o servidor", este é o ponto
em que ele ira comecar o processo de envio de dados do BMX055 para o firebase. Quando quiser que os dados parem pode desligar o modulo ou enviar o sms "A parar de enviar os dados", se mais tarde quiser 
retomar é so enviar "enviar https", se desligar o modulo, tem de repetir todos os passos deste ponto 7.
10. O LED1_PIN liga ao concluir o setup; O LED2_PIN liga ao concluir a ligação https; O LED3_PIN liga e desliga caso esteja a enviar ou a não enviar dados, respetivamente.


## Como CARREGAR certificado para https no LARA R211

1.ir a certificado.h , e no "static const char* root_ca = …", colocar o certificado PEM, cuidado a colocar, não é so copiar e colar, coloque o certificado da mesma forma que um de exemplo já la esta.
2.ir a certificado.cpp, e na função void mandar_certificado_texto(), ira alterar o nome da variável de cima(caso tenha alterado o roor_ca para outro nome) e alterar o nome do certificado com que este vai ficar no LARA 
3. ir ao LARA_R211_Servidor_BMX055.ino, e descomentar a linha//mandar_certificado_texto();, (esta nas ultimas linhas do void setup). 
4.Carregar o código no microcontrolador, no fim do setup todos os certificador irão ser enumerados, pode confirmar se foi vem sucedido

## Como ELIMINAR certificado para https no LARA R211

1.ir ao LARA_R211_Servidor_BMX055.ino, e descomentar a linha //remover_certificado();, (esta nas ultimas linhas do void setup). 
2. Carregar o código no microcontrolador, ele ira enumerar todos os certificados, e pede pelo Serial monitor o nome do certificado a retirar, devera copiar o certificado a remover, colar no Serial monitor e dar enter.
3. para remover outro devera repetir o passo 2.

## Primeira vez a ligar o RTC / Querer configurar as horas 
Caso se pretenda sincronizar a hora com a do computador, deve-se remover
a pilha do modulo RTC e aguardar cerca de 10 minutos, para que este perca a hora previamente
armazenada. Ao correr o código de inicialização apos esse período, o RTC ira assumir a hora atual
do computador.




