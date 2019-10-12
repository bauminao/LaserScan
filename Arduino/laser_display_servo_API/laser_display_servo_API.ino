#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <VL53L0X.h>

#include <Servo.h>

#include <ESP8266WiFi.h>

#include "secret_keys.h"

WiFiServer server(80);

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)

#define HIGH_ACCURACY
#define LONG_RANGE

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
VL53L0X sensor;
Servo servo_H;

int  distance[360];
char buffer[20];
int  _dist;

void setup() {
  Wire.begin();
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  print2Display(" 5 ");
  delay (1500);
  print2Display(" 4 ");
  delay (1500);
  print2Display(" 3 ");
  delay (1500);
  print2Display(" 2 ");
  delay (1500);
  print2Display(" 1 ");
  delay (1500);
  print2Display(" Starting ");
  Serial.println("Starting ESP");

  Serial.println();
  Serial.println();

  Serial.println("Starting Wifi");
  WiFi.begin(ssid,password);
  Serial.println("Starting Server");
  server.begin();
  Serial.println(WiFi.localIP());   

  servo_H.attach(12);
  servo_H.write(90);

  delay (1000);

  display.display();
  display.clearDisplay();
  display.setRotation(2);

  sensor.init();
  sensor.setTimeout(500);
  #if defined LONG_RANGE
  // lower the return signal rate limit (default is 0.25 MCPS)
  sensor.setSignalRateLimit(0.1);
  // increase laser pulse periods (defaults are 14 and 10 PCLKs)
  sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
  sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
  #endif

  #if defined HIGH_SPEED
  // reduce timing budget to 20 ms (default is about 33 ms)
    sensor.setMeasurementTimingBudget(20000);
  #elif defined HIGH_ACCURACY
  // increase timing budget to 200 ms
    sensor.setMeasurementTimingBudget(200000);
  #endif
}

void print2Display(String _text) {
  display.clearDisplay();

  display.setTextSize(2);      // 1,2,3
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  display.print(_text);

  display.display();
}

int measureDistance() {
  int _dist = 0;
  _dist = sensor.readRangeSingleMillimeters();
  //Serial.print(_dist);
  if (sensor.timeoutOccurred()) { Serial.print(" TIMEOUT"); }
  //Serial.println();
  return _dist;

}


void loop() {
  int _measure_delay = 20;   // delay after measurement
  int _stepsize = 5;         // Angle stepsize 
  int minangle  = 10;
  int maxangle  = 170;

  for (int angle = minangle ; angle <= maxangle ; angle = angle + _stepsize)
  {
    servo_H.write(angle);
    _dist = measureDistance();
    itoa (_dist , buffer , 10);
    print2Display(buffer);
    delay(_measure_delay);
    distance[angle] = _dist;
  }
  for (int angle = maxangle ; angle >= minangle ; angle = angle - _stepsize)
  {
    servo_H.write(angle);
    _dist = measureDistance();
    itoa (_dist , buffer , 10);
    print2Display(buffer);
    delay(_measure_delay);
    distance[angle + 180] = _dist;
  }
  WiFiClient client = server.available();
   while (client.connected() )
    {
        if (client.available() )
            {
              client.println("<!DOCTYPE HTML>");
              client.println("<html>");
              client.println("<body>");
              client.println("<h1>Test Ausgabe</h1>");
              client.println("</body>");
              client.println("</html>");
            }
        client.stop();
    }
}
