#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

// Adafruit IO Configurations
#define AIO_SERVER "io.adafruit.com"
#define AIO_SERVERPORT 1883 // Use 8883 for SSL
#define AIO_USERNAME "EversonFerro"
#define AIO_KEY "aio_uPqx72xDVg4MeDewEnG7QD3pHtKL"

// Configurações do WiFi
#define WLAN_SSID       "Sala-2.4G"
#define WLAN_PASS       "gixpg300m"

// Objeto de WiFiClient
WiFiClient client;

// Configuração do cliente MQTT Adafruit IO
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// Feeds de temperatura e umidade no Adafruit IO
Adafruit_MQTT_Publish temperatureFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/TemperaturaLevel");
Adafruit_MQTT_Publish humidityFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/UmidadeLevel");

float humidity, temperature_C, temperature_F;
SoftwareSerial mySerial(D6, D7); // RX, TX

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // set the data rate for the SoftwareSerial port
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
    String data = mySerial.readStringUntil('\n');
    Serial.println("Received data: " + data);

    // Parse the data
    int commaIndex = data.indexOf(',');
    if (commaIndex != -1) { // Check if comma is found
      String humidityStr = data.substring(0, commaIndex);
      String temperatureStr = data.substring(commaIndex + 1);

      humidity = humidityStr.toFloat();
      temperature_C = temperatureStr.toFloat();

      // Debug output
      Serial.print("Parsed humidity: ");
      Serial.println(humidity);
      Serial.print("Parsed temperature: ");
      Serial.println(temperature_C);

      // Check if the MQTT client is connected
      if (mqtt.connected()) {
        // Send data to Adafruit IO
        if (!humidityFeed.publish(humidity)) {
          Serial.println("Failed to publish humidity");
        } else {
          Serial.println("Humidity published!");
        }

        if (!temperatureFeed.publish(temperature_C)) {
          Serial.println("Failed to publish temperature");
        } else {
          Serial.println("Temperature published!");
        }
      }
    } else {
      Serial.println("Error: Invalid data format");
    }
  } else {
    Serial.println("No data available on SoftwareSerial");
  }
  delay(7000); // Delay to prevent rate limit, adjust as needed
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
    delay(5000); // wait 5 seconds
  }
  Serial.println("MQTT Connected!");
}






//#include <SoftwareSerial.h>
//#include <ESP8266WiFi.h>
//#include "Adafruit_MQTT.h"
//#include "Adafruit_MQTT_Client.h"
//
//// Adafruit IO Configurations
//#define AIO_SERVER "io.adafruit.com"
//#define AIO_SERVERPORT 1883 // Use 8883 for SSL
//#define AIO_USERNAME "LucasPilon345"
//#define AIO_KEY "aio_dzCO22N2JFlSRoA2Hvps31f67S3p"
//
//// WiFi Configurations
//const char* WIFI_SSID = "GalaxyS20FE";
//const char* WIFI_PASS = "lucaspilon";
//
//// Create an ESP8266 WiFiClient class to connect to the MQTT server.
//WiFiClient client;
//
//// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
//Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
//
//// Setup feeds
//Adafruit_MQTT_Publish temperature_feed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temperature");
//Adafruit_MQTT_Publish humidity_feed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/humidity");
//
//SoftwareSerial mySerial(D6, D7); // RX, TX
//
//void setup() {
//  // Open serial communications and wait for port to open:
//  Serial.begin(9600);
//  while (!Serial) {
//    ; // wait for serial port to connect. Needed for native USB port only
//  }
//
//  // set the data rate for the SoftwareSerial port
//  mySerial.begin(9600);
//
//  // Convert const char* to char* for WiFi credentials
//  char ssid[sizeof(WIFI_SSID)];
//  char pass[sizeof(WIFI_PASS)];
//  strcpy(ssid, WIFI_SSID);
//  strcpy(pass, WIFI_PASS);
//
//  // Connect to WiFi
//  Serial.println("Connecting to WiFi...");
//  WiFi.begin(ssid, pass);
//
//  int attempts = 0;
//  while (WiFi.status() != WL_CONNECTED && attempts < 20) { // Try for 10 seconds max
//    delay(500);
//    Serial.print(".");
//    attempts++;
//  }
//
//  if (WiFi.status() == WL_CONNECTED) {
//    Serial.println("WiFi connected");
//    Serial.print("IP Address: ");
//    Serial.println(WiFi.localIP());
//  } else {
//    Serial.println("Failed to connect to WiFi");
//  }
//}

//void loop() {
//  MQTT_connect();
//
//  if (mySerial.available()) {
//    String data = mySerial.readStringUntil('\n');
//    Serial.println("Received data: " + data);
//
//    // Parse the data
//    int commaIndex = data.indexOf(',');
//    String humidityStr = data.substring(0, commaIndex);
//    String temperatureStr = data.substring(commaIndex + 1);
//
//    float humidity = humidityStr.toFloat();
//    float temperature = temperatureStr.toFloat();
//
//    // Send data to Adafruit IO
//    if (!humidity_feed.publish(humidity)) {
//      Serial.println("Failed to publish humidity");
//    } else {
//      Serial.println("Humidity published!");
//    }
//
//    if (!temperature_feed.publish(temperature)) {
//      Serial.println("Failed to publish temperature");
//    } else {
//      Serial.println("Temperature published!");
//    }
//  }
//  delay(2000); // Adjust delay as needed
//}
//
//void MQTT_connect() {
//  int8_t ret;
//
//  // Stop if already connected.
//  if (mqtt.connected()) {
//    return;
//  }
//
//  Serial.print("Connecting to MQTT... ");
//
//  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
//    Serial.println(mqtt.connectErrorString(ret));
//    Serial.println("Retrying MQTT connection in 5 seconds...");
//    mqtt.disconnect();
//    delay(5000); // wait 5 seconds
//  }
//  Serial.println("MQTT Connected!");
//}
