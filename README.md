# sulari_group_10
Source code for our laser tag system project

## Function descriptions

### main.ino

- `setup()`: Initializes serial output, GPIO, LEDs, display, IR, gyro, wireless communication, and game data.
- `loop()`: Runs the main state machine, updates LEDs and MQTT, and moves between waiting, play, game over, and debug states.

### game

- `wait_for_respawn()`: Internal helper that waits until a knocked-out player stays still for the full respawn cooldown.
- `game_init()`: Sets the initial IDs, health, deaths, hits, ammo, and shot timer values.
- `game_start()`: Resets both players for a new match and updates the status LEDs.
- `game_update()`: Handles incoming hits, display updates, shooting, ammo use, reloads, and MQTT stat updates during play.
- `game_handle_hit(int player_id)`: Applies damage and hit counts based on the shooter, triggers feedback, and respawns defeated players.
- `game_over()`: Checks whether either player has reached the maximum number of deaths.
- `game_over_stats()`: Declared in the header for game-over statistics, but not currently implemented.
- `debug_mode()`: Shows the debug mode message on the display.

### display

- `display_start()`: Initializes the OLED display and shows the waiting message.
- `display_update(int ammo1, int hp1, int ammo2, int hp2)`: Draws both players' ammo and health values on the OLED.
- `display_show(const char* msg)`: Clears the OLED and displays a single message.

### gpio

- `gpio_init()`: Configures button, switch, motor, buzzer, and LED pins.
- `trigger_pressed()`: Returns whether the trigger button is currently pressed.
- `reload_pressed()`: Returns whether the reload button is currently pressed.
- `start_button_pressed()`: Returns whether the start button is currently pressed.
- `debug_mode_active()`: Returns whether trigger and reload are held together to enter debug mode.
- `player_id()`: Reads the player selector switch and returns the active player ID.
- `status_leds()`: Updates the status LEDs to show the selected player and debug state.
- `rumble_motor_on()`: Turns the rumble motor on.
- `rumble_motor_off()`: Turns the rumble motor off.
- `buzzer(int buzz_duration)`: Turns the buzzer on for the requested duration.

### gyro

- `mpu_init()`: Initializes the MPU6050 accelerometer on the I2C bus.
- `mpu_stationary()`: Returns whether the accelerometer movement change is below the stationary threshold.

### ir

- `ir_rx_interrupt()`: Interrupt handler that measures IR pulse widths and stores valid received pulses.
- `ir_init()`: Configures the IR receiver, transmitter PWM, and receiver interrupt.
- `ir_pwm_burst(int duration)`: Internal helper that sends a 38 kHz IR burst for the requested duration.
- `ir_shoot(int player_id)`: Sends a header pulse and a player-specific IR pulse.
- `ir_pulse_received()`: Returns whether an unread IR pulse has been received.
- `ir_read_pulse_width()`: Returns the latest pulse width and clears the received flag.
- `ir_get_player_id()`: Decodes the latest pulse width into a player ID and filters repeated or invalid hits.

### wireless

- `wireless_init()`: Connects to WiFi and configures the MQTT broker connection.
- `mqtt_loop()`: Keeps the MQTT client connected and processes MQTT traffic.
- `mqtt_stats()`: Publishes both players' current stats as a JSON MQTT message.
- `mqtt_game_over()`: Publishes the game-over event and final stats over MQTT.
- `mqtt_reconnect()`: Attempts a non-blocking MQTT reconnect every five seconds.

### scoreboard_ui.py

- `refresh_scoreboard()`: Rebuilds the scoreboard table from the current player data.
- `on_connect(client, userdata, flags, reason_code, properties)`: Subscribes to the stats topic after a successful MQTT connection.
- `on_message(client, userdata, msg)`: Parses incoming stats messages and updates the displayed player data.
