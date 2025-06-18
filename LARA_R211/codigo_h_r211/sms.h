#ifndef SMS
#define SMS


void sendSMS(String phoneNumber, String message);
String readSMS();                            //vai ler o sms todo (incluindo confirmacao do modulo) e retorna so o sms recebido, ele extrai isto usando a funcao String extractMessageBody(String response)
String extractMessageBody(String response);  //extrai o corpo do sms recebido da mensagem completa recebida pelo modulo
String extractMessageNumber(String response, String prefixo_numero);


#endif