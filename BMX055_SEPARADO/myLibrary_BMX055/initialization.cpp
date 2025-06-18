#include "../includes_myLibrary_BMX055.h"

#include <Wire.h>

void initBMX055_ACC(int address, uint8_t range, uint8_t filter_bandwidth) {
  

  Wire.beginTransmission(address);
  Wire.write(0x0F);  // Configuração do range
  Wire.write(range);
  Wire.endTransmission();

Wire.beginTransmission(address);
  Wire.write(0x13); 
  if(filter_bandwidth==0){
    Wire.write(0x01); // mostra dados nao filtrados
    Wire.endTransmission();
  }

  if(filter_bandwidth!=0){
  Wire.write(0x00); // mostra os dados filtrados
  Wire.endTransmission();

  Wire.beginTransmission(address);
  Wire.write(0x10);  // Taxa de amostragem
  Wire.write(filter_bandwidth);
  Wire.endTransmission();
  }
}

void initBMX055_GYR(int address,uint8_t range, uint8_t filter_bandwidth ) {
 
  // Configuração do giroscópio
  Wire.beginTransmission(address);
  Wire.write(0x0F);
  Wire.write(range);  // Sensibilidade de 500°/s por exemplo
  Wire.endTransmission();


Wire.beginTransmission(address);
Wire.write(0x10);  // Configuração do range
Wire.write(filter_bandwidth);
Wire.endTransmission();

}


void initBMX055_MAG(int address, uint8_t rate ) {

  Wire.beginTransmission(address);
    Wire.write(0x4B); // Register de controle de potência
    Wire.write(0x83); // Soft Reset do sensor (sem isto nao funciona)
    Wire.endTransmission();
    delay(100);
  
  Wire.beginTransmission(address);
    Wire.write(0x4D); // Register de controle de potência
    Wire.write(0b01111111); // overflow int en =1, para saber quando o sensor esta saturado
    Wire.endTransmission();
    delay(100);
  
    uint8_t modo_normal = 0b00000000; //decidi colocar assim para ser mais facil alterar entre modos caso pretenda
    // Configuração do magnetômetro
    Wire.beginTransmission(address);
    Wire.write(0x4C);
    Wire.write((modo_normal & ~0x38) | ((rate << 3) & 0x38));  // Ativar modo normal, e ainda altera os 
                                                              //os bits 3,4,5 conforme o rate escolhido
    Wire.endTransmission();
  delay(200);
    // Configuração do magnetômetro
    Wire.beginTransmission(address);
    Wire.write(0x4E);
    Wire.write(0b10000111);  // Ativar O DATA READY PIN EN
    Wire.endTransmission();
    delay(200);
  
  }