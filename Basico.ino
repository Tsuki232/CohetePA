// se incluyen las librerias necesarias
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <SPI.h>
#include <SD.h>
// se declara el sensor y las variables necesarias
Adafruit_BMP280 bmp;
float TEMPERATURA;
float PRESION, P0;

File dataFile;
// inicializa el sistema a 9600 baudios
void setup() {
  Serial.begin(9600);
  Serial.println("Iniciando sistema...");

  // Inicializar BMP280
  if (!bmp.begin(0x76)) {
    Serial.println("BMP280 no encontrado!");
    while (1);
  }

  // Importante: forza pin 10 como salida
  pinMode(10, OUTPUT);

  // Inicializar microSD
  if (!SD.begin(10)) {   // Usa el mismo pin de salida de datos para la SD
    Serial.println("Error al inicializar microSD");
  } else {
    Serial.println("microSD lista.");
  }

  // Guardar presión inicial/gerenal de Popayán
  P0 = 1015.8;
}
// inicia el ciclo de toma de datos del sensor
void loop() {
  // Toma los valores desde el sensor BMP280
  TEMPERATURA = bmp.readTemperature();
  PRESION = bmp.readPressure() / 100;
  float ALTITUD = bmp.readAltitude(P0);

  // Estructura del archivo de texto
  String txt = "(";
  txt += "\"temperatura\": "; txt += TEMPERATURA; txt += " C°, ";
  txt += "\"presion\": "; txt += PRESION; txt += " hPa, ";
  txt += "\"altitud\": "; txt += ALTITUD; txt += " mts. ";
  txt += ")";

  // Solo permite leer y abrir archivos de ocho caracteres de nombre y 3 caracteres de extension.
  dataFile = SD.open("pila4321.txt", FILE_WRITE);

  // Si se subieron los datos al txt, aqui se leeran
  if (dataFile) {
    dataFile.println(txt);
    dataFile.close();
    Serial.println("Dato guardado en microSD:");
    Serial.println(txt);
  } else {
    Serial.println("No se pudo abrir el archivo.txt");
  }
  // rango de tiempo entre toma de datos.
  delay(50);
}
