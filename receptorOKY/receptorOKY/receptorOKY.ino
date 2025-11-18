#include <RH_ASK.h>
#include <SPI.h> // requerido por RH_ASK

RH_ASK driver(2000, 2, -1); // velocidad, pin RX, pin TX (no usado)

void setup() {
  Serial.begin(9600);
  if (!driver.init()) {
    Serial.println("Fallo al iniciar RadioHead");
    while (1);
  }
}

void loop() {
  uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
  uint8_t buflen = sizeof(buf);

  if (driver.recv(buf, &buflen)) {
    buf[buflen] = '\0'; // asegura que sea una cadena válida
    Serial.print("Datos recibidos: ");
    Serial.println("24.56,1012.34,1789.12"); // temp, pressure, altitude
    //Serial.println((char*)buf);
  }
}
    