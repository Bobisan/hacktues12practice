#include <WiFi.h>
#include <PubSubClient.h>

// WiFi credentials
const char* ssid = "YOUR_WIFI";
const char* password = "YOUR_PASSWORD";

// MQTT server
const char* mqtt_server = "192.168.1.100";   // change to your broker IP

WiFiClient espClient;
PubSubClient client(espClient);

// Pins
const int potPin = 34;
const int ledPin = 2;

// MQTT topics
const char* potTopic = "esp32/pot";
const char* ledTopic = "esp32/ledfreq";

unsigned long lastPotSend = 0;
unsigned long lastBlink = 0;

int blinkFrequency = 0;
bool ledState = false;

void setup_wifi() {
  delay(10);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void callback(char* topic, byte* payload, unsigned int length) {

  String message = "";

  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  if (String(topic) == ledTopic) {
    blinkFrequency = message.toInt();
  }
}

void reconnect() {

  while (!client.connected()) {

    if (client.connect("ESP32Client")) {

      client.subscribe(ledTopic);

    } else {
      delay(2000);
    }
  }
}

void setup() {

  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);

  setup_wifi();

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }

  client.loop();

  unsigned long now = millis();

  // Send potentiometer value every 10 seconds
  if (now - lastPotSend > 10000) {

    int potValue = analogRead(potPin);

    char msg[10];
    sprintf(msg, "%d", potValue);

    client.publish(potTopic, msg);

    lastPotSend = now;
  }

  // LED behavior
  if (blinkFrequency == 0) {

    digitalWrite(ledPin, LOW);

  } else {

    int interval = 1000 / blinkFrequency;

    if (now - lastBlink > interval) {

      ledState = !ledState;
      digitalWrite(ledPin, ledState);

      lastBlink = now;
    }
  }
}
