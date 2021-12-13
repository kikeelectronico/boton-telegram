#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

// Config
struct configuration {
  char ssid[50];
  char password[50];
  char bot_token[50];
  char chat_id_a[50];
  char chat_id_b[50];
  char chat_id_c[50];
  char chat_id_d[50];
};

configuration config;

// Constants            
#define START_MESSAGE               "Bienvenide.\n\nUsa el comando /vincular para subscribirte a las notificaciones del botón."
#define LINK_MESSAGE                "Pulsa el botón 5 veces para vincular esta cuenta de Telegram.\n\nDispones de 20 segundos para ello."
#define LINK_SUCCESS_ADMIN_MESSAGE  "Nueva cuenta vinculada:"
#define LINK_SUCCESS_MESSAGE        "cuenta vinculada con éxito."
#define LINK_FAIL_MESSAGE           "La vinculación ha fallado."
#define LINK_TIMEOUT                20000
#define BOT_POOL_DELAY              1000

X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure client;
UniversalTelegramBot bot(config.bot_token, client);

// Operational variables
uint8_t button_count = 0;
unsigned long start_link_time;
unsigned long last_bot_pool;

void setChatId(String new_id) {
  // Save into the config var
  if(String(config.chat_id_a) == "") {
    new_id.toCharArray(config.chat_id_a, 50);
  } else if(String(config.chat_id_b) == "") {
    new_id.toCharArray(config.chat_id_b, 50);
  } else if(String(config.chat_id_c) == "") {
    new_id.toCharArray(config.chat_id_c, 50);
  } else if(String(config.chat_id_d) == "") {
    new_id.toCharArray(config.chat_id_d, 50);
  }
  // Update the eeprom memory
  EEPROM.put(0,config);
  EEPROM.commit();
}

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
      while (millis() - start_link_time < LINK_TIMEOUT && button_count < 6) {
        if (button_count >= 5) {
          setChatId(chat_id);
          button_count++;
        }
        delay(100);
      }
      if (button_count == 6) {
        String from_name = bot.messages[i].from_name;
        bot.sendMessage(config.chat_id_a, LINK_SUCCESS_ADMIN_MESSAGE, "");
        bot.sendMessage(config.chat_id_a, from_name, "");
        bot.sendMessage(chat_id, LINK_SUCCESS_MESSAGE, "");        
      } else {
        bot.sendMessage(chat_id, LINK_FAIL_MESSAGE, "");
      }
      
    }

  }
}

void setup() {
  // Set comms
  Serial.begin(115200);
  // Get the config data from eeprom
  EEPROM.begin(512);
  delay(100);
  EEPROM.get(0, config);
  // Config UTC time
  configTime(0, 0, "pool.ntp.org");
  // Config the trust certs
  client.setTrustAnchors(&cert);
  // Connect to the network
  WiFi.mode(WIFI_STA);
  WiFi.begin(config.ssid, config.password);
  Serial.println("connecting to WiFi");
  // ToDo: Timeout
  // ToDo: LED alert
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
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
}