#include "../codigo_h_r211/certificado.h"

void waitForAtPrompt() {
  unsigned long start = millis();
  while (millis() - start < 5000) {
    if (Serial2.available()) {
      char c = Serial2.read();
      if (c == '@') {
        Serial.println("Recebido @, pronto para enviar!");
        return;
      } else {
        Serial.write(c);
      }
    }
  }
  Serial.println("Não recebeu @ :(");
}

void readResponse1() {
  while (Serial2.available()) {
    Serial.write(Serial2.read());
  }
}


//serve para remover algum certificado indesejado, ele ira apresentar
//os certificados disponiveis e pede para escrever o certificado no serial a remover
//pode copiar o nome do certificado e colar diretamente no serial monitor para o remover
void remover_certificado() {
    // Mostra os certificados atuais antes da remoção
  sendATCommand("AT+USECMNG=3");
  delay(500);
  Serial.println("Digite o nome do certificado no Serial que deseja remover:");
  
  // Aguarda até que haja algo disponível na Serial
  while (Serial.available() == 0) {
    // espera input
  }

  // Lê a linha digitada
  String nome_certificado = Serial.readStringUntil('\n');
  nome_certificado.trim(); // Remove espaços e quebras de linha extras

  // Confirma o nome
  Serial.print("Removendo o certificado: ");
  Serial.println(nome_certificado);

  // Monta e envia o comando de remoção
  String comando = "AT+USECMNG=2,0,\"" + nome_certificado + "\"";
  sendATCommand(comando.c_str());
  delay(500);

  // Lista novamente os certificados para confirmar remoção
  sendATCommand("AT+USECMNG=3");
  delay(500);
}

//manda const byte cert_google_root[]  (usar nome_certificado.DER )
//esta variavel esta definida no certificado.h
void mandar_certificado(){
   Serial.println("Enviando comando AT+USECMNG...");

  Serial2.print("AT+USECMNG=0,0,\"ThawteCA\",");
  const int cert_len = sizeof(cert_google_root);
  Serial2.print(cert_len);
  Serial2.print("\r\n");

  waitForAtPrompt();

  Serial.println("Enviando certificado binário...");
  Serial2.write(cert_google_root, cert_len);  // Envia os dados binários

  delay(1000);
  readResponse1();

  delay(1000);
sendATCommand("AT+USECMNG=3");
  delay(1000);

}

//manda const char* root_ca (funcionou) (usar nome_certificado.PEM )
//esta variavel esta definida no certificado.h
void mandar_certificado_texto() {
  Serial.println("Enviando comando AT+USECMNG...");

  int cert_len = strlen(root_ca); // Calcula o tamanho do PEM

  //nome que ficara no LARA R211
  Serial2.print("AT+USECMNG=0,0,\"root_ca_google\",");
  Serial2.print(cert_len);
  Serial2.print("\r\n");

  waitForAtPrompt();  // Aguarda o prompt '>' para enviar os dados

  Serial.println("Enviando certificado PEM...");
  Serial2.write((const uint8_t*)root_ca, cert_len);  // Envia o certificado como texto

  delay(1000);
  readResponse1();

  delay(1000);
  sendATCommand("AT+USECMNG=3");
  delay(1000);
}

void ver_certificados(){
  sendATCommand("AT+USECMNG=3");
  delay(1000);

}