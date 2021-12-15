#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ESP8266WebServer.h>
#include <Adafruit_NeoPixel.h>
#include "constants.h"
#include "struct.h"
#include <stdio.h>
#include <string.h>

// Instances
X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure client;
UniversalTelegramBot bot("", client);
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, LED_PIN, NEO_GRB + NEO_KHZ800);
ESP8266WebServer server(80);

// Operational variables
configuration config;
volatile bool push_button_flag = false;
unsigned long start_wifi_time;
unsigned long start_link_time;
unsigned long last_bot_pool;
bool ap_mode = false;

ICACHE_RAM_ATTR void pushButtonInterrupt() {
  push_button_flag = true;
}

// Web server handlers
void handleRoot() {
  server.send(200, "text/html", SETTINGS_HOME_PAGE_1 + String(config.ssid)
                              + SETTINGS_HOME_PAGE_2 + String(config.password)
                              + SETTINGS_HOME_PAGE_3 + String(config.bot_token)
                              + SETTINGS_HOME_PAGE_4 + String(config.message)
                              + SETTINGS_HOME_PAGE_5);
}

void handleSave() {
  if (server.argName(0) == "ssid"
      && server.argName(1) == "password"
      && server.argName(2) == "bot_token"
      && server.argName(3) == "message") {
    // Save the settings
    server.arg(0).toCharArray(config.ssid, 50);
    server.arg(1).toCharArray(config.password, 50);
    server.arg(2).toCharArray(config.bot_token, 50);
    server.arg(3).toCharArray(config.message, 50);
    // Update the eeprom memory
    EEPROM.put(0,config);
    EEPROM.commit();
    server.send(200, "text/html", SETTINGS_SAVED_SUCCESS_1 + server.arg(0) + SETTINGS_SAVED_SUCCESS_2);
    for(uint8_t i = 0; i < 2; i++) {
      led(0,255,0);
      delay(200);
      led(255,255,0);
      delay(200);
    }
  } else {
    server.send(200, "text/html", SETTINGS_SAVED_FAIL);
    for(uint8_t i = 0; i < 5; i++) {
      led(255,0,0);
      delay(200);
      led(255,255,0);
      delay(200);
    }
  }
  
}

// Save a new chat id
bool setChatId(String new_id) {
  bool saved = false;
  // Save into the config var
  if(String(config.chat_id_a) == "") {
    new_id.toCharArray(config.chat_id_a, 50);
    saved = true;
  } else if(String(config.chat_id_b) == "") {
    new_id.toCharArray(config.chat_id_b, 50);
    saved = true;
  } else if(String(config.chat_id_c) == "") {
    new_id.toCharArray(config.chat_id_c, 50);
    saved = true;
  } else if(String(config.chat_id_d) == "") {
    new_id.toCharArray(config.chat_id_d, 50);
    saved = true;
  }
  // Update the eeprom memory
  EEPROM.put(0,config);
  EEPROM.commit();
  return saved;
}

// Telegram handler
void analyzeCommand(int numNewMessages) {
  for (int i=0; i<numNewMessages; i++) {    
    String text = bot.messages[i].text;
    String chat_id = String(bot.messages[i].chat_id);
    if (text == "/start") {
      bot.sendMessage(chat_id, START_MESSAGE, "");
    }
    if (text == "/vincular") {
      bot.sendMessage(chat_id, LINK_MESSAGE, "");
      led(255,0,255);
      start_link_time = millis();
      while (millis() - start_link_time < LINK_TIMEOUT && !push_button_flag) {
        delay(100);
      }

      if (push_button_flag) {
        if (setChatId(chat_id)) {
          String from_name = bot.messages[i].from_name;
          for(uint8_t i = 0; i < 2; i++) {
            led(0,255,0);
            delay(200);
            led(255,0,255);
            delay(200);
          }
          //bot.sendMessage(config.chat_id_a, LINK_SUCCESS_ADMIN_MESSAGE + from_name, "");
          bot.sendMessage(chat_id, LINK_SUCCESS_MESSAGE, "");   
        } else {
          for(uint8_t i = 0; i < 5; i++) {
            led(255,0,0);
            delay(200);
            led(255,0,255);
            delay(200);
          }
          bot.sendMessage(chat_id, LINK_FAIL_MESSAGE, "");
        }        
      } else {
        for(uint8_t i = 0; i < 5; i++) {
          led(255,0,0);
          delay(200);
          led(255,0,255);
          delay(200);
        }
        bot.sendMessage(chat_id, LINK_FAIL_MESSAGE, "");
      }
      push_button_flag = false;
      led(0,255,0); 
    }

  }
}

void led(uint8_t r, uint8_t g, uint8_t b) {
  uint32_t color = pixels.Color(r,g,b);
  pixels.setPixelColor(0, color);
  pixels.show();
}

// Enable AP mode
void enableApMode() {
  Serial.println("Creating an AP");
  WiFi.softAP(AP_NAME, AP_PASSWORD);
  Serial.println("Booting the congif server");
  server.on("/", handleRoot);
  server.on("/save", handleSave);
  server.begin();
  push_button_flag = false;
  led(255,255,0);
}

void setup() {
  // Set comms
  Serial.begin(115200);
  // Begin the LED
  pixels.begin();
  // Welcome
  Serial.println("\nHi. I am your button.");
  for (uint8_t i = 0; i < 200; i+=10) {
    led(0,0,i);
    delay(100);
  }
  led(0,0,0);
  // Set IO
  pinMode(PUSH_BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PUSH_BUTTON_PIN), pushButtonInterrupt, RISING);
  if (!digitalRead(PUSH_BUTTON_PIN)) {
    ap_mode = true;
    Serial.println("AP mode enabled");
  }
  // Get the config data from eeprom
  EEPROM.begin(512);
  delay(100);
  EEPROM.get(0, config);
  // Set the Telegram token
  bot.updateToken(config.bot_token);
  // Config UTC time
  configTime(0, 0, "pool.ntp.org");
  // Config the trust certs
  client.setTrustAnchors(&cert);
  if (!ap_mode) {
    // Connect to the network
    Serial.println("Connecting to WiFi");
    WiFi.mode(WIFI_STA);
    WiFi.begin(config.ssid, config.password);
    start_wifi_time = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - start_wifi_time < WIFI_TIMEOUT) {
      Serial.print(".");
      led(0,0,255);
      delay(500);
      led(0,0,0);
      delay(500);
    }
    if (WiFi.status() != WL_CONNECTED) {
      for(uint8_t i = 0; i < 5; i++) {
        led(0,0,255);
        delay(200);
        led(255,0,0);
        delay(200);
      }
      led(0,0,0);
      enableApMode();
    } else {
      led(0,255,0);
    }
  } else {
    enableApMode();
  }
  
}

void loop() {
  // Pool the bot every BOT_POOL_DELAY miliseconds
  if (millis() - last_bot_pool > BOT_POOL_DELAY)  {
    int buffer_size = bot.getUpdates(bot.last_message_received + 1);
    // Clear the buffer of messages
    while (buffer_size) {
      analyzeCommand(buffer_size);
      buffer_size = bot.getUpdates(bot.last_message_received + 1);
    }
    last_bot_pool = millis();
  }
  // Handle web clients
  if (ap_mode) {
    server.handleClient();
  }
  // Send the message
  if (!ap_mode && push_button_flag) {
    push_button_flag = false;
    for(uint8_t i = 0; i < 2; i++) {
      led(0,0,0);
      delay(200);
      led(0,255,0);
      delay(200);
    }
    if (strlen(config.chat_id_a) > 2)
      bot.sendMessage(config.chat_id_a, config.message, "");
    if (strlen(config.chat_id_b) > 2)
      bot.sendMessage(config.chat_id_b, config.message, "");
    if (strlen(config.chat_id_c) > 2)
      bot.sendMessage(config.chat_id_c, config.message, "");
    if (strlen(config.chat_id_d) > 2)
      bot.sendMessage(config.chat_id_d, config.message, "");
  }
}