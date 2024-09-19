#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <DHT.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_I2C_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

const char* MQTT_CLIENT_ID = "";
const char* MQTT_BROKER = "broker.hivemq.com";
//const char* MQTT_BROKER = "192.168.200.116";
const char* MQTT_USER = "";
const char* MQTT_PASSWORD = "";
const int MQTT_PORT = 1883;

const int GAS_PIN = 35;
const int PULSO_PIN = 34;
const int LED_VERDE_PIN = 33;
const int LED_AMARILLO_PIN = 14;
const int LED_ROJO_PIN = 12;
const int BUZZER_PIN = 15;
const int VIBRADOR_PIN = 27;
float promedioRecibido; 

int led_verde = LED_VERDE_PIN;
int led_amarillo = LED_AMARILLO_PIN;  
int led_rojo = LED_ROJO_PIN;
int buzzer = BUZZER_PIN;


float temperature = 0;
float humidity = 0;
float gas = 0;
int pulseValue = 0;
int ax = 0, ay = 0, az = 0;
// Variables globales para almacenar valores previos del giroscopio
int prev_ax = 0, prev_ay = 0, prev_az = 0;

WiFiClient espClient;
PubSubClient client(espClient);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", -6 * 3600); // GMT-6 para hora de Guanajuato

void conectarWifi() {
  WiFi.begin("POCO X3 Pro", "123456789");
  //WiFi.begin("LiveTelecom_152", "03627006");
  //WiFi.begin("MEGACABLE_2.4G", "WUTYndVLhzjdjzenMJ");
  //WiFi.begin("DECB_EXTEND", "h6M3k5M8N7Z5F8C5a2a2");
  //WiFi.begin("jmsawifi", "linux123");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.println("");
  Serial.println("Conectado a la red WiFi");
}

void conectarBroker() {
  client.setServer(MQTT_BROKER, MQTT_PORT);
  while (!client.connected()) {
    Serial.print("Conectando al broker MQTT...");
    if (client.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASSWORD)) {
      Serial.println("Conectado");
      client.subscribe("utng/proyecto/graficaTemperatura");
      client.subscribe("utng/proyecto/graficaHumedad");
      client.subscribe("utng/proyecto/graficaPulso");
      client.subscribe("utng/proyecto/graficaMovimiento");
      client.subscribe("utng/proyecto/graficaGas");
      client.subscribe("utng/proyecto/controlBuzzer"); // Tópico del buzzer
      client.subscribe("utng/proyecto/controlVibrador"); // Tópico del vibrador
      client.subscribe("utng/proyecto/controlPulso"); // Tópico del pulso
      client.subscribe("actuadores/ledVerde"); // Topico control de led verde
      client.subscribe("actuadores/ledBlanco"); // Topico control de led blanco
      client.subscribe("actuadores/ledAzul"); // Topico control de led verde
      client.subscribe("actuadores/buzzer"); // Topico control del buzzer
      client.subscribe("actuadores/vibrador"); // Topico control del vibrador
    } else {
      Serial.print("falló, rc=");
      Serial.print(client.state());
      Serial.println(" Intentando de nuevo en 5 segundos");
      delay(5000);
    }
  }
}

void displayTemperatureGauge() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Mostrar título
  display.setCursor(0, 0);
  display.println("Temperatura");

  // Dibujar la escala del gauge (semicírculo)
  int centerX = SCREEN_WIDTH / 2;
  int centerY = SCREEN_HEIGHT / 2 + 10; // Ajustar la posición vertical
  int radius = 25;

  for (int i = 0; i <= 180; i += 10) {
    int x1 = centerX + radius * cos(radians(i));
    int y1 = centerY - radius * sin(radians(i));
    int x2 = centerX + (radius - 5) * cos(radians(i));
    int y2 = centerY - (radius - 5) * sin(radians(i));
    display.drawLine(x1, y1, x2, y2, SSD1306_WHITE);
  }

  // Dibujar la aguja del gauge
  float angle = map(temperature, 0, 50, 180, 0);
  int needleX = centerX + (radius - 5) * cos(radians(angle));
  int needleY = centerY - (radius - 5) * sin(radians(angle));
  display.drawLine(centerX, centerY, needleX, needleY, SSD1306_WHITE);

  // Dibujar las marcas de temperatura
  display.setCursor(centerX - radius - 10, centerY + 10);
  display.print("0 C");

  display.setCursor(centerX + radius - 10, centerY + 10);
  display.print("50 C");

  // Mostrar valor de temperatura
  display.setTextSize(1);
  display.setCursor(centerX - 10, centerY - radius - 10);
  display.print(temperature, 1);
  display.print(" C");

  display.display();
  delay(10000); // Mostrar la gráfica durante 10 segundos
  displayTime(); // Volver a mostrar la hora después de 10 segundos
}

void displayHumidityGauge() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Mostrar título
  display.setCursor(0, 0);
  display.println("Humedad");

  // Dibujar la escala del gauge (semicírculo)
  int centerX = SCREEN_WIDTH / 2;
  int centerY = SCREEN_HEIGHT / 2 + 10; // Ajustar la posición vertical
  int radius = 25;

  for (int i = 0; i <= 180; i += 10) {
    int x1 = centerX + radius * cos(radians(i));
    int y1 = centerY - radius * sin(radians(i));
    int x2 = centerX + (radius - 5) * cos(radians(i));
    int y2 = centerY - (radius - 5) * sin(radians(i));
    display.drawLine(x1, y1, x2, y2, SSD1306_WHITE);
  }

  // Dibujar la aguja del gauge
  float angle = map(humidity, 0, 100, 180, 0);
  int needleX = centerX + (radius - 5) * cos(radians(angle));
  int needleY = centerY - (radius - 5) * sin(radians(angle));
  display.drawLine(centerX, centerY, needleX, needleY, SSD1306_WHITE);

  // Dibujar las marcas de humedad
  display.setCursor(centerX - radius - 10, centerY + 10);
  display.print("0%");

  display.setCursor(centerX + radius - 10, centerY + 10);
  display.print("100%");

  // Mostrar valor de humedad
  display.setTextSize(1);
  display.setCursor(centerX - 10, centerY - radius - 10);
  display.print(humidity, 1);
  display.print("%");

  display.display();
  delay(10000); // Mostrar la gráfica durante 10 segundos
  displayTime(); // Volver a mostrar la hora después de 10 segundos
}

void displayPulseGauge() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.println("Pulso:");

  int centerX = SCREEN_WIDTH / 2;
  int centerY = SCREEN_HEIGHT / 2 + 10;
  int radius = 30;

  // Dibuja la gráfica tipo gauge
  display.drawCircle(centerX, centerY, radius, SSD1306_WHITE);
  int angle = map(pulseValue, 0, 2500, 0, 180);
  int endX = centerX + radius * cos(radians(angle));
  int endY = centerY - radius * sin(radians(angle));
  display.drawLine(centerX, centerY, endX, endY, SSD1306_WHITE);

  // Dibuja la escala
  display.setCursor(5, centerY - radius - 10);
  display.println("0");
  display.setCursor(SCREEN_WIDTH - 20, centerY - radius - 10);
  display.println("2500");

  // Muestra el valor del pulso
  display.setCursor(centerX - 10, centerY + radius + 10);
  display.print(pulseValue);

  display.display();
  delay(10000); // Mostrar la gráfica durante 10 segundos
}

void displayGyroscopeLineGraph() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Giroscopio (AX, AY, AZ)");

  // Dibujar líneas para cada eje
  for (int i = 0; i < SCREEN_WIDTH - 1; i++) {
    int x1 = i;
    int x2 = i + 1;
    int y1_ax = map(ax, -17000, 17000, SCREEN_HEIGHT, 0);
    int y2_ax = map(ax, -17000, 17000, SCREEN_HEIGHT, 0);
    int y1_ay = map(ay, -17000, 17000, SCREEN_HEIGHT, 0);
    int y2_ay = map(ay, -17000, 17000, SCREEN_HEIGHT, 0);
    int y1_az = map(az, -17000, 17000, SCREEN_HEIGHT, 0);
    int y2_az = map(az, -17000, 17000, SCREEN_HEIGHT, 0);

    display.drawLine(x1, y1_ax, x2, y2_ax, SSD1306_WHITE);
    display.drawLine(x1, y1_ay, x2, y2_ay, SSD1306_WHITE);
    display.drawLine(x1, y1_az, x2, y2_az, SSD1306_WHITE);
  }

  display.display();
  delay(10000); // Mostrar la gráfica durante 10 segundos
}

void displayGasGauge() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Mostrar título
  display.setCursor(0, 0);
  display.println("Nivel de Gas");

  // Dibujar la escala del gauge (semicírculo)
  int centerX = SCREEN_WIDTH / 2;
  int centerY = SCREEN_HEIGHT / 2 + 10; // Ajustar la posición vertical
  int radius = 25;

  for (int i = 0; i <= 180; i += 10) {
    int x1 = centerX + radius * cos(radians(i));
    int y1 = centerY - radius * sin(radians(i));
    int x2 = centerX + (radius - 5) * cos(radians(i));
    int y2 = centerY - (radius - 5) * sin(radians(i));
    display.drawLine(x1, y1, x2, y2, SSD1306_WHITE);
  }

  // Dibujar la aguja del gauge
  float angle = map(gas, 0, 3000, 180, 0); // Ajustar el rango según sea necesario
  int needleX = centerX + (radius - 5) * cos(radians(angle));
  int needleY = centerY - (radius - 5) * sin(radians(angle));
  display.drawLine(centerX, centerY, needleX, needleY, SSD1306_WHITE);

  // Dibujar las marcas de gas
  display.setCursor(centerX - radius - 10, centerY + 10);
  display.print("0");

  display.setCursor(centerX + radius - 10, centerY + 10);
  display.print("3000");

  // Mostrar valor del gas
  display.setTextSize(1);
  display.setCursor(centerX - 10, centerY - radius - 10);
  display.print(gas);
  display.print(" ");

  display.display();
  delay(10000); // Mostrar la gráfica durante 10 segundos
  displayTime(); // Volver a mostrar la hora después de 10 segundos
}

void displayTime() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);

  timeClient.update();
  String formattedTime = timeClient.getFormattedTime();
  int hour = timeClient.getHours();
  int minute = timeClient.getMinutes();
  int second = timeClient.getSeconds();

  char timeString[16];
  snprintf(timeString, sizeof(timeString), "%02d:%02d:%02d", hour, minute, second);

  int16_t x1, y1;
  uint16_t width, height;
  display.getTextBounds(timeString, 0, 0, &x1, &y1, &width, &height);
  int x = (SCREEN_WIDTH - width) / 2;
  int y = (SCREEN_HEIGHT - height) / 2;

  display.setCursor(x, y);
  display.print(timeString);

  display.display();
}

void controlLEDs(float temperature) {
  if (temperature < 36) {
    digitalWrite(led_verde, HIGH);
    digitalWrite(LED_AMARILLO_PIN, LOW);
    digitalWrite(led_rojo, LOW);
    digitalWrite(VIBRADOR_PIN, LOW);  // Apagar vibrador
    digitalWrite(BUZZER_PIN, LOW);    // Apagar buzzer
  } else if (temperature >= 37 && temperature <= 38) {
    digitalWrite(led_verde, LOW);
    digitalWrite(LED_AMARILLO_PIN, HIGH);
    digitalWrite(led_rojo, LOW);
    digitalWrite(VIBRADOR_PIN, HIGH); // Encender vibrador
    digitalWrite(BUZZER_PIN, LOW);    // Apagar buzzer
  } else {
    digitalWrite(led_verde, LOW);
    digitalWrite(LED_AMARILLO_PIN, LOW);
    digitalWrite(led_rojo, HIGH);
    digitalWrite(VIBRADOR_PIN, LOW); // Encender vibrador
    digitalWrite(BUZZER_PIN, HIGH);   // Encender buzzer
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin(19, 18);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDRESS)) {
    Serial.println(F("No se encontró el display OLED."));
    while (1) delay(10);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  dht.begin();

  pinMode(led_verde, OUTPUT);
  pinMode(led_rojo, OUTPUT);
  pinMode(LED_AMARILLO_PIN, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(PULSO_PIN, INPUT);
  pinMode(VIBRADOR_PIN, OUTPUT);

  conectarWifi();
  conectarBroker();
  timeClient.begin();

  client.setCallback(callback);

}

void loop() {
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
  gas = analogRead(GAS_PIN);
  pulseValue = analogRead(PULSO_PIN);

  pulseValue = random(60,100);

  ax = random(0, 1000);
  ay = random(0, 1000);
  az = random(0, 1000);

  Serial.print("Pulso: ");
  Serial.println(pulseValue);
  Serial.print("Giroscopio AX: ");
  Serial.println(ax);
  Serial.print("Giroscopio AY: ");
  Serial.println(ay);
  Serial.print("Giroscopio AZ: ");
  Serial.println(az);
  Serial.print("Lectura de gas: ");
  Serial.println(gas);
  Serial.println("Temperatura: ");
  Serial.println(temperature);
  Serial.println("Humedad: ");
  Serial.println(humidity); 

  client.publish("utng/proyecto/temperature", String(temperature).c_str());
  client.publish("utng/proyecto/humidity", String(humidity).c_str());
  client.publish("utng/proyecto/pulse", String(pulseValue).c_str());
  client.publish("utng/proyecto/gyroscope", String(ax).c_str());
  client.publish("utng/proyecto/gas", String(gas).c_str());

     // Controlar LEDs basado en la temperatura
  controlLEDs(temperature);

  // Procesar mensajes MQTT
  client.loop();

  // Mostrar la hora en lugar de las gráficas
  displayTime();

  delay(1000); // Actualiza la hora cada segundo
}

void callback(char* topic, byte* payload, unsigned int length) {
  String topicStr = String(topic);
  String message;

  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  Serial.print("Tópico: ");
  Serial.println(topicStr);
  Serial.print("Mensaje: ");
  Serial.println(message);

  // Control para activar el buzzer dependiendo de la temperatura del promedio de la temperatura

  if (topicStr == "utng/proyecto/controlBuzzer") {
    promedioRecibido = message.toFloat();  // Convertir el mensaje a un número flotante

    // Activar el buzzer si el promedio supera un valor determinado (ej. 38 grados)
    if (message == "activar buzzer" ) {
      tone(buzzer, 1000);  // Suena a 1000 Hz
      delay(5000);         // Suena durante 1 segundo
      noTone(buzzer);      // Detener el sonido
    }
  }

  // Control para actizar el vibrador de acuerdo a el valor maximo del gas

  if (topicStr == "utng/proyecto/controlVibrador") {
    promedioRecibido = message.toFloat();  // Convertir el mensaje a un número flotante

    // Activar el vibrador si el valor maximo es superado (ej. 38 grados)
    if (topicStr == "utng/proyecto/controlVibrador") {
    // Activar el vibrador si el mensaje es "activar vibrador"
    if (message == "activar vibrador") {
      digitalWrite(VIBRADOR_PIN, HIGH); // Encender el vibrador
      delay(5000);                      // Encender durante 5 segundos
      digitalWrite(VIBRADOR_PIN, LOW);  // Apagar el vibrador
    }
  }
  }

  // Control para activar los leds de acuerdo a el valor minimo del pulso

  if (topicStr == "utng/proyecto/controlPulso") {
  promedioRecibido = message.toFloat();  // Convertir el mensaje a un número flotante

  // Verifica si el mensaje es "activar pulso"
  if (message == "activar pulso") {
    // Muestra el mensaje en la pantalla OLED
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.print(F("Se registró un conteo"));
    display.setCursor(0, 10);
    display.print(F("máximo en la base de datos"));
    display.display();

    // Pausa para mostrar el mensaje durante 3 segundos
    delay(3000);

    // Después de mostrar el mensaje, puedes limpiar la pantalla o seguir con otras acciones
    display.clearDisplay();
    display.display();
  }
}

  // ------------------------------------------------------------------------

  // Codigo para controlar los actuadores

  // Verifica si el mensaje es para encender o apagar el LED verde
  if (String(topic) == "actuadores/ledVerde") {
    if (message == "on") {
      digitalWrite(led_verde, HIGH); // Enciende el LED
      delay(3000);
      digitalWrite(led_verde, LOW); // Apaga el LED
    } else if (message == "off") {
      digitalWrite(led_verde, LOW); // Apaga el LED
    }
  }

  // Verifica si el mensaje es para encender o apagar el LED blanco
  if (String(topic) == "actuadores/ledBlanco") {
    if (message == "on") {
      digitalWrite(led_amarillo, HIGH); // Enciende el LED
      delay(3000);
      digitalWrite(led_amarillo, LOW); // Apaga el LED
    } else if (message == "off") {
      digitalWrite(led_amarillo, LOW); // Apaga el LED
    }
  }

  // Verifica si el mensaje es para encender o apagar el LED azul
  if (String(topic) == "actuadores/ledAzul") {
    if (message == "on") {
      digitalWrite(led_rojo, HIGH); // Enciende el LED
      delay(3000);
      digitalWrite(led_rojo, LOW); // Apaga el LED
    } else if (message == "off") {
      digitalWrite(led_rojo, LOW); // Apaga el LED
    }
  }

  // Verifica si el mensaje es para encender o apagar el buzzer
  if (String(topic) == "actuadores/buzzer") {
    if (message == "on") {
      tone(buzzer, 1000); // Enciende el buzzer
      delay(5000);
      noTone(buzzer);
    } else if (message == "off") {
      noTone(buzzer); // Apaga el buzzer
    }
  }

  // Verifica si el mensaje es para encender o apagar el vibrador
  if (String(topic) == "actuadores/vibrador") {
    if (message == "on") {
      digitalWrite(VIBRADOR_PIN, HIGH); // Enciende el vibrador
      delay(5000);
      digitalWrite(VIBRADOR_PIN, LOW); // Apaga el vibrador
    } else if (message == "off") {
      digitalWrite(VIBRADOR_PIN, LOW); // Apaga el vibrador
    }
  }



  // -------------------------------------------------------------------------------

  // Codigo para mostrar las graficas en la OLED

  if (topicStr == "utng/proyecto/graficaTemperatura") {
    displayTemperatureGauge(); // Mostrar la gráfica tipo gauge de temperatura
  } else if (topicStr == "utng/proyecto/graficaHumedad") {
    displayHumidityGauge(); // Mostrar la gráfica tipo gauge de humedad
  } else if (topicStr == "utng/proyecto/graficaPulso") {
    displayPulseGauge(); // Mostrar la gráfica de pulso
  } else if (topicStr == "utng/proyecto/graficaMovimiento") {
    displayGyroscopeLineGraph(); // Mostrar la gráfica de movimiento
  } else if (topicStr == "utng/proyecto/graficaGas") {
    displayGasGauge(); // Mostrar la gráfica tipo gauge de gas
  }
}
