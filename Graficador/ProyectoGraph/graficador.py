import serial
import matplotlib.pyplot as plt
from collections import deque

# Configura el puerto serial (ajusta COMx según tu sistema)
ser = serial.Serial('COM7', 9600)  # Reemplaza 'COM3' por el puerto correcto
ser.flush()

# Buffers para graficar
max_points = 100
temps = deque(maxlen=max_points)
pressures = deque(maxlen=max_points)
altitudes = deque(maxlen=max_points)

plt.ion()
fig, (ax1, ax2, ax3) = plt.subplots(3, 1)

while True:
    if ser.in_waiting > 0:
        line = ser.readline().decode('utf-8').strip()
        try:
            temp, pressure, altitude = map(float, line.split(','))
            temps.append(temp)
            pressures.append(pressure)
            altitudes.append(altitude)

            ax1.clear()
            ax2.clear()
            ax3.clear()

            ax1.plot(temps, label='Temperatura (°C)', color='red')
            ax2.plot(pressures, label='Presión (hPa)', color='blue')
            ax3.plot(altitudes, label='Altitud (m)', color='green')

            for ax in [ax1, ax2, ax3]:
                ax.legend()
                ax.grid(True)

            plt.pause(0.01)
        except ValueError:
            print("Dato malformado:", line)