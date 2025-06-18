#include "../codigo_h_r211/power_r211.h"

//funcao que liga o modulo
void powerOnModule() {
  Serial.println("Ligando módulo...");

  //    while (digitalRead(PWK_PIN) == LOW) {

  //   // O código fica preso aqui até que o pino PWK seja HIGH
  //   Serial.println("Aguardando pino PWK HIGH...");
  //   delay(1000);  // Opcional: Delay para evitar saturação do serial
  // }

  //forca este pinos a ficarem low durante meio segundo
 // pinMode(RTS_PIN, OUTPUT);
 // pinMode(PWK_PIN, OUTPUT);
 // digitalWrite(RTS_PIN, LOW);
 // digitalWrite(PWK_PIN, LOW);
 // delay(500);

  //coloca os pinos flutuantes,  pois o pwk esta HIGH, devido a um pull-up interno do lara r211
 // pinMode(PWK_PIN, INPUT);
 // pinMode(RTS_PIN, INPUT);
 // delay(2000);

 // while (digitalRead(RTS_PIN) == HIGH) {
 //   Serial.println("RTS está HIGH, aguardando...");
  //  delay(2000);
 // }

  sendATCommand("+CSGT AT"); //este comando é essencial, sem este o modulo nao acorda
  delay(2000);

  Serial.println("Módulo ligado.");
  delay(2000);
}

//desativa de forma correta o modulo
void powerOffModule() {
  Serial.println("Desligando módulo...");
  sendATCommand("AT+CPWROFF");
  delay(2000);

  pinMode(PWK_PIN, OUTPUT);
  digitalWrite(PWK_PIN, LOW);
  delay(3000);
  pinMode(PWK_PIN, INPUT);
  delay(2000);

  Serial.println("Módulo desligado.");
}