#ifndef PRINT_GUARDA
#define PRINT_GUARDA

//caso a variavel qual_registo seja ="ACC" a funcao retorna 1, ...
//usando esta na serial_print_save_XX, podemes decidir qual quardar (no formato aceitavel pelo codigo python)
//ou se damos print de todas, ou so de uma, podendo ver pelo serial do arduino
int guardar(String qual_registo) {
  qual_registo.toUpperCase();  // Converte para maiúsculas

  if (qual_registo == "ACC") {
    return 1;} //direcionado a guardar os dados do acelerometro
  if (qual_registo == "GYR") {
    return 2;}//direcionado a guardar os dados do giroscopio
  if (qual_registo == "MAG") {
    return 3;}//direcionado a guardar os dados do magnetometro
    if (qual_registo == "TODOS") {
    return 7;}//direcionado a guardar os dados do magnetometro
  if (qual_registo == "PRINT_ACC") {
    return 4;} //direcionado a ler os dados do acelerometro
  if (qual_registo == "PRINT_GYR") {
    return 5;}//direcionado a ler os dados do giroscopio
  if (qual_registo == "PRINT_MAG") {
    return 6;}//direcionado a ler os dados do magnetometro
  // Qualquer outra entrada retorna 0
  return 0;//direcionado a ler os dados de todos os sensores 
}

//-------------------------------------funcoes----------------------------------------------
//caso a variavel serial_print =1 ele ira dar so print dos valores do acelerometro, para poder
//adquirilos pelo codigo python pela porta serial, e se serial_print=0 fara dar print dos valores do ACC,GYR E MAG
// no serial
void serial_print_save_ACC(int serial_print, float ax, float ay, float az, float temperature, String orientation) {
  if (serial_print == 1) {
    //para o ficheiro python guardar corretamente os dados num exel
    Serial.print("ax(g),");
    Serial.print(ax);
    Serial.print(",ay(g),");
    Serial.print(ay);
    Serial.print(",az(g),");
    Serial.print(az);

    Serial.print(",Temperatura(ºC),");
    Serial.print(temperature);
    Serial.print(",Orientação,");
    Serial.println(orientation);
  }
if (serial_print == 7) {
    //para o ficheiro python guardar corretamente os dados num exel
    Serial.print("ax(g),");
    Serial.print(ax);
    Serial.print(",ay(g),");
    Serial.print(ay);
    Serial.print(",az(g),");
    Serial.print(az);

    Serial.print(",Temperatura(ºC),");
    Serial.print(temperature);
    Serial.print(",Orientação,");
    Serial.print(orientation);
  }


  if (serial_print == 0 || serial_print ==4) {
    //mais agradavel para se ler no Serial do arduino
    Serial.println("Acelerometro: ");
    Serial.print("ax(g): ");
    Serial.print(ax);
    Serial.print(" ; ay(g): ");
    Serial.print(ay);
    Serial.print(" ; az(g): ");
    Serial.println(az);

    Serial.print("Temperatura(ºC): ");
    Serial.println(temperature);
    Serial.print("Orientação: ");
    Serial.println(orientation);
    Serial.println("----------------------------------------");
  }
}
//caso a variaver serial_print =1 ele ira dar so print dos valores do giroscopio, para poder
//adquirilos pelo codigo python pela porta serial, e se serial_print=0 fara dar print dos valores do ACC,GYR E MAG
// no serial
void serial_print_save_GYR(int serial_print, float gx, float gy, float gz) {
  if (serial_print == 2) {
    //para o ficheiro python guardar corretamente os dados num exel
    Serial.print("gx(º/s),");
    Serial.print(gx);
    Serial.print(",gy(º/s),");
    Serial.print(gy);
    Serial.print(",gz(º/s),");
    Serial.println(gz);
  }
   if (serial_print == 7) {
    //para o ficheiro python guardar corretamente os dados num exel
    Serial.print(",gx(º/s),");
    Serial.print(gx);
    Serial.print(",gy(º/s),");
    Serial.print(gy);
    Serial.print(",gz(º/s),");
    Serial.print(gz);
  }

  if (serial_print == 0 || serial_print ==5) {
    // Print das variaveis:
    Serial.println("Giroscopio: ");
    Serial.print("gx=");
    Serial.print(gx);
    Serial.print(" º/s, gy=");
    Serial.print(gy);
    Serial.print("º/s, gz=");
    Serial.print(gz);
    Serial.println("º/s");

    Serial.println("----------------------------------------");
  }
}

//caso a variaver serial_print =1 ele ira dar so print dos valores do magnetometro, para poder
//adquirilos pelo codigo python pela porta serial, e se serial_print=0 fara dar print dos valores do ACC,GYR E MAG
// no serial
void serial_print_save_MAG(int serial_print, float B, float mx, float my, float mz, float rhall, float azimuth, float inclination) {
  if (serial_print == 3) {
    //para o ficheiro python guardar corretamente os dados num exel
    Serial.print("B(microT),");
    Serial.print(B);
    Serial.print(",mx(microT),");
    Serial.print(mx);
    Serial.print(",my(microT),");
    Serial.print(my);
    Serial.print(",mz(microT),");
    Serial.print(mz);
    Serial.print(",rhall,");
    Serial.print(rhall);
    Serial.print(",azimuth,");
    Serial.print(azimuth);
    Serial.print(",inclination,");
    Serial.println(inclination);
  }
   if (serial_print == 7) {
    //para o ficheiro python guardar corretamente os dados num exel
    Serial.print(",B(microT),");
    Serial.print(B);
    Serial.print(",mx(microT),");
    Serial.print(mx);
    Serial.print(",my(microT),");
    Serial.print(my);
    Serial.print(",mz(microT),");
    Serial.print(mz);
    Serial.print(",rhall,");
    Serial.print(rhall);
    Serial.print(",azimuth,");
    Serial.print(azimuth);
    Serial.print(",inclination,");
    Serial.println(inclination);
  }

  if (serial_print == 0|| serial_print ==6) {
    //mais agradavel para se ler no Serial do arduino
    Serial.println("Magnetometro: ");
    Serial.print("MAG: ");
    Serial.print("campo B=");
    Serial.print(B);
    Serial.print("microT, mx:");
    Serial.print(mx);
    Serial.print("microT, my:");
    Serial.print(my);
    Serial.print("microT, mz:");
    Serial.print(mz);
    Serial.print("microT, rhall:");
    Serial.println(rhall);

    Serial.print("Azimute (°): ");
    Serial.print(azimuth);
    Serial.print(" ; Inclinação (°): ");
    Serial.println(inclination);
    Serial.println("----------------------------------------");
  }
}

#endif
