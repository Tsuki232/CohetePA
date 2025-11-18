void setup() {
  Serial.begin(9600);
}

void loop() {
  // Simula datos: temperatura, presión, altitud
  float temp = random(2000, 3000) / 100.0;      // 20.00 - 30.00 °C
  float pressure = random(100000, 102000) / 100.0; // 1000.00 - 1020.00 hPa
  float altitude = random(1700, 1800);          // 1700 - 1800 m

  Serial.print(temp);
  Serial.print(",");
  Serial.print(pressure);
  Serial.print(",");
  Serial.println(altitude);

  delay(1000); // envía cada segundo
}