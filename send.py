import sys
import paho.mqtt.client as mqtt

broker = "10.210.66.98"
topic = "esp32/ledfreq"

client = mqtt.Client()

client.connect(broker, 1883)

freq = sys.argv[1]
client.publish(topic, freq)

client.disconnect()