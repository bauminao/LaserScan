#include <Servo.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif
U8G2_SSD1306_128X64_NONAME_1_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);

const char* ssid     = "baumhaus";
const char* password = "1234555566667777";
Servo servoblau;
ESP8266WebServer server ( 80 );

void setup()
{
  Serial.begin(9600);
  servoblau.attach(12);
  u8g2.begin();
  u8g2.enableUTF8Print();
  connectWifi();
  beginServer();
  servoblau.write(0);
  u8g2.setFont(u8g2_font_courB18_tf);
  u8g2.setFontDirection(0);
  u8g2.firstPage();
  do {
    u8g2.drawStr(0, 20, "0 Grad");
  } while ( u8g2.nextPage() );
}

void loop() {
  server.handleClient();
  delay(1000);
}

void connectWifi()
{
  WiFi.enableSTA(true);
  delay(2000);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void beginServer()
{
  server.on ( "/", handleRoot );
  server.begin();
  Serial.println ( "HTTP server started" );
}

void handleRoot() {
  if ( server.hasArg("SERVO") ) {
    handleSubmit();
  } else {
    server.send ( 200, "text/html", getPage() );
  }
}

void handleSubmit() {

  String SERVOValue;
  SERVOValue = server.arg("SERVO");
  Serial.println("Set GPIO ");
  Serial.print(SERVOValue);

  if ( SERVOValue == "0" ) {
    servoblau.write(0);
    server.send ( 200, "text/html", getPage() );
    u8g2.setFont(u8g2_font_courB18_tf);
    u8g2.setFontDirection(0);
    u8g2.firstPage();
    do {
      u8g2.drawStr(0, 20, "0 Grad");
    } while ( u8g2.nextPage() );
  }
  else if ( SERVOValue == "90" )
  {
    servoblau.write(90);
    server.send ( 200, "text/html", getPage() );
    u8g2.setFont(u8g2_font_courB18_tf);
    u8g2.setFontDirection(0);
    u8g2.firstPage();
    do {
      u8g2.drawStr(0, 20, "90 Grad");
    } while ( u8g2.nextPage() );
  }
  else if ( SERVOValue == "180" )
  {
    servoblau.write(180);
    server.send ( 200, "text/html", getPage() );
    u8g2.setFont(u8g2_font_courB18_tf);
    u8g2.setFontDirection(0);
    u8g2.firstPage();
    do {
      u8g2.drawStr(0, 20, "180 Grad");
    } while ( u8g2.nextPage() );
  } else
  {
    Serial.println("Error Servo Value");
  }
}

String getPage() {
  String page = "‹html lang=en-EN›‹head›‹meta http-equiv='refresh' content='60'/›";
  page += "‹title›arduino-projekte.info‹/title›";
  page += "‹style› body { background-color: #fffff; font-family: Arial, Helvetica, Sans-Serif; Color: #000000; }";
  page += ".button {display: inline-block; padding: 15px 15px; font-size: 25px; cursor: pointer; text-align: center; text-decoration: none;";
  page += "outline: none; color: #ffffff; background-color: #4db2ec; border: none; border-radius: 15px;}";
  page += ".button:hover {background-color: #4DCAEC; color:#ffffff;}";
  page += ".button:active {background-color: #4DCAEC; box-shadow: 0 3px #666; transform: translateY(3px); }‹/style›";
  page += "‹/head›‹body›‹h1›ESP8266 WebServer‹/h1›";
  page += "‹h3>Servo Test‹/h3›";
  page += "‹form action='/' method='POST'›";
  page += "‹INPUT class='button' type='submit' name='SERVO' value='0'›    ";
  page += "‹INPUT class='button' type='submit' name='SERVO' value='90'›    ";
  page += "‹INPUT class='button' type='submit' name='SERVO' value='180'›";

  page += "‹/body›";
  return page;
}
