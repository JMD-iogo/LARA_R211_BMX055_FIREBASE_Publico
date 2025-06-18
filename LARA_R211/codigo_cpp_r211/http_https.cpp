#include "../codigo_h_r211/http_https.h"


void initHTTPS() {

  // sendATCommand("AT+USECMNG=3");  // Verifica os certificados armazenados, podemos carregar usando certificado.h
  // delay(500);
 
  sendATCommand("AT+USECPRF=2,0,1");  // Configura a validação do perfil (perfil 2)
  //associa o certificado ao perfil 2
  delay(500);
  sendATCommand("AT+USECPRF=2,3,\"root_ca_google\"");
  delay(500);
  
  //Configurar HTTPS com o Firebase (URL do Firebase)
 // URL do Firebase em formato HTTPS, cuidado!! o proprio modulo adiciona internamente o https:// , pelo que nao colocar isso em baixo, origina erros

 sendATCommand(("AT+UHTTP=0,1,\"" + firebaseHost + "\"").c_str());
  //sendATCommand("AT+UHTTP=0,1,\"link_firebase\"");
  delay(500);
  //porta https
  sendATCommand("AT+UHTTP=0,5,443");
  delay(500);
  
  sendATCommand("AT+UHTTP=0,6,1,2");
  //pag 740 diz como mandar
  delay(500);
}


void testHTTP() {

  delay(200);
  sendATCommand("AT+UHTTP=0,1,\"httpbin.org\"");
  sendATCommand("AT+UHTTP=0,5,80");
  sendATCommand("AT+UHTTPC=0,1,\"/get\",\"UHTTP.TXT\"");
  delay(200);
  sendATCommand("AT+ULSTFILE=");
  sendATCommand("AT+URDFILE=\"UHTTP.TXT\"");

  delay(1000);
  sendATCommand("AT+UDELFILE=\"UHTTP.TXT\"");  // Apaga o arquivo
  delay(1000);
}



void testHTTPS() {
  //passo um, mandar certificado pela funcao do certificado.h
  // Passo 2 - Verificar certificados armazenados (opcional)
  sendATCommand("AT+USECMNG=3");

  // Passo 3 - Setar validação do profile 2
  sendATCommand("AT+USECPRF=2,0,1");
  //colar isto no git bash: e copiar o ultimo certificado para o modulo usando o certificado.h
  //openssl s_client -connect www.google.com:443 -showcerts

  // Passo 4 - Associar certificado "google_root.der" ao profile 2
  sendATCommand("AT+USECPRF=2,3,\"root_ca_google\"");

  // Passo 5 - Configurar HTTPS com profile 2
  delay(2000);
  sendATCommand("AT+UHTTP=0,1,\"www.google.com\"");
  delay(2000);
  sendATCommand("AT+UHTTP=0,5,443");
  delay(2000);
  sendATCommand("AT+UHTTP=0,6,1,2");
  delay(2000);

  sendATCommand("AT+UHTTPC=0,1,\"/\",\"resp3.txt\"");  // GET + grava
  delay(2000);
  sendATCommand("AT+ULSTFILE=");
  delay(2000);
  sendATCommand("AT+URDFILE=\"resp3.txt\"");
  delay(2000);
  sendATCommand("AT+UDELFILE=\"resp3.txt\"");  // Apaga o arquivo "resp3.txt" para nao encher memoria no debug
  delay(1000);
}

//no futuro pode nao funcionar por conta das permissoes de leitura, no mento do teste qualquer pessoa podia ler e escrever no firebase
//cria um ramo novo no firebase usando uma consola, para debug
//curl --ssl-no-revoke -X PUT -H "Content-Type: application/json" -d "{\"nome\":\"João\",\"idade\":25}" "link_firebase"
void lerHTTPSFirebase_test() {

  // Passo 1 - Configurar perfil para usar um certificado da Google CA
  sendATCommand("AT+USECMNG=3");  // Verifica os certificados armazenados (opcional)
  delay(1000);
  // Passo 2 - Setar validação do perfil 2
  sendATCommand("AT+USECPRF=2,0,1");  // Configura a validação do perfil (perfil 2)
  // Passo 3 - Associar o certificado da autoridade certificadora do Firebase (Google Trust Services)
  // (O nome exato do certificado pode variar. Para a Google, geralmente é "GoogleTrustServices")
  delay(1000);
  sendATCommand("AT+USECPRF=2,3,\"root_ca_google\"");
  delay(1000);
  // Passo 4 - Configurar HTTPS com o Firebase (substituindo pelo URL do Firebase)
  sendATCommand(("AT+UHTTP=0,1,\"" + firebaseHost + "\"").c_str());
 // sendATCommand("AT+UHTTP=0,1,\"link_firebase\"");
  delay(1000);
  sendATCommand("AT+UHTTP=0,5,443");
  delay(1000);

  sendATCommand("AT+UHTTP=0,6,1,2");
  //pag 740 diz como mandar
  delay(1000);

  sendATCommand("AT+UHTTPC=0,1,\"/usuarios1.json\",\"resposta.txt\"");

  delay(1000);
  sendATCommand("AT+ULSTFILE=");
  sendATCommand("AT+URDFILE=\"resposta.txt\"");  // Le o arquivo
  //+UUHTTPCR: <profile_id>,<http_command>,<http_result> pag 740
  delay(1000);
  sendATCommand("AT+UDELFILE=\"resposta.txt\"");  // Apaga o arquivo
  sendATCommand("AT+UDELFILE=\"data.json\"");     // Apaga o arquivo
  delay(1000);
}

//cria um ramo novo no firebase usando uma consola, para debug
//curl --ssl-no-revoke -X PUT -H "Content-Type: application/json" -d "{\"nome\":\"João\",\"idade\":25}" "link_firebase"
void escreverHTTPSFirebase_test() {
  // sendATCommand("AT+UDWNFILE=\"data.json\",26"); // 28 é o número de bytes do JSON, se usar o prinln (conta o \r\n); se usar print é 26
  // delay(100);
  // // Agora envie os dados pelo serial
  // Serial2.print("{\"nome\":\"Joao\",\"idade\":25,\"timestamp\": { \".sv\": \"timestamp\" }}");

  const char* enviar_json = "{\"nome\":\"Joao\",\"idade\":25,\"timestamp\": { \".sv\": \"timestamp\" }}";
  int jsonSize = strlen(enviar_json);

  Serial2.print("AT+UDWNFILE=\"data.json\",");
  Serial2.println(jsonSize);
  delay(500);
  Serial2.print(enviar_json);
  delay(5000);


  delay(2000);
  Serial2.println("");
  delay(100);
  sendATCommand("AT+URDFILE=\"data.json\"");  // Le o arquivo
  delay(2000);


  // Passo 1 - Configurar perfil para usar um certificado da Google CA
  sendATCommand("AT+USECMNG=3");  // Verifica os certificados armazenados (opcional)
  delay(1000);
  // Passo 2 - Setar validação do perfil 2
  sendATCommand("AT+USECPRF=2,0,1");  // Configura a validação do perfil (perfil 2)
  // Passo 3 - Associar o certificado da autoridade certificadora do Firebase (Google Trust Services)
  // (O nome exato do certificado pode variar. Para a Google, geralmente é "GoogleTrustServices")
  delay(1000);
  sendATCommand("AT+USECPRF=2,3,\"root_ca_google\"");
  delay(1000);
  // Passo 4 - Configurar HTTPS com o Firebase (substituindo pelo URL do Firebase)
  // URL do Firebase em formato HTTPS: https://<projeto>.firebaseio.com
  sendATCommand(("AT+UHTTP=0,1,\"" + firebaseHost + "\"").c_str());
  //sendATCommand("AT+UHTTP=0,1,\"link_firebase\"");
  delay(1000);
  sendATCommand("AT+UHTTP=0,5,443");
  delay(1000);

  sendATCommand("AT+UHTTP=0,6,1,2");
  //pag 740 diz como mandar
  delay(1000);


  //sendATCommand("AT+UHTTPC=0,3,\"/usuarios.json\",\"response.txt\",\"data.json\",4"); // PUT nao nos da um id e se nao existir o ramo ele cria, podemos ser nos a criar o nosso id
  sendATCommand("AT+UHTTPC=0,4,\"/usuarios.json\",\"response.txt\",\"data.json\",4");  // POST ( adicionar um item com um ID automático dentro de um caminho) e se nao existir o ramo ele cria

  delay(1000);
  sendATCommand("AT+ULSTFILE=");
  sendATCommand("AT+URDFILE=\"response.txt\"");  // Le o arquivo
  //+UUHTTPCR: <profile_id>,<http_command>,<http_result> pag 740
  delay(1000);
  sendATCommand("AT+UDELFILE=\"response.txt\"");  // Apaga o arquivo
  sendATCommand("AT+UDELFILE=\"data.json\"");     // Apaga o arquivo
  delay(1000);
}

//cria um ramo novo no firebase usando uma consola, para debug
//curl --ssl-no-revoke -X PUT -H "Content-Type: application/json" -d "{\"nome\":\"João\",\"idade\":25}" "link_firebase"
void mandarHTTPSFirebase() {
  //para ja as horas ficam com as da database, mais tarde talvez usar as horas do lara


  // sendATCommand("AT+ULSTFILE=");  //debug para ver se realmente apagou tudo
  // delay(1000);



  //desta forma demora o menor tempo possivel, nao temos é feed back
  //sendATCommand("AT+UHTTPC=0,3,\"/usuarios.json\",\"response.txt\",\"data.json\",4"); // PUT nao nos da um id e se nao existir o ramo ele cria, podemos ser nos a criar o nosso id

  String expectedOK_enviar = "OK";
  String expectedERR_enviar = "ERROR";                                                                                                                                                      // pode ajustar se for "ERRO" no seu módulo
  sendATCommand_delay_automatico("AT+UHTTPC=0,4,\"/sensores/acelerometro.json\",\"response_HTTPS_acelerometro.txt\",\"data_acelerometro.json\",4", expectedOK_enviar, expectedERR_enviar);  // POST ( adicionar um item com um ID automático dentro de um caminho) e se nao existir o ramo ele cria
  delay(2000);
  sendATCommand_delay_automatico("AT+UHTTPC=0,4,\"/sensores/giroscopio.json\",\"response_HTTPS_giroscopio.txt\",\"data_giroscopio.json\",4", expectedOK_enviar, expectedERR_enviar);  // POST ( adicionar um item com um ID automático dentro de um caminho) e se nao existir o ramo ele cria
  delay(2000);
  sendATCommand_delay_automatico("AT+UHTTPC=0,4,\"/sensores/magnetometro.json\",\"response_HTTPS_magnetometro.txt\",\"data_magnetometro.json\",4", expectedOK_enviar, expectedERR_enviar);  // POST ( adicionar um item com um ID automático dentro de um caminho) e se nao existir o ramo ele cria
  delay(2000);

  String expectedOK_resposta = "200 OK";
  String expectedERR_resposta = "400 Bad Request";                  // pode ajustar se for "ERRO" no seu módulo
  sendATCommand("AT+URDFILE=\"response_HTTPS_acelerometro.txt\"");  // Le o arquivo
  //+UUHTTPCR: <profile_id>,<http_command>,<http_result> pag 740
  delay(100);
  sendATCommand("AT+URDFILE=\"response_HTTPS_giroscopio.txt\"");  // Le o arquivo
  //+UUHTTPCR: <profile_id>,<http_command>,<http_result> pag 740
  delay(100);
  sendATCommand("AT+URDFILE=\"response_HTTPS_magnetometro.txt\"");  // Le o arquivo
  //+UUHTTPCR: <profile_id>,<http_command>,<http_result> pag 740
  delay(100);
}