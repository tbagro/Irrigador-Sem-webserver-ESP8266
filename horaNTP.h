//
//
// Esta lib realiza a consulta da hora via internet atraves do protocolo NTP
//
#include <TimeLib.h>      //https://github.com/PaulStoffregen/Time
#include <WiFiUdp.h>

static const char ntpServerName[] = "a.ntp.br"; //Servidor (pode ser a.ntp.br / b.ntp.br / c.ntp.br )
const int timeZone = -3; // Fuso horario (-4 Padrão / -3 Horário de Verão)

WiFiUDP Udp;
unsigned int localPort = 8888;
time_t getNtpTime();
void sendNTPpacket(IPAddress &address);
const int NTP_PACKET_SIZE = 48;
byte packetBuffer[NTP_PACKET_SIZE];

time_t getNtpTime()
{
  IPAddress ntpServerIP;
  while (Udp.parsePacket() > 0) ;
  Serial.println(F("Transmitindo NTP Request"));
  WiFi.hostByName(ntpServerName, ntpServerIP);
  Serial.print(ntpServerName);
  Serial.print(": ");
  Serial.println(ntpServerIP);
  sendNTPpacket(ntpServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 3000) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Serial.println(F("Resposta recebida do NTP"));
      Udp.read(packetBuffer, NTP_PACKET_SIZE);
      unsigned long secsSince1900;
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }
  Serial.println("Sem resposta do NTP");
  return 0;
}

void sendNTPpacket(IPAddress &address)
{
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  packetBuffer[0] = 0b11100011;
  packetBuffer[1] = 0;
  packetBuffer[2] = 6;
  packetBuffer[3] = 0xEC;
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  Udp.beginPacket(address, 123);
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}

time_t prevDisplay = 0;

// recupera valor da hora e transforma em um valor inteiro
unsigned int NowHora() {
  unsigned int hora = hour();

  return hora;
}
// recupera valor do minuto e transforma em um valor inteiro
unsigned int NowMin() {
  unsigned int minuto = minute();

  return minuto;
}

// recupera valor da hora e minuto e transforma em uma string
String Hora () {//faz a leitura do RTC
  char hora[30];
  sprintf( hora, "%02d:%02d", hour(), minute()); //
  //Serial.println (hora);
  return hora;
}


//******************** Verificação das horas *******************//



void setupNtp() { /// colocar no setup depois do wifi logon 
  Serial.println(F("Iniciando UDP"));
  Udp.begin(localPort);
  Serial.print(F("Porta local: "));
  Serial.println(Udp.localPort());
  Serial.println(F("Aguardando sincronia do NTP"));
  setSyncProvider(getNtpTime);
  setSyncInterval(300);
}

