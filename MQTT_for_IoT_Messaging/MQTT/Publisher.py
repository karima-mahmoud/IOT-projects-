import paho.mqtt.client as mqtt


broker_ip = "192.168.1.9" 
topic = "home/led"

client = mqtt.Client("Python_Control_App")
client.connect(broker_ip, 1883)

print("System Ready! Type ON to light up or OFF to turn off.")

while True:
    cmd = input("Enter Command: ").upper()
    if cmd in ["ON", "OFF"]:
        client.publish(topic, cmd)
        print(f"Sent {cmd} to NodeMCU")