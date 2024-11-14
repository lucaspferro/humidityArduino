//////Libraries
//#include <dht.h>
//dht DHT;
//
////Constants
//#define DHT22_PIN 2     // DHT 22  (AM2302) - what pin we're connected to
//const int RelePin = 9; // pino ao qual o Módulo Relé está conectado
//
//#include <Wire.h>
//#include <Adafruit_GFX.h>
//#include <Adafruit_SSD1306.h>
//#include <dht.h>
//#include <SoftwareSerial.h>
//
//SoftwareSerial mySerial(A3, A2); // RX, TX
//
//
//#define SCREEN_WIDTH 128 // OLED display width, in pixels
//#define SCREEN_HEIGHT 64 // OLED display height, in pixels
//
//// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
//#define OLED_RESET    -1 // Reset pin # (or -1 if sharing Arduino reset pin)
//Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
//
////dht DHT;
////#define DHT22_PIN 2     // DHT 22  (AM2302) - what pin we're connected to
////const int RelePin = 9; // pino ao qual o Módulo Relé está conectado
//
//float hum;  // Stores humidity value
//float temp; // Stores temperature value
//
//void setup() {
//        // Open serial communications and wait for port to open:
//    Serial.begin(9600);
//    while (!Serial) {
//      ; // wait for serial port to connect. Needed for native USB port only
//    }
//    
//    pinMode(RelePin, OUTPUT); // seta o pino como saída
//    digitalWrite(RelePin, LOW); // seta o pino com nível lógico baixo
//    
//    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
//    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x64
//        Serial.println(F("SSD1306 allocation failed"));
//        for(;;);
//    }
//    display.display();
//    delay(700); // Pause for 2 seconds
//    display.clearDisplay();
//}
//
//void loop() {
//    int chk = DHT.read22(DHT22_PIN);
//    hum = DHT.humidity;
//    temp = DHT.temperature;
//
//    // Clear the buffer
//    display.clearDisplay();
//    
//
//    
//    // Display Humidity
//    display.setTextSize(1);
//    display.setTextColor(SSD1306_WHITE); // Draw white text
//    display.setCursor(0,0);     // Start at top-left corner
//    display.print("Umidade: ");
//    display.print(hum);
//    Serial.println(hum);
//    display.print(" %");
//
//
//    // Display Temperature
//    display.setCursor(0, 20);
//    display.print("Temperatura: ");
//    display.print(temp);
//    Serial.println(temp);
//    display.print(" C");
//
//    
//    if (hum < 80) {
//      display.setCursor(0,35);
//        display.print("Rele ON ");
//    } else {
//      display.setCursor(0,35);
//         display.print("Rele OFF ");
//    }
//    
//    display.display();
//    
//    if (hum < 80) {
//        digitalWrite(RelePin, LOW); // Aciona o pino
//        Serial.println("Relay ON (Humidity < 80)");
//        display.print("Relay ON ");
//    } else {
//        digitalWrite(RelePin, HIGH); // Desaciona o pino
//        Serial.println("Relay OFF (Humidity >= 80)");
//         display.print("Relay OFF ");
//    }
//
//    delay(700); // Delay 2 sec.
//}


#include <dht.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>

dht DHT;

// Definições
#define DHT22_PIN 2     // Pino conectado ao sensor DHT22 (AM2302)
const int RelePin = 9;  // Pino conectado ao módulo Relé

#define SCREEN_WIDTH 128 // Largura do display OLED, em pixels
#define SCREEN_HEIGHT 64 // Altura do display OLED, em pixels

// Definição para um display SSD1306 conectado ao I2C (pinos SDA, SCL)
#define OLED_RESET -1 // Pino de reset (ou -1 se compartilhando o reset do Arduino)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

SoftwareSerial mySerial(A3, A2); // RX, TX

float hum;  // Armazena o valor da umidade
float temp; // Armazena o valor da temperatura

void setup() {
    // Inicia a comunicação serial
    Serial.begin(115200);
    while (!Serial) {
        ; // Aguarda a conexão da porta serial (necessário para porta USB nativa)
    }

    mySerial.begin(115200); // Inicia a SoftwareSerial

    pinMode(RelePin, OUTPUT); // Define o pino do relé como saída
    digitalWrite(RelePin, LOW); // Define o pino do relé como nível lógico baixo

    // Inicia o display OLED
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Endereço 0x3C para 128x64
        Serial.println(F("Falha na alocação do SSD1306"));
        for (;;) ;
    }
    display.display();
    delay(700); // Pausa de 0.7 segundos
    display.clearDisplay();
}

void loop() {
    int chk = DHT.read22(DHT22_PIN);
    hum = DHT.humidity;
    temp = DHT.temperature;

    // Limpa o buffer do display
    display.clearDisplay();

    // Exibe a umidade
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE); // Texto na cor branca
    display.setCursor(0, 0); // Começa no canto superior esquerdo
    display.print("Umidade: ");
    display.print(hum);
    display.print(" %");

    // Exibe a temperatura
    display.setCursor(0, 20);
    display.print("Temperatura: ");
    display.print(temp);
    display.print(" C");

    if (hum < 80) {
        display.setCursor(0, 35);
        display.print("Rele ON ");
    } else {
        display.setCursor(0, 35);
        display.print("Rele OFF ");
    }

    display.display();

    if (hum < 80) {
        digitalWrite(RelePin, LOW); // Aciona o pino
        Serial.println("Relay ON (Humidity < 80)");
    } else {
        digitalWrite(RelePin, HIGH); // Desaciona o pino
        Serial.println("Relay OFF (Humidity >= 80)");
    }

    // Envia os dados via SoftwareSerial
    mySerial.print(hum);
    mySerial.print(",");
    mySerial.println(temp);

    delay(900); // Delay de 2 segundos
}
