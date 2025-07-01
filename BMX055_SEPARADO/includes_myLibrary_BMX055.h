#ifndef INCLUDES_MYLIBRARY_BMX055
#define INCLUDES_MYLIBRARY_BMX055

#include "myLibrary_BMX055/i2c_address_finder.cpp"
//--------------------------------------------------------------------------------------------------------
//Cada sensor tem um certo endereço i2c, dependendo da configuracao do nosso sensor estes podem mudar
//estas funcoes servem para encontrar o certo
//na funcao wire estava a dar erro ao usar uint8_t, pelo que estas funcoes retornam o endereço na forma int
//retorna o endereco i2c do registo do acelerometro
int findCorrectI2CAddress_ACC();
//retorna o endereco i2c do registo do giroscopio
int findCorrectI2CAddress_GYR();
//retorna o endereco i2c do registo do magnetometro
int findCorrectI2CAddress_MAG();
//--------------------------------------------------------------------------------------------------------


//serve para inicializar o sensor com umas certas configuracoes 
#include "myLibrary_BMX055/initialization.cpp"
//acelerometro--------------------------------------------------------------------------------------------
//no acelerometro configura o range (pag28;pag57)
//no acelerometro configura a largura de banda do filtro (pag27;pag57)
void initBMX055_ACC(int address, uint8_t range, uint8_t filter_bandwidth);
//--------------------------------------------------------------------------------------------------------
//giroscopio--------------------------------------------------------------------------------------------
//no giroscopio configura o range (pag99)
//no giroscopio configura a largura de banda do filtro (pag83;pag99)
void initBMX055_GYR(int address,uint8_t range, uint8_t filter_bandwidth);
//magnetometro--------------------------------------------------------------------------------------------
void initBMX055_MAG(int address, uint8_t rate );



//serve para ler os valores 
#include "myLibrary_BMX055/readBMX055.cpp"
//acelerometro--------------------------------------------------------------------------------------------
//ele nao retornar nada, ira alterar a variavel que se coloca dentro da funcao
//precisamos do range pois dependendo do range vai variar a sensibilidade(resolucao) do sensor
//da os valores da aceleracao nas componentes x,y,z e a temperatura
void readBMX055_ACC(int address, uint8_t range, float &ax, float &ay, float &az,float &temperature);
//--------------------------------------------------------------------------------------------------------
//giroscopio--------------------------------------------------------------------------------------------
//ele nao retornar nada, ira alterar a variavel que se coloca dentro da funcao
//precisamos do range pois dependendo do range vai variar a sensibilidade(resolucao) do sensor
//da os valores do giroscopio nas componentes x,y,z
void readBMX055_GYR(int address,uint8_t range, float &gx, float &gy, float &gz);
//--------------------------------------------------------------------------------------------------------
//magnetometro--------------------------------------------------------------------------------------------
int overflow_MAG (int address);
int readBMX055_MAG(int address ,float &B, float &mx, float &my, float &mz, float &rhall);



#include "myLibrary_BMX055/orientacao.cpp"
//acelerometro--------------------------------------------------------------------------------------------
//o proprio registo do acelerometro tem uns bits a que corresponde a orientacao do sensor (pag40;pag56)

//serve para configurar qual o modo de orientacao (pag40)(ele ira detetar uma certa orientacao para diferentes angulos)
void set_orientacion_mode_ACC(uint8_t ORIENT_MODE);
//retorna os bits que detetam se o sensor esta na vertical(para cima ou para baixo) e na horizontal (esquerda e direita)
uint8_t readOrientBits_ACC();
//retorna os bits que detetam se o sensor esta simplesmente em cima de uma mesa virado para cima ou para baixo
uint8_t readFlat_ACC();
//interpreta aqueles bits, retornando uma string a dizer como o sensor esta
String interpretOrientation_ACC(uint8_t OrientBits, uint8_t Flat);
//--------------------------------------------------------------------------------------------------------


#include "myLibrary_BMX055/self_test.cpp"
//acelerometro--------------------------------------------------------------------------------------------
//o proprio acelerometro tem disponivel uma opcao para fazer o teste para cada coordenada (pag29;pag73)
//para ver se o sensor esta bom
//a funcao ira escrever no Serial se os valores estao bons, alem disso, se o sensor esta bom 
//ela retorna 1, se mau retorna 0
int SelfTest_ACC(int address);
//--------------------------------------------------------------------------------------------------------
//giroscopio--------------------------------------------------------------------------------------------
//o proprio giroscopio tem disponivel uma opcao para fazer o teste (pag84;pag117)
//para ver se o sensor esta bom
//a funcao ira escrever no Serial se os valores estao bons, alem disso, se o sensor esta bom 
//ela retorna 1, se mau retorna 0
int SelfTest_GYR(int address);
//--------------------------------------------------------------------------------------------------------

#endif
