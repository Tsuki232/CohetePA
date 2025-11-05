#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <SPI.h>
#include <SD.h>

Adafruit_BMP280 bmp;
float TEMPERATURA;
float PRESION, P0;

File dataFile;

void setup() {
  Serial.begin(9600);
  Serial.println("Iniciando sistema...");

  // Inicializar BMP280
  if (!bmp.begin(0x76)) {
    Serial.println("BMP280 no encontrado!");
    while (1);
  }

  // Importante: forzar pin 10 como salida
  pinMode(10, OUTPUT);

  // Inicializar microSD
  if (!SD.begin(10)) {   // Usa el mismo pin CS que te funcionó en CardInfo
    Serial.println("Error al inicializar microSD");
  } else {
    Serial.println("microSD lista.");
  }

  // Guardar presión inicial
  P0 = 1015.0;
}

void loop() {
  TEMPERATURA = bmp.readTemperature();
  PRESION = bmp.readPressure() / 100;
  float ALTITUD = bmp.readAltitude(P0);

  String txt = "(";
  txt += "\"temperatura\": "; txt += TEMPERATURA; txt += " C°, ";
  txt += "\"presion\": "; txt += PRESION; txt += " hPa, ";
  txt += "\"altitud\": "; txt += ALTITUD; txt += " mts. ";
  txt += ")";

  // Solo permite leer y abrir archivos de ocho caracteres de nombre y 3 caracteres de extension.
  dataFile = SD.open("pila1234.txt", FILE_WRITE);

  if (dataFile) {
    dataFile.println(txt);
    dataFile.close();
    Serial.println("Dato guardado en microSD:");
    Serial.println(txt);
  } else {
    Serial.println("No se pudo abrir el archivo.txt");
  }

  delay(2000);
}
