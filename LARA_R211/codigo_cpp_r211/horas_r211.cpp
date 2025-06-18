#include "../codigo_h_r211/horas_r211.h"

//atualiza as horas automaticamente de acordo com a rede
void atualiza_horas(){
  sendATCommand("AT+CTZU=1");
}

// da o horario de inverno
String da_horas()
{

  String horas = sendATCommandReturn("AT+CCLK?");
  Serial.print("horas: ");
  Serial.println(horas);

  // Encontra o índice onde começa o +CCLK:
  int startIndex = horas.indexOf("\"");               // Primeiro aspas
  int endIndex = horas.indexOf("\"", startIndex + 1); // Próxima aspas

  String horaExtraida = horas.substring(startIndex + 1, endIndex);

  Serial.print("hora extraida: ");
  Serial.println(horaExtraida);
  return horaExtraida;
}

//extrai as horas da string a cima e coloca-as num objeto Tempo
Tempo extrairTempo(String dataHora) {
  Tempo t;
  int splitIndex = dataHora.indexOf(',');
  String horaCompleta = dataHora.substring(splitIndex + 1); // "00:00:18+04"

  t.hora = horaCompleta.substring(0, 2).toInt();
  t.minuto = horaCompleta.substring(3, 5).toInt();
  t.segundo = horaCompleta.substring(6, 8).toInt();
  t.fuso = horaCompleta.substring(8); // "+04"

  return t;
}

//extrai a data da string a cima e coloca-as num objeto Tempo
Data extrairData(String dataHora) {
  Data d;

  int splitIndex = dataHora.indexOf(',');
  String data = dataHora.substring(0, splitIndex); // "04/01/01"

  d.ano = data.substring(0, 2).toInt();
  d.mes = data.substring(3, 5).toInt();
  d.dia = data.substring(6, 8).toInt();

  return d;
}

//utc "universal time" -- este trabalhar 
//unix time
//local time 

//empacotar alguns dados e depois enviar de 15 em 15 segundos