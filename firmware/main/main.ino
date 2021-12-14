#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ESP8266WebServer.h>
#include "constants.h"
#include "struct.h"

// Instances
X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure client;
UniversalTelegramBot bot("", client);
ESP8266WebServer server(80);

// Operational variables
configuration config;
int button_count = 0;
volatile bool push_button_flag = false;
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
                              + SETTINGS_HOME_PAGE_4);
}

void handleSave() {  
  Serial.print(server.argName(0));
  Serial.print(" ");
  Serial.println(server.arg(0));
  Serial.print(server.argName(1));
  Serial.print(" ");
  Serial.print(server.arg(1));
  Serial.print(server.argName(2));
  Serial.print(" ");
  Serial.print(server.arg(2));
  if (server.argName(0) == "ssid" && server.argName(1) == "password" && server.argName(2) == "bot_token") {
    // Save the settings
    server.arg(0).toCharArray(config.ssid, 50);
    server.arg(1).toCharArray(config.password, 50);
    server.arg(2).toCharArray(config.bot_token, 50);
    // Update the eeprom memory
    EEPROM.put(0,config);
    EEPROM.commit();
    server.send(200, "text/html", SETTINGS_SAVED_SUCCESS_1 + server.arg(0) + SETTINGS_SAVED_SUCCESS_2);
  } else {
    server.send(200, "text/html", SETTINGS_SAVED_FAIL);
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
  return saved
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
      start_link_time = millis();
      bool linked = false;
      while (millis() - start_link_time < LINK_TIMEOUT && button_count < 6) {
        if (push_button_flag) {
          push_button_flag = false;
          button_count++;
        }
        if (button_count == 5) {
          linked = setChatId(chat_id);
          button_count++;
        }
        delay(100);
      }
      if (linked) {
        String from_name = bot.messages[i].from_name;
        bot.sendMessage(config.chat_id_a, LINK_SUCCESS_ADMIN_MESSAGE + from_name, "");
        bot.sendMessage(chat_id, LINK_SUCCESS_MESSAGE, "");        
      } else {
        bot.sendMessage(chat_id, LINK_FAIL_MESSAGE, "");
      }
      button_count = 0;      
    }

  }
}

void setup() {
  // Set comms
  Serial.begin(115200);
  Serial.println("\nHi, I am your button.");
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
    // ToDo: Timeout
    // ToDo: LED alert
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.print(".");
    }
  } else {
    Serial.println("Creating an AP");
    WiFi.softAP(AP_NAME, AP_PASSWORD);
    Serial.println("Booting the congif server");
    server.on("/", handleRoot);
    server.on("/save", handleSave);
    server.begin();
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
}