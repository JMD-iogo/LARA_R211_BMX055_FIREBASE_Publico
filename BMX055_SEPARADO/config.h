//estao definitas aqui todas as possiveis configuracoes do acelerometro

//acelerometro--------------------------------------------------------------------------------------------
//PAG 28
// Definição das escalas do acelerômetro
#define RANGE_ACC_2G 0x03
#define RANGE_ACC_4G 0x05
#define RANGE_ACC_8G 0x08
#define RANGE_ACC_16G 0x0C

//PAG 27
// definicao da largura de banda do filtro acelerometro
#define NO_FILTER 0
#define FILTER_BANDWIDTH_ACC_7Hz 0x08     //update time = 64ms
#define FILTER_BANDWIDTH_ACC_15Hz 0x09    //update time = 32ms
#define FILTER_BANDWIDTH_ACC_31Hz 0x0A    //update time = 16ms
#define FILTER_BANDWIDTH_ACC_62Hz 0x0B    //update time = 8ms
#define FILTER_BANDWIDTH_ACC_125Hz 0x0C   //update time = 4ms
#define FILTER_BANDWIDTH_ACC_250Hz 0x0D   //update time = 2ms
#define FILTER_BANDWIDTH_ACC_500Hz 0x0E   //update time = 1ms
#define FILTER_BANDWIDTH_ACC_1000Hz 0x0F  //update time = 0.5ms

//pag40
//definicao o modo de orientacao(muda os angulos a que o sensor deteta a orientacao)
#define ORIENT_MODE_SYMMETRICAL_ACC 0b00
#define ORIENT_MODE_HIGH_ASYMMETRICAL_ACC 0b01
#define ORIENT_MODE_LOW_ASYMMETRICAL_ACC 0b10
//--------------------------------------------------------------------------------------------------------

//giroscopio----------------------------------------------------------------------------------------------
//pag99
// Definição das escalas do giroscopio
#define RANGE_GYR_2000 0x0 //2000graus por s   resolucao: 1LSB equivale a 0.061 graus por segundo 
#define RANGE_GYR_1000 0x1 //1000graus por s   resolucao: 1LSB equivale a 0.0305 graus por segundo
#define RANGE_GYR_500 0x2 //500graus por s   resolucao: 1LSB equivale a 0.0153 graus por segundo
#define RANGE_GYR_250 0x3 //250graus por s   resolucao: 1LSB equivale a 0.0076 graus por segundo
#define RANGE_GYR_125 0x4 //125graus por s   resolucao: 1LSB equivale a 0.0038 graus por segundo

//PAG 99/100
// definicao da largura de banda do filtro giroscopio
#define NO_FILTER_GYR 0x0  //Output Data Rate 2000Hz
#define FILTER_BANDWIDTH_GYR_230Hz 0x1     //Output Data Rate 2000Hz
#define FILTER_BANDWIDTH_GYR_116Hz 0x2    //Output Data Rate 1000Hz
#define FILTER_BANDWIDTH_GYR_47Hz 0x3    //Output Data Rate 400Hz
#define FILTER_BANDWIDTH_GYR_23Hz 0x4    //Output Data Rate 200Hz
#define FILTER_BANDWIDTH_GYR_12Hz 0x5   //Output Data Rate 100Hz
#define FILTER_BANDWIDTH_GYR_64Hz 0x6   //Output Data Rate 200Hz
#define FILTER_BANDWIDTH_GYR_32Hz 0x7   //Output Data Rate 100Hz
//--------------------------------------------------------------------------------------------------------

//magnetometro--------------------------------------------------------------------------------------------
//PAG 28
// Definição das escalas do magnetometro
#define RATE_MAG_10Hz 0x00 //(default)
#define RATE_MAG_2Hz 0x01
#define RATE_MAG_6Hz 0x02
#define RATE_MAG_8Hz 0x03
#define RATE_MAG_15Hz 0x04
#define RATE_MAG_20Hz 0x05
#define RATE_MAG_25Hz 0x06
#define RATE_MAG_30Hz 0x07

//--------------------------------------------------------------------------------------------------------
