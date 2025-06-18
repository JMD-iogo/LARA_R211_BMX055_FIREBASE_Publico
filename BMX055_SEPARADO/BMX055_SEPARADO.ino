//guardar dados nos ficheiros
//analisar a correlacao no matlab, desvio padrao, media
//comparar os 3 eixos, as escalas, ver resolucao, testar correlacao entre acelerometro e magnetometro
//testar a diferentes temperaturas

//comparar a minha biblioteca com a minha que fiz, dizer que escrevi a minha
//testar para os diferentes ganhos(2,4,8,16), e para x,y,z, testando diferentes orientacoes

//media sem filtro, ver a media e desvio padrao  com ele parado, ver para diferentes temperaturas
//, ruido, correlacao entre sinais (x,y,z)

//enviar uma mensagem com o nivel de sinal, rssi, tentar receber uma mensagem (para ser bidirecional)
//depois enviar os dados por email
//depois enviar dados para o servidor da google
//depois fazer algo para poder ver isso no computador

//explorei funcionalidades que queria menos o No-motion, da pagina 45

#include <Wire.h>
#include "includes_myLibrary_BMX055.h"
#include "config.h"
#include "print_guarda.h"

//a definir e ler com atencao ------------------------------------------------------------------------------
//o sensor sera inicializado com as diferentes opcoes do acelerometro :
//(ver quais valores as variaveis podem assumir no config.h):
uint8_t filter_bandwidth_ACC = NO_FILTER;
uint8_t range_aceleracao_ACC = RANGE_ACC_2G;  // Escolha da escala
uint8_t modo_orientacao_ACC = ORIENT_MODE_SYMMETRICAL_ACC;

//o sensor sera inicializado com as diferentes opcoes do giroscopio :
//(ver quais valores as variaveis podem assumir no config.h):
uint8_t filter_bandwidth_GYR = NO_FILTER_GYR;
uint8_t range_GYR = RANGE_GYR_500;

//o sensor sera inicializado com as diferentes opcoes do magnetometro :
uint8_t rate_MAG = RATE_MAG_10Hz;

String qual_registo = "PRINT_ACC";  //escrever ACC,MAG ou GYR para adquirir os dados  de um dos sensores corretamente pelo python, TODOS para adquirir todos
                                //escrever PRINT_ACC,PRINT_MAG ou PRINT_GYR para visualizar somente um dos sensores no serial
                               //escrever qualquer coisa que nao os de cima para so dar print de todos ao mesmo tempo

//-------------------------------------------------------------------------------------------------------

int sdaPin = 21;  // Pino SDA no ESP32
int sclPin = 22;  // Pino SCL no ESP32
int BMX055_GYRO_ADDR;  // Endereço do giroscópio;
int BMX055_ACC_ADDR;   // Endereço do acelerometro
int BMX055_MAG_ADDR;   //  Endereço do magnetômetro
void setup() {

  Serial.begin(115200);

    // Inicializando a comunicação I2C com pinos específicos
  Wire.begin(sdaPin, sclPin);

  Serial.println("Ver endereços I2C dos sensores...");
  for (byte addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0) {
      Serial.print("Dispositivo encontrado no endereço 0x");
      Serial.println(addr, HEX);
    }
  }

Serial.println("Procurar endereços automaticamente..");
  //encontra sozinho o endereco i2c certo para o acelerometro
  BMX055_ACC_ADDR = findCorrectI2CAddress_ACC();
  //BMX055_ACC_ADDR = 0x19;
  //encontra sozinho o endereco i2c certo para o giroscopio
  BMX055_GYRO_ADDR = findCorrectI2CAddress_GYR();
 // BMX055_GYRO_ADDR = 0x69;
  //encontra sozinho o endereco i2c certo para o magnetometro
  BMX055_MAG_ADDR = findCorrectI2CAddress_MAG();
 // BMX055_MAG_ADDR = 0x13;
  //--------fazer self test ao acelerometor, podemos comentar se nao quisermos;
  //SelfTest_ACC(BMX055_ACC_ADDR); //nota ao fazer o selftest temos fazer NO_FILTER ou colocar FILTER_BANDWIDTH_1000Hz (para evitar uma falha de input do utilizador a funcao fara reset antes do test, e no final(por recomendacao no datashet))
  //-------- caso contrario o update time é muito grande e ele nao consegue fazer o selftet
  //--------- ele tem de estar antes do init, pois no final ele faz um soft reset

  //fazer self test ao giroscopio, podemos comentar se nao quisermos;
  //SelfTest_GYR(BMX055_GYRO_ADDR);  //devera ficar a cima da inicializacao, pois ele faz um reset

//nao foi criado self teste para o magnetometro, mas daria


  // Inicializa o sensor acelerometro com os parâmetros definidos
  initBMX055_ACC(BMX055_ACC_ADDR, range_aceleracao_ACC, filter_bandwidth_ACC);
  set_orientacion_mode_ACC(modo_orientacao_ACC);

  // Inicializa o sensor gioscopio com os parâmetros definidos
  initBMX055_GYR(BMX055_GYRO_ADDR, range_GYR, filter_bandwidth_GYR);

  // Inicializa o sensor gioscopio com os parâmetros definidos
  initBMX055_MAG(BMX055_MAG_ADDR, rate_MAG);
}


void loop() {
  //variaveis para o acelerometro
  float ax, ay, az, temperature;

  //variaveis para o giroscopio
  float gx, gy, gz;

  //variaveis para o magnetometor
  float B, mx, my, mz, rhall;

//ESTOU A DAR UM PRINT DE TEST NESTA FUNCAO
  //le e altera as variaveis do acelerometro contando com o range escolhido, pois diferentes ranges tem diferentes resoluçoes
  readBMX055_ACC(BMX055_ACC_ADDR, range_aceleracao_ACC, ax, ay, az, temperature);

  //le e altera as variaveis do giroscopio contando com o range escolhido, pois diferentes ranges tem diferentes resoluçoes
  readBMX055_GYR(BMX055_GYRO_ADDR, range_GYR, gx, gy, gz);

  //bits que dizem se o sensor acelerometro esta na horizontal ou vertical
  uint8_t orient_bits_ACC = readOrientBits_ACC();
  //bits que dizem se o sensor acelerometro esta plano (virado para cima ou baixo)
  uint8_t flat_bits_ACC = readFlat_ACC();
  //serve para interpretar os bits do acelerometro a cima, retornando uma string a dizer como ele esta
  String orientation = interpretOrientation_ACC(orient_bits_ACC, flat_bits_ACC);


  // Ler magnetômetro, os outros tb dao para fazer assim, mas por escolha pessoal nao o fiz
  if (readBMX055_MAG(BMX055_MAG_ADDR, B, mx, my, mz, rhall)) {


    float azimuth = atan2(my, mx) * 180 / PI;  // Convertendo para graus
    if (azimuth < 0) azimuth += 360;           // Ajuste para garantir que o valor esteja entre 0 e 360°

    // Calcular a inclinação (ângulo vertical)
    float inclination = atan2(mz, sqrt(mx * mx + my * my)) * 180 / PI;  // Convertendo para graus
//ira dar print na forma agradavel de ler no serial, ou se é na forma para o python guardar, vindo na forma "var",var,"var1",var1,"var2",var2
   //caso seja guarda("MAG") ele servira para guardar os dados do magnetometro, tudo o resto é para ver no serial todos ao mesmo tempo
    serial_print_save_MAG(guardar(qual_registo), B, mx, my, mz, rhall, azimuth, inclination);
  }



//ira dar print na forma agradavel de ler no serial, ou se é na forma para o python guardar, vindo na forma "var",var,"var1",var1,"var2",var2
   //caso seja guarda("ACC") ele servira para guardar os dados do acelerometro, tudo o resto é para ver no serial todos ao mesmo tempo
  serial_print_save_ACC(guardar(qual_registo), ax, ay, az, temperature, orientation);
  //ira dar print na forma agradavel de ler no serial, ou se é na forma para o python guardar, vindo na forma "var",var,"var1",var1,"var2",var2
   //caso seja guarda("GYR") ele servira para guardar os dados do giroscopio, tudo o resto é para ver no serial todos ao mesmo tempo
  serial_print_save_GYR(guardar(qual_registo), gx, gy, gz);

  delay(500);
}

