//
// CONTROLADOR DE RELE COM RELOGIO NTP VIA WIFI
// Versao  para 1 rele e nenhum sensor de umidade
//
// Tiago Alexandre Batista 2018
//

//


#include <ESP8266WiFi.h>

#include "runanyway.h" //contra o acionamento dos pinos de acordo com a hora
#include "horaNTP.h" // consulta hora NTP via internet


char ssid[] = "Tamanduai";  //  your network SSID (name)
char pass[] = "semsenha";       // your network password


// matrizes de horarios e tempos de rega (acionamentos do rele) no array de rega
#define HORA 0
#define MINUTO 1
#define TEMPOMS 2

// PROGRAMAR OS HORARIOS E TEMPOS DE REGA AQUI
//              {hora, min, tempo em milissegundos}

int rega_01[] = {7,   00,  120000};
int rega_02[] = {12,   00,  120000};
int rega_03[] = {22,   43,  120000};


int rele01 = D1; // pino do rele


// S E T U P
//
void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println();

  // Configura os pinos de saida do rele e buzzer
  pinMode(rele01, OUTPUT);
  digitalWrite(rele01, LOW); // desliga o rele

  // We start by connecting to a WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  setupNtp();//verifica hora da internet

}

// L O O P
//
void loop()
{

  horaliga(rega_01[HORA], rega_01[MINUTO], rega_01[TEMPOMS], NowHora(),   NowMin(), rele01);
  horaliga(rega_02[HORA], rega_02[MINUTO], rega_02[TEMPOMS], NowHora(),   NowMin(), rele01);
  horaliga(rega_03[HORA], rega_03[MINUTO], rega_03[TEMPOMS], NowHora(),   NowMin(), rele01);
  Serial.print("hora atual  ");
  Serial.println(Hora());

  // wait ten seconds before asking for the time again
  delay(1000);
}









