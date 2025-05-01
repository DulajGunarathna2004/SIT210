import paho.mqtt.client as mqtt
import RPi.GPIO as GPIO
import time
import sys

# MQTT Configuration
BROKER = "broker.emqx.io"
PORT = 1883
TOPIC_WAVE = "SIT210/wave10"
TOPIC_PAT = "SIT210/pat"

# GPIO Setup
LED_PIN = 19
GPIO.setmode(GPIO.BCM)
GPIO.setup(LED_PIN, GPIO.OUT)

# MQTT: Message handler
def on_message(client, userdata, msg):
    message = msg.payload.decode("utf-8")
    print("Received:", message)

    if message.strip() == "DulajWave":
        print("Wave detected. Blinking LED 3 times...")
        for _ in range(3):
            GPIO.output(LED_PIN, GPIO.HIGH)
            time.sleep(0.5)
            GPIO.output(LED_PIN, GPIO.LOW)
            time.sleep(0.5)

    elif message.strip() == "DulajPat":
        print("Pat detected. Blinking LED 4 times...")
        for _ in range(4):
            GPIO.output(LED_PIN, GPIO.HIGH)
            time.sleep(1)
            GPIO.output(LED_PIN, GPIO.LOW)
            time.sleep(1)

# MQTT: Connection handler
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected to MQTT broker.")
        client.subscribe(TOPIC_WAVE)
        client.subscribe(TOPIC_PAT)
    else:
        print(f"Failed to connect to MQTT broker. Return code: {rc}")
        sys.exit(1)

# MQTT Client setup
client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

# Main loop
try:
    print("Connecting to MQTT broker...")
    client.connect(BROKER, PORT, 60)
    client.loop_forever()
except KeyboardInterrupt:
    print("\nProgram interrupted by user.")
except Exception as e:
    print(f"An error occurred: {e}")
finally:
    GPIO.cleanup()
    client.disconnect()
    print("Cleaned up GPIO and disconnected MQTT client.")
