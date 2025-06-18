#ifndef DEFINICAO_PINOS
#define DEFINICAO_PINOS

#include <Arduino.h>
#include <HardwareSerial.h>

//este sera o numero para o qual o modulo mandara mensagens de confirmacao
//este sera o numero para o qual o modulo aceita os comandos mandados por sms
String prefixo_numero = "+351";
String numero = "numero_telemovel";

//definicao do link do firebase
//so o que vem depois do https://
String firebaseHost = "link_firebase"; 

//definicao da apn do cartao sim
String APN_cartaoSIM = "internet.nos.pt";
// Definição dos pinos
#define RX_PIN 16
#define TX_PIN 17
#define PWK_PIN 12
#define RTS_PIN 14
#define CTS_PIN 5
#define RI_PIN 18

#define LED1_PIN 15
#define LED2_PIN 2  
#define LED3_PIN 4  

#define SDA_PIN 21  // Pino SDA no ESP32
#define SCL_PIN 22  // Pino SCL no ESP32


#endif
