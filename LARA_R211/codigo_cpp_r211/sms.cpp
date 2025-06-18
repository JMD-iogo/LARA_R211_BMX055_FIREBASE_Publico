#include "../codigo_h_r211/sms.h"

void sendSMS(String phoneNumber, String message) {
  sendATCommand("AT+CMGF=1");
  delay(100);
  sendATCommand(("AT+CMGS=\"" + phoneNumber + "\"").c_str());
  delay(100);
  sendATCommand((message + "\x1A").c_str());
}

//vai ler o sms e usando outra funcao retorna so o corpo do texto
String readSMS() {
  sendATCommand("AT+CMGF=1");  // Define o modo de texto
  delay(100);
  String sms_recebido = sendATCommandReturn("AT+CMGL=\"ALL\"");  // Lista todas as mensagens
  Serial.println(sms_recebido);
  delay(500);
  // Apaga todas as mensagens

  sendATCommand("AT+CMGD=0,4");
  delay(100);

  return sms_recebido;
}
// Função para extrair o corpo da mensagem
String extractMessageBody(String response) {
  int cmglIndex = response.indexOf("+CMGL:");
  if (cmglIndex == -1) return "";

  // Encontra o fim da linha com "+CMGL:"
  int firstNewline = response.indexOf('\n', cmglIndex);
  if (firstNewline == -1) return "";

  // Encontra a próxima quebra de linha depois do corpo
  int secondNewline = response.indexOf('\n', firstNewline + 1);

  String body;
  if (secondNewline == -1) {
    // Se não encontrar outra quebra de linha, vai até o fim
    body = response.substring(firstNewline + 1);
  } else {
    // O corpo está entre as duas quebras de linha
    body = response.substring(firstNewline + 1, secondNewline);
  }

  body.trim();  // Remove espaços ou \r no início/fim
  return body;
}




String extractMessageNumber(String response, String prefixo_numero) {
  int start = response.indexOf(prefixo_numero);
  if (start == -1) {
    Serial.println("Prefixo não encontrado.");
    return "";
  }

  int end = response.indexOf('\",', start);  // encontra a vírgula depois do número

  if (end == -1) {
    Serial.println("Vírgula após o número não encontrada.");
    return "";
  }

  String phoneNumber = response.substring(start, end - 1);
  //Serial.println("Numero da mensagem:");
  //Serial.println(phoneNumber);
  return phoneNumber;
}
