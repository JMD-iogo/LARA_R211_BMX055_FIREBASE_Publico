//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//Autor : Diogo Ferreia
//https://pt.mouser.com/datasheet/2/783/BST_BMX055_DS000-1509552.pdf
//paginas relevantes na criacao disto: 29,49, 73(ACC Register 0x32);
//criacao da minha biblioteca, este em especifico serve para fazer o self test do sensor acelerometro.
//ele comeca por colocar as configuracoes default(para elimar possiveis filtros, por exemplo o filtro com a largura de banda de 7Hz, tem um update time de 64ms, como o teste é rapido neste caso ele nao iria detetar diferencas
//configura para o range 8g
//temos de fazer para cada um dos eixos
//no endereco 0x32, temos selecionar qual eixo vamos testar, colocando o self_test_axis bits de uma certa forma
//o self_test_amp tem de ser = 1b
//vamos fazer 2 medicoes, uma com o self_test_sign = 0b e outra =1b
//de seguida le-se os valores nos enderecos normais dos valores da aceleracao, e calculamos a diferenca
//para cada eixo para o caso self_test_sign = 0b e =1b
//cada subtracao tem um valor minimo que se encontra na tabela 9, pag29, caso seja inferior que essses valores o sensor nao esta bom
//faz o print da subtracao e o valor minimo que esta pode ter 
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#include <Wire.h>
#include "../includes_myLibrary_BMX055.h"
//pag29
int SelfTest_ACC(int address) {
   Serial.println("A iniciar o self_test....");
   //coloca as configuracoes default
  Wire.beginTransmission(address);
  Wire.write(0x14);
  Wire.write(0xB6);
  Wire.endTransmission();
  delay(1000);  

  Wire.beginTransmission(address);
  Wire.write(0x0F);  // Configuração do range
  Wire.write(0x08);  // Define range para 8g
  Wire.endTransmission();
  delay(100);
  //bit7,6,5,3 nao usado(colocar 0)
  //bit 4, self_test_amp (colocar 1)
  //bit 2, self_test_sign (colocar 1 para positivo, colocar 0 para negativo) fazer a diferenca de ambos
  //bit 1,0 , define o eixo a usar
  //uint8_t axes_positive[3] = {0b00010101, 0b00010110, 0b00010111}; // X, Y, Z
  uint8_t axes_positive[3] = { 0x15, 0x16, 0x17 };  // X, Y, Z
  // uint8_t axes_negative[3] = {0b00010001, 0b00010010, 0b00010011}; // X, Y, Z
  uint8_t axes_negative[3] = { 0x11, 0x12, 0x13 };  // X, Y, Z
  uint8_t regs[3] = { 0x02, 0x04, 0x06 };           // Registradores de dados para X, Y, Z
  float values_positive[3];
  float values_negative[3];

//vai percorrer as 3 coordenadas
  for (int i = 0; i < 3; i++) {
    // Ativar o self-test para o eixo correspondente
    Wire.beginTransmission(address);
    Wire.write(0x32);
    Wire.write(axes_positive[i]);
    Wire.endTransmission();
    delay(100);

    // Ler o dado do positivo
    Wire.beginTransmission(address);
    Wire.write(regs[i]);
    Wire.endTransmission();
    Wire.requestFrom(address, 2);
    values_positive[i] = ((Wire.read() | (Wire.read() << 8)) >> 4) * 0.00391;  // O deslocamento de 4 bits é necessário


    // Desativar o self-test
    Wire.beginTransmission(address);
    Wire.write(0x32);
    Wire.write(0b00000000);
    Wire.endTransmission();
    delay(250);

    // Ativar o self-test para o eixo correspondente
    Wire.beginTransmission(address);
    Wire.write(0x32);
    Wire.write(axes_negative[i]);
    Wire.endTransmission();
    delay(100);

    // Ler o dados do negativo
    Wire.beginTransmission(address);
    Wire.write(regs[i]);
    Wire.endTransmission();
    Wire.requestFrom(address, 2);
    values_negative[i] = ((Wire.read() | (Wire.read() << 8)) >> 4) * 0.00391;  // O deslocamento de 4 bits é necessário
  }

  // Calcular as diferenças entre o negativo e positivo
  float t1=abs(values_positive[0] - values_negative[0]) * 1000;
  float t2=abs(values_positive[1] - values_negative[1]) * 1000;
  float t3=abs(values_positive[2] - values_negative[2]) * 1000;
  Serial.print("Diff X: ");
  Serial.print(t1);
  Serial.println(" mg (tem de ser >800mg)");
  Serial.print("Diff Y: ");
  Serial.print(t2);
  Serial.println(" mg (tem de ser >800mg)");
  Serial.print("Diff Z: ");
  Serial.print(t3);
  Serial.println(" mg (tem de ser >400mg)");

  
  delay(100);
  
  //coloca as configuracoes default
  Wire.beginTransmission(address);
  Wire.write(0x14);
  Wire.write(0xB6);
  Wire.endTransmission();
  delay(5000);                                              //demora a fazer o reset
  if(t1>800 & t2>800 & t3>400){
    Serial.println("Sensor com valores dentro dos limites");
    Serial.println("Fim do self_test....");
    return 1;

  }
  else{
    Serial.println("Sensor com valores fora dos limites");
    Serial.println("Fim do self_test....");
    return 0;
  }
}


//pag84 /72 /92
int SelfTest_GYR(int address) {
  Serial.println("A iniciar SelfTest....");
  // reset para nao influenciar nos testes
  Wire.beginTransmission(address);
  Wire.write(0x14);
  Wire.write(0xB6);  // Sensibilidade de 500°/s por exemplo
  Wire.endTransmission();
  delay(2000);


    // ativar o self test
    Wire.beginTransmission(address);
  Wire.write(0x3C);
  Wire.write(1);  // Sensibilidade de 500°/s por exemplo
  Wire.endTransmission();
  delay(2000);

    Wire.beginTransmission(address);
  Wire.write(0x3C);
  Wire.endTransmission();
  Wire.requestFrom(address, 1);  // Solicita 1 byte de dados
                                 // uint8_t valor = Wire.read();  // Lê o byte de dados
  uint8_t valor = Wire.read();   // Lê o byte de dados
  uint8_t mascara = 0b00000010;  //(bist_rdy)



  // o bit 1 (bist_rdy) do 0x3c esta = 1, significa que o teste foi bem sucedido
  if (valor & mascara) {
    Serial.println("SelfTest bem suscedido");
    delay(2000);

    Wire.beginTransmission(address);
    Wire.write(0x3C);
    Wire.endTransmission();
    Wire.requestFrom(address, 1);  // Solicita 1 byte de dados
                                   // uint8_t valor = Wire.read();  // Lê o byte de dados
    uint8_t valor1 = Wire.read();  // Lê o byte de dados
    uint8_t mascara1 = 0b00000100;
    if (valor & mascara) {
      Serial.println("Sensor com valores dentro do limite ");
      // reset para nao influenciar  os nosso valores
      Wire.beginTransmission(address);
      Wire.write(0x14);
      Wire.write(0xB6);  
      Wire.endTransmission();
      delay(2000);
       return 1;
    } else {
      Serial.println("Sensor com valores FORA do limite ");
      // reset para nao influenciar  os nosso valores
      Wire.beginTransmission(address);
      Wire.write(0x14);
      Wire.write(0xB6);  
      Wire.endTransmission();
      delay(2000) ;
      return 0;
    }
  }
  // o bit 1 (bist_rdy) do 0x3c esta = 0, significa que o teste foi mal sucedido
  else {
    Serial.println("SelfTest mal suscedido");

    // reset para nao influenciar os nosso valores
    Wire.beginTransmission(address);
    Wire.write(0x14);
    Wire.write(0xB6);  // Sensibilidade de 500°/s por exemplo
    Wire.endTransmission();
    delay(2000) ;
    return 0;
  }
}