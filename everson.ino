#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

// Configurações do Adafruit IO
#define AIO_SERVER "io.adafruit.com"
#define AIO_SERVERPORT 1883 // Use 8883 para SSL
#define AIO_USERNAME "EversonFerro"
#define AIO_KEY "<colocar aqui a KEY>T"

// Configurações do WiFi
#define WLAN_SSID "Sala-2.4G"
#define WLAN_PASS "gixpg300m"

// Objeto de WiFiClient
WiFiClient client;

// Configuração do cliente MQTT do Adafruit IO
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// Feeds de temperatura, umidade e CO2 no Adafruit IO
Adafruit_MQTT_Publish temperatureFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/TemperaturaLevel");
Adafruit_MQTT_Publish humidityFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/UmidadeLevel");
Adafruit_MQTT_Publish co2Feed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/CO2Level");

float humidity, temperature_C, co2;
SoftwareSerial mySerial(D6, D7); // RX, TX

void setup() {
  Serial.begin(115200);
  while (!Serial) { ; }

  mySerial.begin(115200);

  Serial.println(F("Conectando ao WiFi..."));
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println(F("WiFi conectado"));
}

void loop() {
  MQTT_connect();

  if (mySerial.available()) {
    String data = mySerial.readStringUntil('>');
    int startIdx = data.indexOf('<');
    if (startIdx != -1) {
      data = data.substring(startIdx + 1);  // Remove o marcador de início '<'
      Serial.println("Received data: " + data);

      // Verifica se a string contém duas vírgulas
      int firstCommaIndex = data.indexOf(',');
      int secondCommaIndex = data.indexOf(',', firstCommaIndex + 1);

      if (firstCommaIndex != -1 && secondCommaIndex != -1 && secondCommaIndex < data.length() - 1) {
        String humidityStr = data.substring(0, firstCommaIndex);
        String temperatureStr = data.substring(firstCommaIndex + 1, secondCommaIndex);
        String co2Str = data.substring(secondCommaIndex + 1);

        // Validação e conversão dos dados recebidos
        if (isFloat(humidityStr) && isFloat(temperatureStr) && isFloat(co2Str)) {
          humidity = humidityStr.toFloat();
          temperature_C = temperatureStr.toFloat();
          co2 = co2Str.toFloat();

          // Exibindo os valores parseados no Serial Monitor
          Serial.print("Parsed humidity: ");
          Serial.println(humidity);
          Serial.print("Parsed temperature: ");
          Serial.println(temperature_C);
          Serial.print("Parsed CO2: ");
          Serial.println(co2);

          // Publicação dos dados no Adafruit IO
          if (mqtt.connected()) {
            if (!humidityFeed.publish(humidity)) {
              Serial.println("Failed to publish humidity");
            } else {
              Serial.println("Humidity published!");
            }
            delay(80);
            if (!temperatureFeed.publish(temperature_C)) {
              Serial.println("Failed to publish temperature");
            } else {
              Serial.println("Temperature published!");
            }
            delay(80);
            if (!co2Feed.publish(co2)) {
              Serial.println("Failed to publish CO2");
            } else {
              Serial.println("CO2 published!");
            }
          }
        } else {
          Serial.println("Error: Invalid data format");
        }
      } else {
        Serial.println("Error: Invalid data format or missing comma");
      }
    } else {
      Serial.println("Error: No valid start marker");
    }
  } else {
    Serial.println("No data available on SoftwareSerial");
  }

  delay(6000); // Delay para diminuir a taxa de atualização
}

// Função para validar se uma string pode ser convertida em um float
bool isFloat(String str) {
  char* endptr;
  str.toFloat();
  float val = str.toFloat();
  return !isnan(val);
}

void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000); 
  }
  Serial.println("MQTT Connected!");
}
