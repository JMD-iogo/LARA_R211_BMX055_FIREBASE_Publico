#include "horas_rtc.h"  

HorasRTC rtc;  

void setup() {
  Serial.begin(115200);
  int sdaPin = 21;
  int sclPin = 22;
  Wire.begin(sdaPin, sclPin);

  //rtc.setupRTC("SSID", "SENHA");
  rtc.setupRTC("nome", "pass"); //ajusta as horas com o wifi

}

void loop() {
  // put your main code here, to run repeatedly:
  String data_a_enviar = rtc.data_enviar();
  Serial.println("horas:" + data_a_enviar);
  delay(5000);
}
