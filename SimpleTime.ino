#include <WiFi.h>
#include <WebServer.h>

#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>
#include <Wire.h>

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0);

/*Put your SSID & Password*/
const char* ssid = "papa";  // Enter SSID here
const char* password = "12345678";  //Enter Password here

WebServer server(80);

uint8_t LED1pin = 2;
bool LED1status = LOW;

void setup() {
  u8g2.begin();
  Serial.begin(115200);
  delay(100);
  pinMode(LED1pin, OUTPUT);

  write_led("Connecting");
  Serial.println("Connecting to ");
  Serial.println(ssid);

  //connect to your local wi-fi network
  WiFi.begin(ssid, password);

  //check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());
  char ip[WiFi.localIP().toString().length() + 1];
  WiFi.localIP().toString().toCharArray(ip, WiFi.localIP().toString().length() + 1);
  write_led(ip);

  server.on("/", handle_OnConnect);
  server.on("/on", handle_led1on);
  server.on("/off", handle_led1off);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
}
void loop() {
  server.handleClient();
  if(LED1status){
    digitalWrite(LED1pin, HIGH);
  }else {
    digitalWrite(LED1pin, LOW);
  }
}

void write_led(char txt[]) {
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_logisoso16_tr);  // choose a suitable font at https://github.com/olikraus/u8g2/wiki/fntlistall
  u8g2.drawStr(8, 25, txt); // write something to the internal memory
  u8g2.sendBuffer();
}

void handle_OnConnect() {
  LED1status = LOW;
  Serial.println("led OFF");
}

void handle_led1on() {
  LED1status = HIGH;
  Serial.println("GPIO4 Status: ON");
  server.send(200, "text/plain", "OK");
}

void handle_led1off() {
  LED1status = LOW;
  Serial.println("GPIO4 Status: OFF");
  server.send(200, "text/plain", "OK");
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}
