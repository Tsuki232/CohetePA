#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <SPI.h>
#include <SD.h>
#include <math.h>

Adafruit_BMP280 bmp;
float TEMPERATURA;
float PRESION, P0;
float ALTITUD_INICIAL = 0;
bool primeraLectura = true;

File dataFile;

void setup() {
  Serial.begin(9600);
  Serial.println("Iniciando sistema...");

  if (!bmp.begin(0x76)) {
    Serial.println("BMP280 no encontrado!");
    while (1);
  }


  if (!SD.begin(10)) {
    Serial.println("Error al inicializar microSD");
  } else {
    Serial.println("microSD lista.");
  }

  // Presión base estándar
  P0 = 1015.0;
}

void loop() {
  TEMPERATURA = bmp.readTemperature();
  PRESION = bmp.readPressure() / 100;
  float ALTITUD = bmp.readAltitude(P0);

  // Guardar altitud inicial en la primera lectura
  if (primeraLectura) {
    ALTITUD_INICIAL = ALTITUD;
    primeraLectura = false;
  }

  // Altitud relativa a Popayán
  float ALTITUD_POPAYAN = 1760.0;
  float DIF_RELATIVA = ALTITUD - ALTITUD_POPAYAN;

  // Altitud desde que se encendió el Arduino
  float DIF_INICIAL = fmax(0.0, ALTITUD - ALTITUD_INICIAL);

  String txt = "(";
  txt += "\"temperatura\": "; txt += TEMPERATURA; txt += " C°, ";
  txt += "\"presion\": "; txt += PRESION; txt += " hPa, ";
  txt += "\"altitud_actual\": "; txt += ALTITUD; txt += " mts, ";
  txt += "\"diferencia_popayan\": "; txt += DIF_RELATIVA; txt += " mts, ";
  txt += "\"desde_encendido\": "; txt += DIF_INICIAL; txt += " mts ";
  txt += ")";

  dataFile = SD.open("tierraAL.txt", FILE_WRITE);

  if (dataFile) {
    dataFile.println(txt);
    dataFile.close();
    Serial.println("Dato guardado en microSD:");
    Serial.println(txt);
  } else {
    Serial.println("No se pudo abrir el archivo.txt");
  }

  delay(100);
}