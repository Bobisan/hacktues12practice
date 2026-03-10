#include <WiFi.h>
#include <PubSubClient.h>

// WiFi credentials
const char* ssid = "Na bobcho toploto";
const char* password = "pey8jk38qvr2dj4";

// MQTT broker
const char* mqtt_server = "10.210.66.98";

// Topics
const char* potTopic = "esp32/pot";
const char* ledTopic = "esp32/ledfreq";

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastPotSend = 0;
unsigned long lastBlink = 0;

int blinkFrequency = 0;
bool ledState = false;

// ---------------- WIFI ----------------
void setup_wifi() {
  delay(10);

  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected");
}

// ---------------- MQTT CALLBACK ----------------
void callback(char* topic, byte* payload, unsigned int length) {

  String message = "";

  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  if (String(topic) == ledTopic) {
    blinkFrequency = message.toInt();

    Serial.print("New blink frequency received: ");
    Serial.print(blinkFrequency);
    Serial.println(" Hz");
  }
}

// ---------------- MQTT RECONNECT ----------------
void reconnect() {

  while (!client.connected()) {

    Serial.print("Connecting to MQTT...");

    if (client.connect("ESP32TestClient")) {

      Serial.println("connected");

      client.subscribe(ledTopic);
      Serial.println("Subscribed to LED topic");

    } else {

      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" retry in 2 seconds");

      delay(2000);
    }
  }
}

// ---------------- SETUP ----------------
void setup() {

  Serial.begin(115200);

  randomSeed(analogRead(0));

  setup_wifi();

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  Serial.println("ESP32 MQTT Simulation Started");
}

// ---------------- LOOP ----------------
void loop() {

  if (!client.connected()) {
    reconnect();
  }

  client.loop();

  unsigned long now = millis();

  // ---- Simulated potentiometer every 10 seconds ----
  if (now - lastPotSend >= 10000) {

    int simulatedPot = random(0, 1024);

    char msg[10];
    sprintf(msg, "%d", simulatedPot);

    client.publish(potTopic, msg);

    Serial.print("Published simulated pot value: ");
    Serial.println(simulatedPot);

    lastPotSend = now;
  }

  // ---- Simulated LED blinking ----
  if (blinkFrequency == 0) {

    if (ledState != false) {
      ledState = false;
      Serial.println("LED OFF");
    }

  } else {

    int interval = 1000 / (blinkFrequency);

    if (now - lastBlink >= interval) {

      ledState = !ledState;

      if (ledState) {
        Serial.print("LED ON at: ");
      } else {
        Serial.print("LED OFF at: ");
      }
      Serial.print(blinkFrequency);
      Serial.println(" Hz");

      lastBlink = now;
    }
  }
}
