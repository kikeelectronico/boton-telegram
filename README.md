# El botón de Telegram
Se trata de un botón que envia un mesaje a través de Telegram hasta a cuatro usuarios.

<img src="https://raw.githubusercontent.com/kikeelectronico/boton-telegram/main/photos/cover.jpg">

# Materiales

- WeMos D1 mini
- Final de carrera.
- WS2812B LED
- PLA o material similar
- 2 tuercas M3
- 2 tornillos 45xM3
- Cable unifilar
- Tubo termoretráctil

# Herramientas

- PC
- Arduino IDE
- Soldador
- Impresora 3D

# Construcción

1. Imprime los modelos 3D de la caja.
2. Coloca el final de carrera usando los pines horizontales del centro de la pieza llamada Base.
3. Pega el LED detrás del agujero frontal de la base.
4. Coloca la placa WeMos D1 mini en el espacio reservado en el interior de la base.
5. Conecta el LED a Vcc, GND y al pin digital D6.
6. Conecta el final de carrera a GND y al pin digital D7.
7. Programa la placa usando el firmware y el IDE de Arduino.
8. Inserta 2 tuercas de métrica 3 en los huecos hexagonales de la pieza llamada Tapa.
9. Encaja la pieza llamada Botón en la pieza Tapa a través los agujeros redondos.
10. Encaja la Tapa en la Base y cierrala con dos tornillos de métrica 3 y 45 mm de largo desde la parte inferior.
11. Coloca las piezas decorativas: antideslizante y anillo.

# Conectar a tu red WiFi y configura el mensaje

Para realizar la configuración, El botón de Telegram genera su propia página web y red WiFi temporal a la que solo puedes acceder al estar cerca del dispositivo.

Para configurar botón:

1. Presiona el botón y no lo sueltes.
2. Conecta el cable USB.
3. Espera a que el LED se encienda en color amarillo.
4. Suelta el botón.

El botón de Telegram ha creado una red WiFi temporal llamada _Soy tu botón_ cuya contraseña es _vamosaconfigurarcosas_.

5. Conecta tu ordenador o dispositivo móvil a la red WiFi del botón.

Puede que el ordenador o el dispositivo móvil detecten que la red WiFi no tiene conexión a Internet. Si es así, el dispositivo pregunta si quieres mantener la conexión: mantenla.

6. Abre un navegador web y accede a la dirección http://192.168.4.1.
7. Rellena los datos de tu red WiFi, el token de tu bot de Telegram y el mensaje a enviar.

# Vincular una cuenta de Telegram para recibir mensajes

El botón puede enviar el mensaje hasta a cuatro usuarios de Telegram. Estos usuarios deben estar vinculados, un proceso que solo puedes llevar a cabo teniendo acceso físico al botón.

Para vincular un usuario:

1. Buscar tu bot en Telegram.
2. Inicia una conversación con el bot.
3. Envía el comando /start al bot y espera su respuesta.
4. Envía el comando /vincular y espera a que el LED del botón cambie al color morado.
5. Pulsa el botón.

Si el proceso de vinculación es correcto, el LED parpadea en color verde dos veces y el usuario recibe un mensaje del bot confirmando el proceso.

Si el proceso de vinculación falla, el LED parpadea en color rojo varias veces y el usuario recibe un mensaje del bot informando de que el proceso ha ido mal.

# El LED de colores

El botón de Telegram dispone de un LED con el que indica su estado.

|  Color |  Significado |
|---|---|
| Azul, fijo  | El dispositivo se está iniciando.  |
| Azul, parpadeo  | El dispositivo está buscando la red WiFi.  |
| Amarillo, fijo  | El dispositivo se encuentra en modo de configuración.  |
| Verde, parpadeo  | El proceso ha sido un éxito.  |
| Rojo, parpadeo  | El proceso ha fallado.  |
| Verde, fijo  | El dispositivo está preparado para enviar mensajes.  |


# Licencia
Creative Commons Atribución-CompartirIgual 4.0 Internacional.

