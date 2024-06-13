#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
float value;
#define BLYNK_TEMPLATE_ID "TMPL35BFD6QJN"
#define BLYNK_TEMPLATE_NAME "UroCheck"
#define BLYNK_AUTH_TOKEN "QnWtp0g85-W9AtYOxTYBu0sxvLQiIRw3"
char Moisture;
int percentage;
#define BLYNK_PRINT Serial
#include <esp_wifi.h>
#include <BlynkSimpleEsp32.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#define BOTtoken "7312987475:AAFA5Ir-YGD_ubyw0FiG0VD5-5V9Z1zS84Q"
#define CHAT_ID "7210025057"
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);
char auth[]=BLYNK_AUTH_TOKEN;
char ssid[]="TIS_Storeroom";
char pass[]="Kuchnahichalta@10-10";
BlynkTimer timer;

#define MOISTURE_PIN 32

void sendSensor(){
  int Moisture=analogRead(MOISTURE_PIN);
  percentage=map(Moisture, 1200, 4095, 100, 0);
  percentage=constrain(percentage,0,100);
  Blynk.virtualWrite(V0,percentage);
  Serial.println(percentage);
}
void setup() {
  pinMode(32,INPUT);
  pinMode(27,OUTPUT);
  Serial.begin(115200);
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)){
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.clearDisplay();
  Serial.begin(115200);
  Blynk.begin(auth,ssid,pass);
  timer.setInterval(100L, sendSensor);
  pinMode(32,INPUT);
  Serial.begin(115200);
  Serial.print("Connecting WiFi: ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,pass);
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  while(WiFi.status()!=WL_CONNECTED){
    Serial.print("Connecting");
    display.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  bot.sendMessage(CHAT_ID, "Bot started up", " ");
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)){
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.clearDisplay();
}

void loop() {
  digitalWrite(27,HIGH);
  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(50,25);
  display.print(percentage);
  display.print("%");
  display.println(" ");
  display.display();
  Blynk.run();
  timer.run();
  if(percentage>50){
    String message="Moisture in the diaper is High! Percentage Value:  " + String(percentage);
    bot.sendMessage(CHAT_ID, message, "");
    Serial.println(message);
    delay(1000);
  }
  //delay(1000);
}
