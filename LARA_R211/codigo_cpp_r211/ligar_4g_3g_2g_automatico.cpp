//caso necessario mudar a APN, neste momento é "internet.nos.pt"
#include "../codigo_h_r211/ligar_4g_3g_2g_automatico.h"
#include "../../definicoes.h"
void testInternet() {
  // Step 2: Configure the SMTP server (Sapo)
  sendATCommand("AT+UHTTP=2,1,\"www.u-blox.com\"");
  delay(5000);
  sendATCommand("AT+UHTTP=2,1");
  sendATCommand("AT+UHTTP=0");
  delay(5000);
}

void initGPRS() {
  Serial.println("Iniciando conexão de dados móveis (WOO/NOS)...");

  // Verifica se está registrado na rede
  //sendATCommand("AT+CREG?");
  //delay(1000);
  //sendATCommand("AT+CSQ");
  //delay(1000);
  // Verifica se está anexado à rede de dados
  //sendATCommand("AT+CGATT?");
  //delay(1000);

  // // Define o APN da operadora WOO/NOS
  // sendATCommand("AT+CGDCONT=1,\"IP\",\"internet.nos.pt\"");
  // delay(1000);

  // // Ativa o contexto de dados (PDP)
  // sendATCommand("AT+CGACT=1,1");
  // delay(2000);

  // // Verifica se foi ativado com sucesso
  // sendATCommand("AT+CGACT?");
  // delay(1000);

  // // (Opcional) Ver IP atribuído
  // sendATCommand("AT+CGPADDR=1");
  // delay(1000);

  // sendATCommand("AT+CGDCONT?");
  // delay(2000);

  sendATCommand("AT+CGACT=0,1");
  delay(2000);

  sendATCommand(("AT+CGDCONT=1,\"IP\",\"" + APN_cartaoSIM + "\"").c_str());
  //sendATCommand("AT+CGDCONT=1,\"IP\",\"internet.nos.pt\"");
  delay(3000);
  sendATCommand("AT+CGACT=1,1");
  delay(2000);

  sendATCommand("AT+CGDCONT?");
  delay(2000);


  //Ativa a conexão de dados pelo método UPSDA
  sendATCommand("AT+UPSDA=0,3");  // Ativa perfil 0
  delay(3000);                    // Espera conexão

  sendATCommand("AT+UPSND=0,8");
  delay(2000);
  sendATCommand("AT+UPSND=0,0");  // Ver IP atribuído
  delay(1000);
  Serial.println("Conexão de dados móveis (WOO) ativada.");
}

void testPING() {
  sendATCommand("AT+UPING=\"8.8.8.8\"");
}

//----------------------------------------------------


