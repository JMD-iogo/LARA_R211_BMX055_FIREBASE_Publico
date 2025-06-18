#include "../includes_myLibrary_BMX055.h"

#include <Wire.h>
void readBMX055_ACC(int address, uint8_t range, float &ax, float &ay, float &az, float &temperature) {

  Wire.beginTransmission(address);
  Wire.write(0x02);
  Wire.endTransmission();
  Wire.requestFrom(address, 6);

  //no arduino nano isto funcionava bem, mas ao passar para esp32 nao funcionou bem, o sinal do numero desaparecia
  //lia valores de 0 a 4096 , mas devia ser de -2048 a 2048
  //int16_t raw_ax = (Wire.read() | (Wire.read() << 8)) >> 4;
  //int16_t raw_ay = (Wire.read() | (Wire.read() << 8)) >> 4;
  //int16_t raw_az = (Wire.read() | (Wire.read() << 8)) >> 4;

  uint16_t raw;
raw = Wire.read() | (Wire.read() << 8);
int16_t raw_ax = ((int16_t)raw) >> 4;

raw = Wire.read() | (Wire.read() << 8);
int16_t raw_ay = ((int16_t)raw) >> 4;

raw = Wire.read() | (Wire.read() << 8);
int16_t raw_az = ((int16_t)raw) >> 4;


  float resolucao;
  switch (range) {
    case 0x03:
      resolucao = 0.00098;
      break;  // ±2g
    case 0x05:
      resolucao = 0.00195;
      break;  // ±4g
    case 0x08:
      resolucao = 0.00391;
      break;  // ±8g
    case 0x0C:
      resolucao = 0.00781;
      break;  // ±16g
    default:
      resolucao = 0;

  }

  ax = raw_ax * resolucao;
  ay = raw_ay * resolucao;
  az = raw_az * resolucao;

  
  Wire.beginTransmission(address);  // Inicia a comunicação com o sensor
  Wire.write(0x08);  // Envia o comando para ler o registrador de temperatura (0x08)
  Wire.endTransmission();
  
  Wire.requestFrom(address, 1);  // Solicita 1 byte de dados do sensor
    int8_t raw_temp = Wire.read();  // Lê o valor e já considera o sinal
    temperature = raw_temp * 0.5 + 23.0;  // Conversão para Celsius com offset
}

//pag99
// Definição das escalas do giroscopio
void readBMX055_GYR(int address,uint8_t range, float &gx, float &gy, float &gz) {
  Wire.beginTransmission(address);
  Wire.write(0x02);
  Wire.endTransmission();
  Wire.requestFrom(address, 6);

 //int16_t raw_gx = Wire.read() | (Wire.read() << 8);
  //int16_t raw_gy = Wire.read() | (Wire.read() << 8);
 //int16_t raw_gz = Wire.read() | (Wire.read() << 8);

 uint16_t raw;
raw = Wire.read() | (Wire.read() << 8);
int16_t raw_gx = (int16_t)raw;

raw = Wire.read() | (Wire.read() << 8);
int16_t raw_gy = (int16_t)raw;

raw = Wire.read() | (Wire.read() << 8);
int16_t raw_gz = (int16_t)raw;


 float resolucao;
switch (range) {
  case 0x0:
    resolucao = 0.061;
    break;  // 2000graus por s
  case 0x1:
    resolucao = 0.0305;
    break;  // 1000graus por s
  case 0x2:
    resolucao = 0.0153;
    break;  // 500graus por s
  case 0x3:
    resolucao = 0.0076;
    break;  // 250graus por s
    case 0x4:
    resolucao = 0.0038;
    break;  // 125graus por s
  default:
    resolucao = 1;
    }
 gx = raw_gx * resolucao;
gy = raw_gy * resolucao;
gz = raw_gz * resolucao;


}



int overflow_MAG(int address) {
  Wire.beginTransmission(address);
  Wire.write(0x4A);  // Registrador de status de overflow
  Wire.endTransmission();
  Wire.requestFrom(address, 1);

  uint8_t overflowStatus = Wire.read();
  delay(200);
  if (overflowStatus & 0b01000000) {
    delay(100);
    return 0;
  }

  delay(100);
  return 1;
}

int readBMX055_MAG(int address, float &B, float &mx, float &my, float &mz, float &rhall) {
  //le o data_ready_status, qunado ele é um o sensor escreveu os valores novos

  Wire.beginTransmission(address);
  Wire.write(0x48);  // Registrador de status
  Wire.endTransmission();
  Wire.requestFrom(address, 1);

  uint8_t status = Wire.read();
  if (!(status & 0b00000001)) {
    Serial.println("Sensor não está pronto para leitura!");
    return 0;
  }

  if (overflow_MAG(address)) {


    Wire.beginTransmission(address);
    Wire.write(0x42);
    Wire.endTransmission();
    Wire.requestFrom(address, 8);

    //int16_t raw_mx = (Wire.read() | (Wire.read() << 8)) >> 3;
    //int16_t raw_my = (Wire.read() | (Wire.read() << 8)) >> 3;
    //int16_t raw_mz = (Wire.read() | (Wire.read() << 8)) >> 1;
    //int16_t raw_rhall = (Wire.read() | (Wire.read() << 8)) >> 2;


    uint16_t raw;

raw = Wire.read() | (Wire.read() << 8);
int16_t raw_mx = ((int16_t)raw) >> 3;

raw = Wire.read() | (Wire.read() << 8);
int16_t raw_my = ((int16_t)raw) >> 3;

raw = Wire.read() | (Wire.read() << 8);
int16_t raw_mz = ((int16_t)raw) >> 1;

raw = Wire.read() | (Wire.read() << 8);
int16_t raw_rhall = ((int16_t)raw) >> 2;  // RHALL pode ser unsigned dependendo do uso


    float resolucao = 0.0625;  //a definir  //16LSB/microT ?? pag123
    mx = raw_mx * resolucao;
    my = raw_my * resolucao;
    mz = raw_mz * resolucao;
    rhall = raw_rhall * resolucao;  //valor da resistencia da placa do sensor hall
    B = sqrt(pow(mx, 2) + pow(my, 2) + pow(mz, 2));

    //este sensor compensa as variacoes de temperatura
    return 1;
  } else {
    Serial.println("overflow ");
    return 0;
  }
}



