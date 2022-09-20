#include <Arduino.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <AM2320.h>
#include <SH1106Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#include <secret.h>

#define IP "10.22.5.122:3000"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_ADDRESS 0x3C

#define MOS_1 D8
#define MOS_2 D7
#define MOS_3 D6

#define MEASURE_DELAY 1000
#define API_DELAY 2000
#define SCREEN_DELAY 50

AM2320 sensor_am2320;

long unsigned int lastMeasure = 0;
long unsigned int lastApiCall = 0;
long unsigned int lastScreenUpdate = 0;

int Mos_1_State = 0;
int Mos_2_State = 0;
int Mos_3_State = 0;
int Servo_State = 0;

float currentTemp = 0;
float currentHumidity = 0;

int lastHttpCode = 0;

SH1106Wire display(SCREEN_ADDRESS, D2, D1, GEOMETRY_128_64, I2C_ONE, 100000);

void connectWifi(String ssid, String password)
{ // connects to the wifi
  Serial.println("Connecting to wifi with");
  Serial.printf("SSID: %s", SECRET_WIFI_SSID);
  Serial.printf("PASS: %s\n", SECRET_WIFI_PASSWORD);
  WiFi.begin(SECRET_WIFI_SSID, SECRET_WIFI_PASSWORD);

  Serial.print("Connecting");
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  int tries = 0;

  while (WiFi.status() != WL_CONNECTED)
  {
    tries++;
    char buffer[32];
    display.clear();
    display.drawStringf(64, 32 - 8, buffer, "Connecting %i", tries);
    display.display();
    Serial.print(".");
    delay(250);
  }
}

void setup()
{
  pinMode(MOS_1, OUTPUT);
  pinMode(MOS_2, OUTPUT);
  pinMode(MOS_3, OUTPUT);
  
  Serial.begin(115200);
  while (!Serial)
  {
    delay(10);
  }

  sensor_am2320.begin(D2, D1);

  display.init();
  display.setFont(ArialMT_Plain_16);
  display.flipScreenVertically();
  display.clear();

  connectWifi(SECRET_WIFI_SSID, SECRET_WIFI_PASSWORD);
}
void loop()
{

  if (millis() > (lastMeasure + MEASURE_DELAY))
  {
    lastMeasure = millis();

    if (sensor_am2320.measure())
    {
      currentTemp = sensor_am2320.getTemperature();
      currentHumidity = sensor_am2320.getHumidity();
    }
  }

  if (millis() > (lastApiCall + API_DELAY))
  {
    lastApiCall = millis();

    WiFiClient client;
    HTTPClient http;
    
    if (WiFi.status() != WL_CONNECTED) {
      connectWifi(SECRET_WIFI_SSID, SECRET_WIFI_PASSWORD);
    }
    
    http.begin(client, "http://10.22.5.122:3000/data/settings.json");
    lastHttpCode = http.GET();
    DynamicJsonDocument doc(2048);
    deserializeJson(doc, http.getString());
    serializeJsonPretty(doc, Serial);
    http.end();
    
    http.begin(client, "http://" + String(IP) + "/saveData?hum=" + String(currentHumidity) + "&temp=" + String(currentTemp));
    http.GET();
    http.end();

    
    Mos_1_State = doc["fan"];
    Mos_2_State = doc["water"];
    Mos_3_State = doc["led"];
    Servo_State = doc["roof"];
    analogWrite(MOS_1, Mos_1_State);
    analogWrite(MOS_2, Mos_2_State);
    analogWrite(MOS_3, Mos_3_State);
  }

  if (millis() > (lastScreenUpdate + SCREEN_DELAY))
  {
    lastScreenUpdate = millis();

    float timeSinceMeasure = millis() - lastMeasure;
    float timeSinceApiCall = millis() - lastApiCall;

    display.clear();
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(2, 0, String((MEASURE_DELAY - timeSinceMeasure) / 1000.0));
    display.setTextAlignment(TEXT_ALIGN_RIGHT);
    display.drawString(124, 0, String((API_DELAY - timeSinceApiCall) / 1000.0));
    display.setTextAlignment(TEXT_ALIGN_LEFT);

    display.drawString(1, 16, String(currentHumidity));
    display.drawString(1, 32, String(currentTemp));
    
    display.setTextAlignment(TEXT_ALIGN_RIGHT);
    display.drawString(125, 16, "F" + String(Mos_1_State));
    display.drawString(125, 32, "W" + String(Mos_2_State));
    display.drawString(125, 48, "L" + String(Mos_3_State));

    if (timeSinceMeasure < 200)
    {
      display.drawVerticalLine(0, 0, 16);
    }
    if (timeSinceApiCall < 200)
    {
      display.drawVerticalLine(125, 0, 16);
    }
    
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawString(64, 0, "H" + String(lastHttpCode));
    display.drawString(64, 16, "R" + String(Servo_State));

    display.display();
  }

  delay(10);
}