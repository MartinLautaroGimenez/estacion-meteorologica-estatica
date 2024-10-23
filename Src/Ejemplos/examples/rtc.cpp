#include <Arduino.h>
#include <Wire.h>
#include <RTClib.h>

RTC_DS3231 rtc;

void setup() {
  Serial.begin(115200);
  
  // Inicia el módulo RTC
  if (!rtc.begin()) {
    Serial.println("No se encuentra el RTC.");
    while (1);
  }

  // Si el RTC ha perdido energía, establecer la fecha/hora a la compilación
  if (rtc.lostPower()) {
    Serial.println("RTC perdió energía, configurando la fecha y hora...");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void loop() {
  DateTime now = rtc.now();

  // Imprime la fecha y hora en el formato YYYY-MM-DD HH:MM:SS
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  delay(1000); // Espera 1 segundo antes de actualizar la hora
}
