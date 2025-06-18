#ifndef CRIA_APAGA_ARQUIVOS
#define CRIA_APAGA_ARQUIVOS

//cria_apaga_arquivos_r211
void verificarEApagarArquivos();
void CriaGuardaDadosJsonAcelerometro(float temperatura, float acel_x, float acel_y, float acel_z,String orientation, String data_a_enviar);
void CriaGuardaDadosJsonGiroscopio(float gyro_x, float gyro_y, float gyro_z, String data_a_enviar);
void CriaGuardaDadosJsonMagnetometro(float mag_x, float mag_y, float mag_z,float B,float rhall, String data_a_enviar);



#endif