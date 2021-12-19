/*
  This code allows you to configure the button manually.
  1. Type your configuration in my_configuration variable.
  2. Load the program in the board.
  3. Load the firmware in the board.
*/

#include <EEPROM.h>

struct configuration {
  char ssid[50];
  char password[50];
  char bot_token[50];
  char message[50];
  char chat_id_a[50];
  char chat_id_b[50];
  char chat_id_c[50];
  char chat_id_d[50];
};

configuration my_configuration = {
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
};

configuration my_configuration_readed;

void setup() {  
  
  Serial.begin(115200);   
  
  EEPROM.begin(512);
  delay(100);
  EEPROM.put(0,my_configuration);
  EEPROM.commit();
  
  EEPROM.get(0, my_configuration_readed);
  Serial.println("");
  Serial.println("");
  Serial.println(my_configuration_readed.ssid);
  Serial.println(my_configuration_readed.password);
  Serial.println(my_configuration_readed.bot_token);
  Serial.println(my_configuration_readed.message);
  Serial.println(my_configuration_readed.chat_id_a);
  Serial.println(my_configuration_readed.chat_id_b);
  Serial.println(my_configuration_readed.chat_id_c);
  Serial.println(my_configuration_readed.chat_id_d);
  Serial.println("");

}

void loop() {

}
