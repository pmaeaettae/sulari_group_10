import tkinter as tk
from tkinter import ttk
import json
import paho.mqtt.client as mqtt

# Help found from:
# https://pypi.org/project/paho-mqtt/
# https://docs.python.org/3/library/tkinter.html
# https://www.geeksforgeeks.org/python/python-gui-tkinter/
# https://mosquitto.org/

# MQTT settings
MQTT_BROKER = "localhost"  # Replace with your MQTT broker address
MQTT_PORT = 1883
MQTT_TOPIC = "lasertag/stats"
MQTT_CLIENT_ID = "group10_lasertag_ui"

# Player data
players = {
    "Player 1": {"HP": 3, "KO": 0, "Hits": 0},
    "Player 2": {"HP": 3, "KO": 0, "Hits": 0},
}

# Tkinter setup
root = tk.Tk()
root.title("Laser Tag Scoreboard")
root.geometry("500x300")

title = tk.Label(root, text="Laser Tag Scoreboard", font=("Arial", 20, "bold"))
title.pack(pady=20)

# Player stats display
columns = ["Player", "HP", "KO", "Hits"]
tree = ttk.Treeview(root, columns=columns, show="headings", height=5)

for col in columns:
    tree.heading(col, text=col)
    tree.column(col, width=100)

tree.pack(pady=20)

# Update scoreboard
def refresh_scoreboard():
    for row in tree.get_children():
        tree.delete(row)
    for player, stats in players.items():
        tree.insert("", "end", values=(player, stats["HP"], stats["KO"], stats["Hits"]))

refresh_scoreboard()

# MQTT callbacks
def on_connect(client, userdata, flags, reason_code, properties):
    if reason_code == 0:
        print("Connected to MQTT Broker!")
        client.subscribe(MQTT_TOPIC)
        print(f"Subscribed to topic: {MQTT_TOPIC}")
    else:
        print(f"Failed to connect, reason code {reason_code}")

def on_message(client, userdata, msg):
    payload = msg.payload.decode(errors="replace")
    print(f"Received message: {payload}")

    try:
        data = json.loads(payload)
    except json.JSONDecodeError as exc:
        print(f"Invalid JSON payload: {exc}")
        return

    if "p1" in data:
        players["Player 1"]["HP"] = data["p1"].get("hp", players["Player 1"]["HP"])
        players["Player 1"]["KO"] = data["p1"].get("deaths", players["Player 1"]["KO"])
        players["Player 1"]["Hits"] = data["p1"].get("hits", players["Player 1"]["Hits"])

    if "p2" in data:
        players["Player 2"]["HP"] = data["p2"].get("hp", players["Player 2"]["HP"])
        players["Player 2"]["KO"] = data["p2"].get("deaths", players["Player 2"]["KO"])
        players["Player 2"]["Hits"] = data["p2"].get("hits", players["Player 2"]["Hits"])

    root.after(0, refresh_scoreboard)

# MQTT setup
client = mqtt.Client(client_id=MQTT_CLIENT_ID, protocol=mqtt.MQTTv5)
client.on_connect = on_connect
client.on_message = on_message

client.connect(MQTT_BROKER, MQTT_PORT, 60)

client.loop_start()

# Start UI
root.mainloop()

# Close mqtt
client.loop_stop()
client.disconnect()