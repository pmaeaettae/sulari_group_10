import tkinter as tk
from tkinter import ttk
import json
import paho.mqtt.client as mqtt

# MQTT settings
MQTT_BROKER = "192.168.1.1"  # Replace with your MQTT broker address
MQTT_PORT = 1883
MQTT_TOPIC = "lasertag/stats"

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

# Start UI
root.mainloop()