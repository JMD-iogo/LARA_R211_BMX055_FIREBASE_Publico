#ifndef HORAS_RTC_H
#define HORAS_RTC_H

#include <RTClib.h>
#include <Wire.h>


class HorasRTC {
public:

  HorasRTC();  // Construtor
  void ajustarHoraViaWiFi(const char* ssid, const char* senha);
  bool isHorarioDeVerao(int mes, int dia);
  void setupRTC(const char* ssid, const char* senha);                                                                 // Configura o RTC (ajuste de hora)
  void ajustarHora(int ano, int mes, int dia, int hora, int minuto, int segundo);  // Ajuste manual da hora
  void exibirHora();                                                               // Exibe a hora no Monitor Serial
  DateTime obterHora();                                                            // Obtém a hora atual

  void obtem_horas_convert_utc();
  String data_enviar();
private:
  RTC_DS3231 rtc;  // Instância do DS3231
};

#endif
