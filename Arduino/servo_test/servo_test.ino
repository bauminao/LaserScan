#include <Servo.h>


Servo servoblau;

void setup()
{
  Serial.begin(9600);
  servoblau.attach(12);
  servoblau.write(0);
}

void loop() {
  servoblau.write(0);
  delay(1000);
  servoblau.write(22);
  delay(1000);
  servoblau.write(45);
  delay(1000);
  servoblau.write(68);
  delay(1000);
  servoblau.write(90);
  delay(1000);
}

