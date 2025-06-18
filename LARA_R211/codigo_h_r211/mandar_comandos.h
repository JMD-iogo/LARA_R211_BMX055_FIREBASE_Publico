#ifndef MANDAR_COMANDOS
#define MANDAR_COMANDOS


#include <Arduino.h>
#include <HardwareSerial.h>
#include "../../definicoes.h"

//mandar_comadandos
void sendATCommand(const char* command, int delayMs = 3000);
String sendATCommandReturn(String command, unsigned long timeout = 2000);
void readResponse();
bool sendATCommand_delay_automatico(String cmd, String expectedOK, String expectedERR);


#endif