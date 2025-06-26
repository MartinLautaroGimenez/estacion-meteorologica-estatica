#include "../lib/config.h"
#ifdef USE_PANTALLA
#include "../lib/pantallacontroller.h"

PantallaController::PantallaController(uint8_t direccion)
  : lcd(direccion, 16, 2) {}

void PantallaController::begin() {
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Estacion ETEC");
  lcd.setCursor(0, 1);
  lcd.print("Iniciando...");
}

void PantallaController::mostrarInicio() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Bienvenido");
  lcd.setCursor(0, 1);
  lcd.print("EMESJ - 2025");
}

void PantallaController::mostrarLecturas(float temp, float hum, float pres) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temp, 1);
  lcd.print("C H:");
  lcd.print(hum, 0);
  lcd.print("%");

  lcd.setCursor(0, 1);
  lcd.print("P:");
  lcd.print(pres, 0);
  lcd.print("hPa");
}

void PantallaController::mostrarEstadoWiFi(const String& ssid, const String& ip) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi: ");
  lcd.print(ssid);

  lcd.setCursor(0, 1);
  lcd.print("IP: ");
  lcd.print(ip);
}

void PantallaController::mostrarError(const String& mensaje) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Error:");
  lcd.setCursor(0, 1);
  lcd.print(mensaje.substring(0, 16)); // Solo 16 caracteres
}

void PantallaController::mostrarPantallaCiclo(uint8_t indice, const LeerSensores& s) {
  lcd.clear();

  switch (indice) {
    case 0:
      lcd.setCursor(0, 0);
      lcd.print("Temp:");
      lcd.print(s.tempDHT, 1);
      lcd.print("C");

      lcd.setCursor(0, 1);
      lcd.print("Hum:");
      lcd.print(s.humDHT, 0);
      lcd.print("%");
      break;

    case 1:
      lcd.setCursor(0, 0);
      lcd.print("Presion:");
      lcd.print(s.presBMP, 0);
      lcd.print("hPa");

      lcd.setCursor(0, 1);
      lcd.print("Alt:");
      lcd.print(s.altBMP, 0);
      lcd.print("m");
      break;

    case 2:
      lcd.setCursor(0, 0);
      lcd.print("Luz:");
      lcd.print(s.bh1750, 0);
      lcd.print(" lux");

      lcd.setCursor(0, 1);
      lcd.print("CO2:");
      lcd.print(s.ppmCO2, 0);
      lcd.print("ppm");
      break;

    case 3:
      lcd.setCursor(0, 0);
      lcd.print("Viento:");
      lcd.print(s.vientoVel, 1);
      lcd.print("m/s");

      lcd.setCursor(0, 1);
      lcd.print("Dir:");
      lcd.print(s.vientoDir);
      lcd.print("deg");
      break;

    case 4:
      lcd.setCursor(0, 0);
      lcd.print("Pres.NM:");
      lcd.print(s.presNMBMP, 0);

      lcd.setCursor(0, 1);
      lcd.print("Sens.T:");
      lcd.print(s.sensDHT, 1);
      break;

    default:
      lcd.setCursor(0, 0);
      lcd.print("Estacion EME");
      lcd.setCursor(0, 1);
      lcd.print("Sensores OK");
      break;
  }
}
#endif
