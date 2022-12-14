/*
  Curso de Microprocessadores

  MCU: DOIT ESP32 DEVKIT V1 , Clock: 80MHz - 240Mhz

  UPE-Poli

  Data: 25.10.2022

------------- Funcionamento do Programa -----------------

O programa trata o sinal de um sensor PIR de movimento pelo pino de entrada 19, de forma que caso haja detecção de presença, o programa aciona um led
através do pino 21 e envia um e-mail para um usuário com um alerta. Caso não haja detecção de movimento, o led permanece apagado e nenhum alerta é enviado.

*/

//Arquivos de referências para as bibliotecas utitlizados no código
#include <EMailSender.h>  //biblioteca utilizada para enviar e-mail
#include <WiFi.h>         //biblioteca utilizada para conectar o microprocessador ao WiFi

/*
--------------------------------------------------------
LED SENSOR - VARIAVEIS E DEFINIÇÕES DE FUNÇÕES - INÍCIO
--------------------------------------------------------
*/
int ledPin = 21;                // escolha de um pino para o LED
int inputPin = 19;              // escolha de um pino de entrada para o sensor 
int pirState = LOW;             // flag de estado do sensor, assumindo que nenhum movimento é deteccado
int sensor = LOW;                    // variável para ler o sinal do inputPin


const char* ssid = "x";             //nome do WiFi
const char* password = "123456789"; //senha do Wifi

int connection_state = 0;        //conectado ao WiFi ou não 
int reconnect_interval = 10000; //se não conectado aguarda um tempo para tentar de novo

bool WiFiConnect(const char* nSSID = nullptr, const char* nPassword = nullptr)
{
    int attempt = 0;
    Serial.print("Conectando a ");
    if(nSSID) {
        WiFi.begin(nSSID, nPassword);
        Serial.println(nSSID);
    }

    int i = 0;
    while(WiFi.status()!= WL_CONNECTED && i++ < 50)
    {
        delay(200);
        Serial.print(".");
    }
    ++attempt;
    Serial.println("");
    if(i == 51) {
        Serial.print("Conexao: TIMEOUT na tentativa: ");
        Serial.println(attempt);
        if(attempt % 2 == 0)
            Serial.println("Cheque se ponto de acesso está disponível\r\n");
        return false;
    }
    Serial.println("Conexao: ESTABELECIDA");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    return true;
} 

void Awaits()
{
    uint32_t ts = millis();
    while(connection_state==0)
    {
        delay(50);
        if(millis() > (ts + reconnect_interval) && !connection_state){
            connection_state = WiFiConnect();
            ts = millis();
        }
    }
}



void setup() {
  pinMode(ledPin, OUTPUT);      // declara LED como output
  pinMode(inputPin, INPUT);     // declara sensor como input

  Serial.begin(9600);           //estabelece comunicação serial com o ESP32 em um baud rate de 9600 bit/s

  connection_state = WiFiConnect(ssid, password);
    if(connection_state==false)  // se não estiver conectado ao Wifi
        Awaits();          // constantemente tenta conectar ao Wifi

    
}

// LOOP RELACIONADO AO LED E SENSOR 
void loop() {
  sensor = digitalRead(inputPin);  // valor de input do sensor
  if (sensor == HIGH) {            // checa se o input está como HIGH
    digitalWrite(ledPin, HIGH);  // Acende o LED
    if (pirState == LOW) {
      // esp32 acabou de ser ligado
      Serial.println("Movimento detectado!");
      EMailSender::EMailMessage message;
      message.subject = "PROJETO MONITORAMENTO DE SEGURANÇA";
      message.message = "SENSOR DETECTOU MOVIMENTO ";
    
      EMailSender emailSend("adsf@poli.br", "ldhkidcjafaddycu");
      EMailSender::Response resp = emailSend.send("professorjenesio@gmail.com", message);

      Serial.println("Sending status: ");

      Serial.println(resp.status);
      Serial.println(resp.code);
      Serial.println(resp.desc);
      // Apenas printa na mudança de estado, não o estado
      pirState = HIGH;
    }
  } else {
    digitalWrite(ledPin, LOW); // Desliga o LED
    if (pirState == HIGH) {
      // we have just turned of
      Serial.println("Movimento terminou!");
      // Apenas printa na mudança de estado, não o estado
      pirState = LOW;
    }
  }
}