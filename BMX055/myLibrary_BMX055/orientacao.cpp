#include "../includes_myLibrary_BMX055.h"
#include <Wire.h>


#define BMX055_ACCEL_ADDR 0x19  // Endereço I2C do acelerômetro
#define ORIENT_REG 0x0C         // Registrador que contém os orient bits

 

void set_orientacion_mode_ACC(uint8_t ORIENT_MODE) {

  Wire.beginTransmission(BMX055_ACCEL_ADDR);
  Wire.write(0x2C);  // Configuração do range
  Wire.write(ORIENT_MODE);  //modo simetrico
  Wire.endTransmission();
  delay(100);

  Wire.beginTransmission(BMX055_ACCEL_ADDR);
  Wire.write(0x16);  
  Wire.write(0b11000000);  // ativar o modo de orientacao e o modo flat
  Wire.endTransmission();
  delay(100);


}

uint8_t readOrientBits_ACC() {
  delay(50);
  Wire.beginTransmission(BMX055_ACCEL_ADDR);
  Wire.write(0x0C);
  Wire.endTransmission();
  Wire.requestFrom(BMX055_ACCEL_ADDR, 1);  // Lê 1 byte do registrador
              // pegar os bits 6, 5 e 4
  return ((Wire.read() & 0b01110000) >> 4); //(diz o z, e a orientacao do plano xy, nao diz se esta flat ou nao)

}

uint8_t readFlat_ACC() { //deteta se esta flat
delay(50);
 Wire.beginTransmission(BMX055_ACCEL_ADDR);
  Wire.write(0x0C);
  Wire.endTransmission();
  Wire.requestFrom(BMX055_ACCEL_ADDR, 1);  // Lê 1 byte do registrador
              // pegar os bits 7
  return ((Wire.read() & 0b10000000) >> 7); //so quero o bit mais significatico
}

String interpretOrientation_ACC(uint8_t OrientBits, uint8_t Flat) {

  if (Flat == 0x01) {
    switch (OrientBits) {
      case 0b000: return "z>0 e face para cima";
      case 0b001: return "z>0 e face para cima";
      case 0b010: return "z>0 e face para cima";
      case 0b011: return "z>0 e face para cima";

      case 0b100: return "z<0 e face para baixo";
      case 0b101: return "z<0 e face para baixo";
      case 0b110: return "z<0 e face para baixo";
      case 0b111: return "z<0 e face para baixo";
      default: return "Desconhecido";
    }
  }
  if (Flat == 0x00) {
    switch (OrientBits) {
      case 0b000: return "z>0 e vertical para cima";
      case 0b001: return "z>0 e vertical para baixo";
      case 0b010: return "z>0 e horizontal para esquerda";
      case 0b011: return "z>0 e horizontal para direita";

      case 0b100: return "z<0 e vertical para baixo";
      case 0b101: return "z<0 e vertical para cima";
      case 0b110: return "z<0 e horizontal para direita";
      case 0b111: return "z<0 e horizontal para esquerda";
      default: return "Desconhecido";
    }
  }
}