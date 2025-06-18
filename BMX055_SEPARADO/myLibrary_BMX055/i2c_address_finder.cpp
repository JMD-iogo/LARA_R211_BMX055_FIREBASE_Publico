//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//Autor : Diogo Ferreia
//serve para comparar o id dos registos (este é fixo)dependendo do endereco i2c, pois dependendo das ligacoes das portas, por exemplo o endereco i2c do acelerometro pode ser o 0x18 ou 0x19, como nao encontrei as portas que era suposto ligar,
//na pagina 145 do: "https://pt.mouser.com/datasheet/2/783/BST_BMX055_DS000-1509552.pdf", este ficheiro.h , para o acelerometro compara as duas possibilidades e ve na posicao 0x00 se o valor é b'1111'1010 == 0xFA
//estes valores na posicao 0x00 do acelerometro é o id do registo: b'1111'1010 == 0xFA  na pagina 50 do documento
//estes valores na posicao 0x00 do giroscopio é o id do registo: b'0000'1111 == 0x0F  na pagina 93 do documento
//estes valores na posicao 0x4B do magnetometro se é possivel, esta é a posicao do powercontrol, se este tiver = 0, nao conseguimos aceder ao id do registo, mas este so pode ter aqueles 2 valores  na pagina 130 ou 134 do documento
//funcao retorna o endereço i2c certo para os diferentes registos
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


#include "../includes_myLibrary_BMX055.h"
#include <Wire.h>

// Declaração das funções---------------------------------------------
int findCorrectI2CAddress_ACC();  //retorna o endereço i2c certo
int findCorrectI2CAddress_GYR();  //retorna o endereço i2c certo
int findCorrectI2CAddress_MAG();  //retorna o endereço i2c certo

// Definições dos endereços possiveis registros---------------------------------------------
#define ADDR_1_ACC 0x18            // Primeiro endereço a testar para o acelerometro
#define ADDR_2_ACC 0x19            // Segundo endereço a testar para o acelerometro
#define REGISTER_ACC 0x00          // Registro onde vamos ler o valor esperado para o acelerometro
#define EXPECTED_CHIP_ID_ACC 0xFA  //valor esperado dentro endereco do resgisto do Register_acc

#define ADDR_1_GYR 0x68            // Primeiro endereço a testar para o giroscopio
#define ADDR_2_GYR 0x69            // Segundo endereço a testar para o giroscopio
#define REGISTER_GYR 0x00          // Registro onde vamos ler o valor esperado para o giroscopio
#define EXPECTED_CHIP_ID_GYR 0x0F  //valor esperado dentro endereco do resgisto do Register_gyr

#define ADDR_1_MAG 0x10            // Primeiro endereço a testar para o giroscopio
#define ADDR_2_MAG 0x11            // Segundo endereço a testar para o giroscopio
#define ADDR_3_MAG 0x12            // Terceiro endereço a testar para o giroscopio
#define ADDR_4_MAG 0x13            // Quarto endereço a testar para o giroscopio
#define REGISTER_MAG 0x40          // Registro onde vamos ler o valor esperado para o giroscopio
#define EXPECTED_CHIP_ID_MAG 0x32  //valor esperado dentro endereco do resgisto do Register_mag


//funcoes----------------------------------------------------------------------------------------
int findCorrectI2CAddress_ACC() {  //retorna o endereço i2c certo
  
  uint8_t addresses_ACC[] = { ADDR_1_ACC, ADDR_2_ACC };
  for (uint8_t i = 0; i < 2; i++) {
    uint8_t addr = addresses_ACC[i];
    Wire.beginTransmission(addr);
    Wire.write(REGISTER_ACC);
    if (Wire.endTransmission() == 0) {     // Se o dispositivo responde
      Wire.requestFrom(addr, (uint8_t)1);  // Pedir 1 byte do registro 0x00
      if (Wire.available()) {
        uint8_t value = Wire.read();
        if (value == EXPECTED_CHIP_ID_ACC) {
          Serial.print("Endereço correto encontrado ACC: 0x");
    Serial.println(addr, HEX);
    delay(4000);
          return addr;  // Retorna o endereço correto
        }
      }
    }
  }
  Serial.print("Erro ao encontrar o endereco i2c, sera usado o valor pre-definido 0x19");
  delay(4000);
  return ADDR_2_ACC;  // Retorna 0x19 se nenhum endereço retornou o valor esperado
}

int findCorrectI2CAddress_GYR() {  //retorna o endereço i2c certo
  uint8_t addresses_GYR[] = { ADDR_1_GYR, ADDR_2_GYR };
  for (uint8_t i = 0; i < 2; i++) {
    uint8_t addr = addresses_GYR[i];
    Wire.beginTransmission(addr);
    Wire.write(REGISTER_GYR);
    if (Wire.endTransmission() == 0) {     // Se o dispositivo responde
      Wire.requestFrom(addr, (uint8_t)1);  // Pedir 1 byte do registro 0x00
      if (Wire.available()) {
      
        uint8_t value = Wire.read();
        if (value == EXPECTED_CHIP_ID_GYR) {
          Serial.print("Endereço correto encontrado GYR: 0x");
          Serial.println(addr, HEX);
          delay(4000);
          return addr;  // Retorna o endereço correto
        }
      }
    }
  }
  Serial.print("Erro ao encontrar o endereco i2c, sera usado o valor pre-definido 0x69");
    delay(4000);
  return ADDR_2_GYR;  // Retorna 0x69 se nenhum endereço retornou o valor esperado
}

int findCorrectI2CAddress_MAG() {  //retorna o endereço i2c certo
  uint8_t addresses_MAG[] = { ADDR_1_MAG, ADDR_2_MAG, ADDR_3_MAG, ADDR_4_MAG };
  for (uint8_t i = 0; i < 4; i++) {
    uint8_t addr = addresses_MAG[i];
    Wire.beginTransmission(addr);
    Wire.write(REGISTER_MAG);
    if (Wire.endTransmission() == 0) {     // Se o dispositivo responde
      Wire.requestFrom(addr, (uint8_t)1);  // Pedir 1 byte do registro 0x00
      if (false) {
        Serial.print("Endereço correto encontrado MAG: 0x");
    Serial.println(addr, HEX);
    delay(4000);
        return addr;  // Retorna o endereço correto
      }
    }
  }
  Serial.println("Erro ao encontrar o endereco i2c, sera usado o valor pre-definido 0x13");
    delay(4000);
  return ADDR_4_MAG;  // Retorna 0x13 se nenhum endereço retornou o valor esperado
}