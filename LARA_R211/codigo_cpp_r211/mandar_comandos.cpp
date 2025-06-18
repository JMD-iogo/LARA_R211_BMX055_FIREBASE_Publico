#include "../codigo_h_r211/mandar_comandos.h"

void sendATCommand(const char* command, int delayMs) {
  Serial.print("Enviando comando: ");
  Serial.println(command);

  unsigned long startTime = millis();
  while (digitalRead(RTS_PIN) == HIGH) {
    if (millis() - startTime > 5000) {
      Serial.println("RTS ainda está HIGH, abortando envio...");
      return;
    }
    delay(100);
  }

  Serial2.println(command);
  delay(delayMs);
  readResponse();
}

// Fica true se responder OK, false se ERROR ou timeout
bool sendATCommand_delay_automatico(String cmd, String expectedOK, String expectedERR) {
  Serial2.println(cmd);

  unsigned long timeout = 5000;
  unsigned long start = millis();
  String response = "";

  while (millis() - start < timeout) {
    while (Serial2.available()) {
      char c = Serial2.read();
      response += c;

      if (response.indexOf(expectedERR) != -1) {
        Serial.println("❌ Erro recebido: " + response);
        return false;
      }

      if (response.indexOf(expectedOK) != -1) {
        Serial.println("✅ Recebido: " + response);
        return true;
      }
    }
  }

  Serial.println("⏱ Timeout sem resposta válida: " + response);
  return false;
}



String sendATCommandReturn(String command, unsigned long timeout) {
  Serial2.println(command);  // Substitua por Serial2 ou SoftwareSerial se necessário
  String response = "";
  unsigned long start = millis();
  while (millis() - start < timeout) {
    while (Serial2.available()) {
      response += Serial2.readString();
    }
  }
  Serial.println(response);  // Debug no monitor serial
  return response;
}

void readResponse() {
  unsigned long timeout = millis() + 5000;
  bool dataReceived = false;

  while (millis() < timeout) {
    while (Serial2.available()) {
      char c = Serial2.read();
      if (isPrintable(c) || c == '\n' || c == '\r') {
        Serial.write(c);
        dataReceived = true;
      }
    }

    if (dataReceived) {
      timeout = millis() + 500;
      dataReceived = false;
    }
  }
  Serial.println();
}