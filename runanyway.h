//
//
// Esta lib adiciona função que confere os horarios de acioamento do pino rele01
//
//

void horaliga(int Hora, int Minuto, unsigned long Interval, int horaAtual, int minutoAtual, int pin) {

  if (Hora == horaAtual && Minuto == minutoAtual)
  {
    Serial.print("\nIrrigando");
    digitalWrite(pin, HIGH);
    delay(Interval);
    digitalWrite(pin, LOW);
     Serial.print("\nEsperando para evitar dupla rega");
    delay(150);// espera um minuto para evitar que regue duas vezes no mesmo minuto
  } 
}

