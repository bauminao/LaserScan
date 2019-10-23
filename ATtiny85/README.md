Notwendige Teile:
* USB-I2C Device
  - modprobe i2c-dev
  - digispark, ATtiny85: I2C-Tiny-USB
  - https://github.com/harbaum/I2C-Tiny-USB

* ATtiny85 Programmierkrams
  - Arduino Nano als Programmiergeraet.

* Die "Ziel-Platine" auf dem der ATTiny85 arbeitet.
  - I2C Bus für Sensoren
    - ToF Sensor
    - Display
    - Ultrasschall Sensor
    - Kompass / Gyroskop / Magnetometer
  - I2C Bus Richtugn Controler
  - PWM für horizontal-Servo
  - [PWM für vertikal-Servo], wird aber RST überschreiben.
  - Power 


