import mysql.connector
import paho.mqtt.client as mqtt
import datetime

connection = mysql.connector.connect(
    host="localhost",
    user="Burgaski_Glarusi",
    password="Dildoto_n@_pepelqshk4",
    database="hacktues_practice"
)

broker = "10.210.66.98"
topic = "esp32/pot"

print("Connected!")

def on_message(client, userdata, msg):
    payload = msg.payload.decode()
    print(payload)
    sql = "INSERT INTO pot_values (pot, date_time) VALUES (%s, %s)"
    values = (str(payload), str(datetime.datetime.now()))
    cursor.execute(sql, values)
    connection.commit()
    


cursor = connection.cursor()
cursor.execute("""
CREATE TABLE IF NOT EXISTS pot_values (
    id INT AUTO_INCREMENT PRIMARY KEY,
    pot INT,
    date_time DATETIME
)
""")

client = mqtt.Client()

client.on_message = on_message
client.connect(broker, 1883)
client.subscribe(topic)
client.loop_forever()
