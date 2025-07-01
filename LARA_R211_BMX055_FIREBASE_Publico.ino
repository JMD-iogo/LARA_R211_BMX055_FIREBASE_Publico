//este codigo adquite dados atravez de funcoes criadas pelo autor, para o BMX055, envia estes dados
//para um firebase que pode nao estar disponivel no futuro. Ele obtem as usas de um RTC usando "#include <RTClib.h>"

//resumo do codigo:
//set up do lara r211
//set up do BMX055
//numero recebe um sms de set up concluido
//mandar sms a dizer "ligar https", recebe a confirmacao de que concluiu a ligacao
//mandar sms a dizer "enviar https", recebe a confirmacao de que iniciou o envio
//neste momento ele ira estar a mandar dados atuais do BMX055 de 40 em 40 segundos de forma indefinida
//para parar de enviar, mandar sms a dizer "nao enviar https", ele neste momento ira para de enviar, iremos receber um sms a confirmar que parou
//O LED1_PIN liga ao concluir o setup; O LED2_PIN liga ao concluir a ligação https; O LED3_PIN liga e desliga caso esteja a enviar ou a não enviar dados, respetivamente.


//cuidados: o envio e recebimento de sms nao é 100% fiavel; para mandar dados https para outro servidor
//é preciso tanto trocar o link a enviar, como o caminho e ainda alterar o certificado,
//neste momento esta para o firebase e o certificado Google Trust Services Root 2
#include "definicoes.h"

#include <Wire.h>
#include <RTClib.h>

#include "LARA_R211/includes_codigo_cpp_r211.h"
#include "LARA_R211/includes_codigo_h_r211.h"

#include "RTC/horas_rtc.h"    // Inclui o cabeçalho da classe HorasRTC
#include "RTC/horas_rtc.cpp"  // Inclui o cabeçalho da classe HorasRTC

#include "BMX055/includes_myLibrary_BMX055.h"
#include "BMX055/config.h"
#include "BMX055/print_guarda.h"

//a definir e ler com atencao ------------------------------------------------------------------------------

//definir numero de telefone no definicoes.h
//definir link do firebase no definicoes.h
//definir pinos no definicoes.h
//definir apn no ficheiro  ligar_4g_3g_2g_automatico.cpp
//o sensor sera inicializado com as diferentes opcoes do acelerometro :
//(ver quais valores as variaveis podem assumir no config.h):
uint8_t filter_bandwidth_ACC = FILTER_BANDWIDTH_ACC_7Hz;
uint8_t range_aceleracao_ACC = RANGE_ACC_2G;  // Escolha da escala
uint8_t modo_orientacao_ACC = ORIENT_MODE_SYMMETRICAL_ACC;

//o sensor sera inicializado com as diferentes opcoes do giroscopio :
//(ver quais valores as variaveis podem assumir no config.h):
uint8_t filter_bandwidth_GYR = FILTER_BANDWIDTH_GYR_23Hz;
uint8_t range_GYR = RANGE_GYR_500;

//o sensor sera inicializado com as diferentes opcoes do magnetometro :
uint8_t rate_MAG = RATE_MAG_10Hz;

String qual_registo = "TUDO";  //escrever ACC,MAG ou GYR para adquirir os dados  de um dos sensores corretamente pelo python
                              //escrever PRINT_ACC,PRINT_MAG ou PRINT_GYR para visualizar somente um dos sensores no serial
                              //escrever qualquer coisa que nao os de cima para so dar print de todos ao mesmo tempo
//tem de ter tudo para evviar os dados para a base de dados de forma certa, se tiver  acc vai causar erros
//-------------------------------------------------------------------------------------------------------


int BMX055_GYRO_ADDR = 0x69;  // Endereço do giroscópio;
int BMX055_ACC_ADDR = 0x19;   // Endereço do acelerometro
int BMX055_MAG_ADDR = 0x13;   //  Endereço do magnetômetro

String messageBody = "";            //variavel que tem somente a mensagem recebida pelo modulo
String messageNumber = "";          //variavel que tem o numero da mensagem recebida pelo modulo
bool a_enviar_dados_https = false;  //ao mandar sms para enviar os dados ele entra num loop, so para ao se mandar
                                    //um sms, esta variavel é falsa quando nao esta a enviar, e verdadeira ao enviar


//variaveis para o acelerometro
float ax, ay, az, temperature;
String orientation;

//variaveis para o giroscopio
float gx, gy, gz;

//variaveis para o magnetometor
float B, mx, my, mz, rhall;
String data_a_enviar;

HorasRTC rtc;                      // Cria uma instância do objeto HorasRTC do modulo rtc (é da biblioteca)
volatile bool sms_chegou = false;  //o modulo lara r211 tem um pino que esta sempre HIGH, mas quando recebe um
                                   //sms ele fica LOW durante um tempo, mas devido ao tempo que o codigo demora
                                   //a enviar os dados o mais provavel era o codigo nao apanhar o sinal low
                                   //foi criada a funcao a baixo handleRI(), que coloca a variavel sms_chegou true quando o sms chega
                                   //mesmo quando o codigo nao esta a correr naquele sitio

//ira guardar o sinal no ri_pin mesmo que nao esteja no momento de ler no codigo
//ao receber um sms o pino RI fica LOW, mas é so momentaneamente
void IRAM_ATTR handleRI() {
  sms_chegou = true;
}

void setup() {

  Serial.begin(115200);                               //inicia a comunicacao serial, para debug e visualizar no Serial Monitor do PC
  Serial2.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN);  // baud rate, data format, RX, TX ; é o que vamos usar para mandar comandos para o lara, isto é um serial fisico, e nao se software (o esp32 da nos essa opcao)
  pinMode(RTS_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(RI_PIN), handleRI, FALLING);  //ira guardar o sinal no ri_pin mesmo que nao esteja no momento de ler no codigo



  // Inicializando a comunicação I2C com pinos específicos, estes podem ser alterados no definicao_pinos.h
  int sdaPin = SDA_PIN;
  int sclPin = SCL_PIN;
  Wire.begin(sdaPin, sclPin);

  rtc.setupRTC();  // Configura o RTC, vai dar as horas das quais o compilador carregou


  // rtc.exibirHora();   //print da hora do modulo rtc
  pinMode(CTS_PIN, OUTPUT);  // CTS como saída, este vai dizer ao sensor que pode mandar dados ou nao (CTS_PIN, LOW -- pode enviar dados)
  pinMode(PWK_PIN, INPUT);   // pino PWK, serve para ligar o modulo (so um pulso nisto nao basta, é preciso mandar um comando para o acordar)
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  digitalWrite(CTS_PIN, LOW);   // Garante que o módulo pode enviar
  digitalWrite(LED1_PIN, LOW);  // Os leds comecam desligados
  digitalWrite(LED2_PIN, LOW);  // Os leds comecam desligados
  digitalWrite(LED3_PIN, LOW);  // Os leds comecam desligados
  pinMode(RI_PIN, INPUT);       // Ri como entrada, este é o pino que fica LOW ao receber um sms
  delay(200);

  //ira ligar o modulo
  powerOnModule();
  pinMode(PWK_PIN, INPUT);  //tem de ficar flutuante
  pinMode(RTS_PIN, INPUT);  //qunado o sensor quer enviar dados ele avisa por este este pino


  // Verifica SMS recebido
  sendATCommand("AT+CMGF=1");  // Configura para modo de texto
  delay(300);
  sendATCommand("AT+CNMI=1,1,0,0,0");  // Configura para receber novas mensagens imediatamente
  delay(300);
  sendATCommand("AT+CMGD=0,4");  // Apaga todas as mensagens lidas, nunca guardo sms, quando leio um apago logo
  delay(300);


  //--nao esta a retornar a data correta-----------------------------
  //funcao serviar para o modulo atualizar as horas de acordo com as horas da rede
  atualiza_horas();
  //funcao da as horas e a data do momento de acordo com a rede, vindo numa string tudo junto
  String dataHora = da_horas();
  //separa a string a cima e coloca-as num objeto Tempo e Data, dando para extrair cada informacao (exemploo a baixo)
  Tempo agora = extrairTempo(dataHora);
  Data hoje = extrairData(dataHora);

  //exemplo do objeto a cima
  Serial.println("=== Tempo ===");
  Serial.print("Hora: ");
  Serial.println(agora.hora);
  Serial.print("Minuto: ");
  Serial.println(agora.minuto);
  Serial.print("Segundo: ");
  Serial.println(agora.segundo);
  Serial.print("Fuso: ");
  Serial.println(agora.fuso);

  Serial.println("=== Data ===");
  Serial.print("Ano: ");
  Serial.println(hoje.ano);
  Serial.print("Mes: ");
  Serial.println(hoje.mes);
  Serial.print("Dia: ");
  Serial.println(hoje.dia);
  //----------------------------------------------------------------

  initGPRS();  // Ativa dados móveis com APN da WOO
  delay(1000);
  testPING();  //testa o ping da rede
  delay(1000);
  Serial.println("Setup concluído!");  //do lara r211, ja estara pronto para receber comandos at, sms e ja estara ligado á melhor internet disponivel (4g lte, 3g ou 2g)

  // Inicializa o sensor BMX----------------------------------------------------------------
  // Inicializa o sensor acelerometro com os parâmetros definidos
  initBMX055_ACC(BMX055_ACC_ADDR, range_aceleracao_ACC, filter_bandwidth_ACC);
  set_orientacion_mode_ACC(modo_orientacao_ACC);

  // Inicializa o sensor gioscopio com os parâmetros definidos
  initBMX055_GYR(BMX055_GYRO_ADDR, range_GYR, filter_bandwidth_GYR);

  // Inicializa o sensor gioscopio com os parâmetros definidos
  initBMX055_MAG(BMX055_MAG_ADDR, rate_MAG);
  //----------------------------------------------------------------



  //manda um sms para o numero definido (o objetivo do codigo seria usar-lo á distancia)
  sendSMS(prefixo_numero + numero, "Setup concluído!");
  delay(1000);
  digitalWrite(LED1_PIN, HIGH);  // Garante que o pino 11 inicia desligado

  //mandar_certificado(); //Mandar o certificado, descomentar caso o certificado desapareça
  //remover_certificado(); //ira enumerar todos os certificados, e pede pelo Serial monitor o nome do certificado a retirar
  //mandar_certificado_texto(); //carrega um certificado PEM, no certificado.h //static const char* root_ca = ..., colocar nesta variavel e alterar o nome
  //no certificado.cpp tem de se alterar o nome da variavel caso tena se alterado, e alterar o nome que se quer que o certificado fique dentro do LARA
   //ver_certificados(); util para ver quais certificados estao no lara
}

// ajustar manualmente a hora do modulo rtc, nao é nada pratico, mas existe essa opcao
// rtc.ajustarHora(2025, 4, 25, 15, 30, 0);  // Ajuste manual para 25 de abril de 2025, 15:30:00

void loop() {
  if (qual_registo == "ACC" || qual_registo == "MAG" || qual_registo == "GYR") {  //DADOS BMX055
  dados_bmx();
  } else {
    static bool waitingRTS = false;
    //modulo lara r211 esta pronto para enviar dados
    if (digitalRead(RTS_PIN) == LOW) {
      //serve para so dar a confirmacao uma vez, caso contrario cada vez que fosse enviar dados ele diza que o rts esta pronto
      //deta forma ele so diz "RTS diz que .." so se o pino RTS_PIN estivesse alguma vez HIGH (o que nao é normal num funcionamento normal do lara)
      if (waitingRTS) {
        Serial.println("RTS diz que esta pronto, a enviar comandos...");
        waitingRTS = false;
      }
      digitalWrite(CTS_PIN, LOW);  // Indica que o módulo pode enviar dados

      //se o sms_chegou o pino RI do lara ficou low, consequentemente o sms_chegou esta true
      if (sms_chegou) {
        sms_chegou = false;  // resetar flag
        delay(100);
        String sms_recebido = readSMS();  //retorna uma string com toda a mensagem do lara recebida
                                          //vai ser usado para so um certo numero poder iniciar, para a comunicacao https dependendo do sms que mande:
                                          // Extração do corpo da mensagem
        messageBody = extractMessageBody(sms_recebido);
        messageBody.toUpperCase();  //coloca tudo em letra maiuscula, para nao causar erros caso o utilizador envie so a primeira letra maiuscula por exemplo
        Serial.println("Corpo da mensagem:");
        Serial.println(messageBody);
        //Extracao do numero do sms
        messageNumber = extractMessageNumber(sms_recebido, prefixo_numero);
        Serial.println("Numero da mensagem:");
        Serial.println(messageNumber);
        sendATCommand("AT+CMGD=0,4");  // Apaga todas as mensagens lidas(so para ter a certeza)
        delay(500);
      }
      //se a mensagem recebida foi "LIGAR HTTPS" e o numero foi o definido ao inicio ele entra no if
      if (messageBody == "LIGAR HTTPS" && messageNumber == (prefixo_numero + numero)) {
        messageBody = "";  //para so entrar uma vez nos if
        //para termos uma representacao visual (os sms nao sao 100% fiaveis, as vezes nao chegam)



        //so para o utilisador saber o ponto da situacao
        //sendSMS(prefixo_numero + numero, "A iniciar https!");  //sinceramente é lento demais mas deixei para se saber quando enviar os sms
        //inicia o https com o firebase da google, se quiser mudar tem de se alterar o certificado a usar e o link https
        initHTTPS();
        sendSMS(prefixo_numero + numero, "https iniciado!");
        delay(500);
        digitalWrite(LED2_PIN, HIGH);
      }

      //se a mensagem recebida foi "NAO ENVIAR HTTPS" e o numero foi o definido ao inicio ele entra no if
      //ira colocar a variavel a_enviar_dados_https = false, para parar de enviar os dados
      //alem disso manda um sms de confirmacao e apaga as leds 1 e 2
      if (messageBody == "NAO ENVIAR HTTPS" && messageNumber == (prefixo_numero + numero)) {
        if (messageBody == "NAO ENVIAR HTTPS") {  //so ira mandar uma vez
          sendSMS(prefixo_numero + numero, "A parar de enviar os dados");
        }
        messageBody = "";  //para so entrar uma vez nos if


        a_enviar_dados_https = false;
        digitalWrite(LED3_PIN, LOW);
      }

      //se a mensagem recebida foi "ENVIAR HTTPS" e o numero foi o definido ao inicio ele entra no if
      //ira iniciar o envio de dados para a base de dados, ira entrar neste if ate enviar mos o sms "nao enviar https"
      //ao fazer mos isso a variavel a_enviar_dados_https fica false e ele deixa de entrar aqui
      if ((messageBody == "ENVIAR HTTPS" && messageNumber == (prefixo_numero + numero)) || a_enviar_dados_https) {
        if (messageBody == "ENVIAR HTTPS") {  //so ira mandar uma vez a dizer
          sendSMS(prefixo_numero + numero, "A enviar dados para o servidor");
        }

        messageBody = "";  //para so entrar uma vez nos if
        digitalWrite(LED3_PIN, HIGH);


        //DADOS BMX055
        //-----------------------------------------------------------------------------------
        //variaveis para o acelerometro

       dados_bmx();


        //garante que ele nao existe na memoria, caso contrario ele enche o buffer e depois da erro
        //o response nao vale a pena apagar pois a respostra sera reescrita, na criacao de dados json é que causa problemas nao se apagar
        verificarEApagarArquivos();  //apaga possiveis arquivos guardados no modulo que podem causar bugs
        // sendATCommand("AT+ULSTFILE=");  //debug para ver se realmente apagou tudo
        // delay(1000);

        //Acelerometro-------------------------------------------------------------------------------------------------------
        //funcao cria um json com os valores de input, depois guarda no Lara R211, para depois ser enviado por https
        CriaGuardaDadosJsonAcelerometro(temperature, ax, ay, az, orientation, data_a_enviar);
        //------------------------------------------------------------------------------------------------------------------
        //Giroscopio-------------------------------------------------------------------------------------------------------
        //funcao cria um json com os valores de input, depois guarda no Lara R211, para depois ser enviado por https
        CriaGuardaDadosJsonGiroscopio(gx, gy, gz, data_a_enviar);
        //------------------------------------------------------------------------------------------------------------------
        //Magnetometro-------------------------------------------------------------------------------------------------------
        //funcao cria um json com os valores de input, depois guarda no Lara R211, para depois ser enviado por https
        CriaGuardaDadosJsonMagnetometro(mx, my, mz, B, rhall, data_a_enviar);
        //------------------------------------------------------------------------------------------------------------------


        //manda os dados json que as funcoes a tras guardaram no lara r211
        mandarHTTPSFirebase();
        delay(500);                  
        a_enviar_dados_https = true;  // a primeira vez entra aqui devida a parte do if do sms, das proximas vezes entra neste if devido a esta variavel
      }



      //caso o lara nao esteja pronto para receber comandos entra aqui e coloca a variavel waitingRTS true, para so entrar aqui uma vez
      // so voltara a entrar caso ele entre uma vez no if correspondente a este else
    } else {
      if (!waitingRTS) {
        Serial.println("RTS está HIGH, aguardando...");
        waitingRTS = true;
      }
      digitalWrite(CTS_PIN, HIGH);  // Bloqueia envio até RTS ficar LOW
    }
  }
  delay(100);  // Aguarda antes de repetir o ciclo
}

void dados_bmx() {
  //-----------------------------------------------------------------------------------


  //le e altera as variaveis do giroscopio contando com o range escolhido, pois diferentes ranges tem diferentes resoluçoes
  readBMX055_GYR(BMX055_GYRO_ADDR, range_GYR, gx, gy, gz);
  //le e altera as variaveis do acelerometro contando com o range escolhido, pois diferentes ranges tem diferentes resoluçoes
  readBMX055_ACC(BMX055_ACC_ADDR, range_aceleracao_ACC, ax, ay, az, temperature);

  readBMX055_MAG(BMX055_MAG_ADDR, B, mx, my, mz, rhall);

   data_a_enviar = rtc.data_enviar();
  Serial.println("horas:" + data_a_enviar);


  //ira dar print na forma agradavel de ler no serial, ou se é na forma para o python guardar, vindo na forma "var",var,"var1",var1,"var2",var2
  //caso seja guarda("GYR") ele servira para guardar os dados do giroscopio, tudo o resto é para ver no serial todos ao mesmo tempo
  serial_print_save_GYR(guardar(qual_registo), gx, gy, gz);

  //bits que dizem se o sensor acelerometro esta na horizontal ou vertical
  uint8_t orient_bits_ACC = readOrientBits_ACC();
  //bits que dizem se o sensor acelerometro esta plano (virado para cima ou baixo)
  uint8_t flat_bits_ACC = readFlat_ACC();
  //serve para interpretar os bits do acelerometro a cima, retornando uma string a dizer como ele esta
  orientation = interpretOrientation_ACC(orient_bits_ACC, flat_bits_ACC);

  //ira dar print na forma agradavel de ler no serial, ou se é na forma para o python guardar, vindo na forma "var",var,"var1",var1,"var2",var2
  //caso seja guarda("ACC") ele servira para guardar os dados do acelerometro, tudo o resto é para ver no serial todos ao mesmo tempo
  serial_print_save_ACC(guardar(qual_registo), ax, ay, az, temperature, orientation);

  float azimuth = atan2(my, mx) * 180 / PI;  // Convertendo para graus
  if (azimuth < 0) azimuth += 360;           // Ajuste para garantir que o valor esteja entre 0 e 360°

  // Calcular a inclinação (ângulo vertical)
  float inclination = atan2(mz, sqrt(mx * mx + my * my)) * 180 / PI;  // Convertendo para graus
                                                                      //ira dar print na forma agradavel de ler no serial, ou se é na forma para o python guardar, vindo na forma "var",var,"var1",var1,"var2",var2
                                                                      //caso seja guarda("MAG") ele servira para guardar os dados do magnetometro, tudo o resto é para ver no serial todos ao mesmo tempo
  serial_print_save_MAG(guardar(qual_registo), B, mx, my, mz, rhall, azimuth, inclination);
  //-----------------------------------------------------------------------------------
}
