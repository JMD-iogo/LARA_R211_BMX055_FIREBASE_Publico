#include "horas_rtc.h"

HorasRTC::HorasRTC() {
  // Construtor vazio
}
bool HorasRTC::isHorarioDeVerao(int mes, int dia) {
  // Regra da União Europeia:
  // Começa no último domingo de março
  // Termina no último domingo de outubro
  if (mes < 3 || mes > 10) return false;
  if (mes > 3 && mes < 10) return true;
  if (mes == 3 && dia >= 25) return true;  // aproximação
  if (mes == 10 && dia < 25) return true;  // aproximação
  return false;
}

void HorasRTC::setupRTC() {

  if (!rtc.begin()) {
    Serial.println("Não foi possível encontrar o RTC.");
    while (1)
      ;  // Se não conseguir encontrar o RTC, trava aqui.
  }

  // Verifica se a hora foi perdida e ajusta automaticamente
  if (rtc.lostPower()) { //para ele atualizar temos de desligar o modulo uns segundos e nao pode estar com a pilha se isto tiver com este if
    //tirar pilha, desligar do esp, carregar codigo, colocar a pilha e siga caminho, hora atualizada pela do computador, depois ele ira colocar em utc+0 (o do computador tem de estar em portugal)
    Serial.println("A configuração do tempo foi perdida. Definindo a hora.");
    // Ajusta a hora com base na hora de compilação
    obtem_horas_convert_utc();
  }
}

void HorasRTC::ajustarHora(int ano, int mes, int dia, int hora, int minuto, int segundo) {
  // Ajusta manualmente a data e hora
  rtc.adjust(DateTime(ano, mes, dia, hora, minuto, segundo));
  Serial.println("Hora ajustada com sucesso!");
}



String HorasRTC::data_enviar(){
  DateTime now = rtc.now();  // Obtém a hora atual do RTC

  char buffer[20];
  snprintf(buffer, sizeof(buffer), "%02d/%02d/%02d,%02d:%02d:%02d",
           now.year() % 100, now.month(), now.day(),
           now.hour(), now.minute(), now.second());
  return String(buffer);
}

DateTime HorasRTC::obterHora() {
  return rtc.now();  // Retorna a hora atual
}

//esta muda pra utc, pega na hora do compilador, pelo que so serve caso nao se desligue depois de carregar o codigo
//caso contrario temos de mandar um sms para alterar as horas
void HorasRTC::obtem_horas_convert_utc() {
  DateTime local = DateTime(F(__DATE__), F(__TIME__));

  int offset = 0;
  if (isHorarioDeVerao(local.month(), local.day())) {
    offset = -3600;  // Subtrai 1h no verão
  }

  DateTime utc(local.unixtime() + offset);  // Aplica o offset
  rtc.adjust(utc);
}


void HorasRTC::exibirHora() {
  DateTime now = rtc.now();  // Obtém a hora atual do RTC

  // Exibe a hora no Monitor Serial
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();
}
