#ifndef HORAS
#define HORAS

#include <Arduino.h>
#include <HardwareSerial.h>

struct Tempo {
  int hora;
  int minuto;
  int segundo;
  String fuso;
};
struct Data {
  int ano;
  int mes;
  int dia;
};

// Funções
Tempo extrairTempo(String dataHora);
Data extrairData(String dataHora);
String da_horas();
void atualiza_horas();
#endif