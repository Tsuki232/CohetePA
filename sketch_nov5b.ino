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
float ALTITUD_MAXIMA = 0;
bool primeraLectura = true;

File dataFile;
String nombreArchivo;

void generarNombreArchivo() {
  int contador = 1;
  while (true) {
    nombreArchivo = "prueba";
    if (contador < 99) nombreArchivo += "0";
    nombreArchivo += String(contador);
    nombreArchivo += ".txt";

    if (!SD.exists(nombreArchivo)) {
      break;
    }
    contador++;
  }
}

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
    generarNombreArchivo();  // Generar nombre único al iniciar
    Serial.print("Archivo de sesión: ");
    Serial.println(nombreArchivo);
  }

  P0 = 1015.0;  // Presión base estándar
}

void loop() {
  TEMPERATURA = bmp.readTemperature();
  PRESION = bmp.readPressure() / 100;
  float ALTITUD = bmp.readAltitude(P0);

  if (primeraLectura) {
    ALTITUD_INICIAL = ALTITUD;
    ALTITUD_MAXIMA = ALTITUD;
    primeraLectura = false;
  }

  if (ALTITUD > ALTITUD_MAXIMA) {
    ALTITUD_MAXIMA = ALTITUD;
  }

  float ALTITUD_POPAYAN = 1760.0;
  float DIF_RELATIVA = ALTITUD - ALTITUD_POPAYAN;
  float DIF_INICIAL = fmax(0.0, ALTITUD - ALTITUD_INICIAL);
  float DIF_MAXIMA = ALTITUD_MAXIMA - ALTITUD_INICIAL;

  String json = "{";
  json += "\"temperatura\": " + String(TEMPERATURA, 2) + ", ";
  json += "\"presion\": " + String(PRESION, 2) + ", ";
  json += "\"altitud_actual\": " + String(ALTITUD, 2) + ", ";
  json += "\"altitud_inicial\": " + String(ALTITUD_INICIAL, 2) + ", ";
  json += "\"altitud_maxima\": " + String(ALTITUD_MAXIMA, 2) + ", ";
  json += "\"variacion_maxima\": " + String(DIF_MAXIMA, 2) + ", ";
  json += "\"diferencia_popayan\": " + String(DIF_RELATIVA, 2) + ", ";
  json += "\"desde_encendido\": " + String(DIF_INICIAL, 2);
  json += "}";

  dataFile = SD.open(nombreArchivo, FILE_WRITE);

  if (dataFile) {
    dataFile.println(json);
    dataFile.close();
    Serial.println("Dato guardado en microSD:");
    Serial.println(json);
  } else {
    Serial.println("No se pudo abrir el archivo.");
  }

  delay(100);
}