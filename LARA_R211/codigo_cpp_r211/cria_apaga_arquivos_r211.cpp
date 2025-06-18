#include "../codigo_h_r211/cria_apaga_arquivos_r211.h"

void CriaGuardaDadosJsonMagnetometro(float mag_x, float mag_y, float mag_z,float B,float rhall, String data_a_enviar) {


  char enviar_json_magnetometro[256];  // Buffer para armazenar o JSON formatado

  // Monta o JSON com os valores passados
  snprintf(enviar_json_magnetometro, sizeof(enviar_json_magnetometro),
           "{\"Magnetometro_x\":%.4f,\"Magnetometro_y\":%.4f,\"Magnetometro_z\":%.4f,\"B\":%.4f,\"rhall\":%.4f,\"timestamp_rtc\":\"%s\",\"timestamp_server\": { \".sv\": \"timestamp\" }}",
           mag_x, mag_y, mag_z,B,rhall, data_a_enviar.c_str());

  int jsonSize_magnetometro = strlen(enviar_json_magnetometro);

  Serial2.print("AT+UDWNFILE=\"data_magnetometro.json\",");
  Serial2.println(jsonSize_magnetometro);
  delay(50);

  Serial2.print(enviar_json_magnetometro);
  Serial2.println("");
  delay(250);

  sendATCommand("AT+URDFILE=\"data_magnetometro.json\"");
  delay(2000);
}


void CriaGuardaDadosJsonGiroscopio(float gyro_x, float gyro_y, float gyro_z, String data_a_enviar) {
  char enviar_json_giroscopio[256];  // Buffer para armazenar o JSON formatado

  // Monta o JSON com os valores passados
  snprintf(enviar_json_giroscopio, sizeof(enviar_json_giroscopio),
           "{\"Giroscopio_x\":%.4f,\"Giroscopio_y\":%.4f,\"Giroscopio_z\":%.4f,\"timestamp_rtc\":\"%s\",\"timestamp_server\": { \".sv\": \"timestamp\" }}",
           gyro_x, gyro_y, gyro_z, data_a_enviar.c_str());

  int jsonSize_giroscopio = strlen(enviar_json_giroscopio);

  Serial2.print("AT+UDWNFILE=\"data_giroscopio.json\",");
  Serial2.println(jsonSize_giroscopio);
  delay(50);

  Serial2.print(enviar_json_giroscopio);
  Serial2.println("");
  delay(150);

  sendATCommand("AT+URDFILE=\"data_giroscopio.json\"");
  delay(2000);
}


void CriaGuardaDadosJsonAcelerometro(float temperatura, float acel_x, float acel_y, float acel_z,String orientation, String data_a_enviar) {
  char enviar_json_acelerometro[256];  // Buffer para armazenar o JSON formatado

  // Monta o JSON com os valores passados
  snprintf(enviar_json_acelerometro, sizeof(enviar_json_acelerometro),
           "{\"Temperatura\":%.4f,\"Acelerometro_x\":%.4f,\"Acelerometro_y\":%.4f,\"Acelerometro_z\":%.4f,\"Orientacao\":\"%s\",\"timestamp_rtc\":\"%s\",\"timestamp_server\": { \".sv\": \"timestamp\" }}",
           temperatura, acel_x, acel_y, acel_z, orientation.c_str(),data_a_enviar.c_str());

  int jsonSize_acelerometro = strlen(enviar_json_acelerometro);

  Serial2.print("AT+UDWNFILE=\"data_acelerometro.json\",");
  Serial2.println(jsonSize_acelerometro);
  delay(50);

  Serial2.print(enviar_json_acelerometro);
  Serial2.println("");
  delay(250);

  sendATCommand("AT+URDFILE=\"data_acelerometro.json\"");
  delay(2000);
}

void verificarEApagarArquivos() {
  String resposta = sendATCommandReturn("AT+ULSTFILE=");  // Lista arquivos
  delay(1000);                                            // Espera resposta (ajuste se necessário)

  // Verifica se os arquivos estão presentes e envia comando para apagá-los
  if (resposta.indexOf("data_acelerometro.json") != -1) {
    sendATCommand("AT+UDELFILE=\"data_acelerometro.json\"");
    delay(100);
  }

  if (resposta.indexOf("data_giroscopio.json") != -1) {
    sendATCommand("AT+UDELFILE=\"data_giroscopio.json\"");
    delay(100);
  }

  if (resposta.indexOf("data_magnetometro.json") != -1) {
    sendATCommand("AT+UDELFILE=\"data_magnetometro.json\"");
    delay(100);
  }
}
