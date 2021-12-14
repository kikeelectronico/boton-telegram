// Constants
#define SETTINGS_HOME_PAGE_1          "<html> <head> <meta charset=\"UTF-8\"> </head> <body> \
                                      <form action=\"/save\"> \
                                        <label for=\"fname\">SSID</label><br> \
                                        <input type=\"text\" id=\"ssid\" name=\"ssid\" value=\""      
#define SETTINGS_HOME_PAGE_2              "\"><br> \
                                        <label for=\"lname\">Contraseña</label><br> \
                                        <input type=\"text\" id=\"password\" name=\"password\" value=\""            
#define SETTINGS_HOME_PAGE_3              "\"><br> \
                                        <label for=\"lname\">Token del bot</label><br> \
                                        <input type=\"text\" id=\"bot_token\" name=\"bot_token\" value=\""            
#define SETTINGS_HOME_PAGE_4              "\"><br> \
                                        <label for=\"lname\">Mensaje</label><br> \
                                        <input type=\"text\" id=\"message\" name=\"message\" value=\""            
#define SETTINGS_HOME_PAGE_5              "\"><br><br> \
                                        <input type=\"submit\" value=\"Guardar\"> \
                                      </form> \
                                    </body> </html>"

#define SETTINGS_SAVED_SUCCESS_1    "<html> <head> <meta charset=\"UTF-8\"> </head> <body> \
                                      <p>La configuración se ha guardado correctamente. Reinicia el botón y vuelve a conectar tu dispositivo a "
#define SETTINGS_SAVED_SUCCESS_2    ".</p> </body> </html>"

#define SETTINGS_SAVED_FAIL           "<html> <head> <meta charset=\"UTF-8\"> </head> <body> \
                                      <p>Algo ha ido mal. La configuración no se ha guardado.</p> \
                                    </body> </html>"
#define START_MESSAGE               "Bienvenide.\n\nUsa el comando /vincular para subscribirte a las notificaciones del botón."
#define LINK_MESSAGE                "Pulsa el botón 5 veces para vincular esta cuenta de Telegram.\n\nDispones de 20 segundos para ello."
#define LINK_SUCCESS_ADMIN_MESSAGE  "Nueva cuenta vinculada: "
#define LINK_SUCCESS_MESSAGE        "Cuenta vinculada con éxito."
#define LINK_FAIL_MESSAGE           "La vinculación ha fallado."
#define WIFI_TIMEOUT                10000
#define LINK_TIMEOUT                20000
#define BOT_POOL_DELAY              1000
#define PUSH_BUTTON_PIN             13
#define AP_NAME                     "Soy tu botón"
#define AP_PASSWORD                 "vamosaconfigurarcosas"