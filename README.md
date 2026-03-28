# 🔥 Smart Distributed Fire Detection & Evacuation System (ESP32 + ESP-NOW)

A **multi-node smart fire detection system** built using **ESP32 microcontrollers**, **MQ2 smoke sensors**, **IR flame sensors**, and **ESP-NOW wireless communication**.

The system detects fire conditions in **multiple zones** and communicates wirelessly between nodes. Each node independently detects fire while a central **monitor node (Node-2)** displays the system status on an **OLED screen**.

This project demonstrates a **low-latency distributed safety network** suitable for:

- Smart buildings
- Industrial safety systems
- Warehouses
- Smart campuses
- Fire evacuation research prototypes

---

# 📡 System Architecture

The system contains **4 ESP32 nodes**.

| Node | Role |
|-----|------|
| **Node-1** | Fire detection node (MQ2 + Flame + LEDs + Buzzer) |
| **Node-2** | Fire detection + **Central Monitor (OLED Display)** |
| **Node-3** | Fire detection node (MQ2 + Flame + LEDs + Buzzer) |
| **Node-4** | Fire detection node (MQ2 + Flame + LEDs + Buzzer) |

Each node represents a **zone of a building**.

Zone 1 → Node-1
Zone 2 → Node-2 (Monitor)
Zone 3 → Node-3
Zone 4 → Node-4


All nodes communicate using **ESP-NOW**, a **low-power peer-to-peer wireless protocol** built into ESP32.

---

# 🧠 How the System Works

## 1️⃣ Sensor Monitoring
Each node continuously reads:

- **MQ2 Smoke Sensor**
- **IR Flame Sensor**

Fire is detected when:


Smoke > Threshold AND Flame Sensor = LOW


*(Flame sensor is active-LOW)*

---

## 2️⃣ Local Safety Response

When fire is detected at a node:

- 🔴 Red LEDs turn ON  
- 🔔 Buzzer activates  
- 📡 Node broadcasts alert to all nodes

Each node **reacts only to its own sensors**.

---

## 3️⃣ Wireless Communication (ESP-NOW)

Nodes broadcast status packets every second.

Example packet:

```json
{
  "node": 3,
  "fire": true,
  "sensorFail": false
}

This allows the network to share real-time fire status across all zones.

4️⃣ Central Monitoring (Node-2)

Node-2 receives all packets and updates the OLED screen.

Display examples:

Normal Operation
System Normal
Fire Detected
🔥 FIRE!
Node 4 detected
Sensor Failure
Sensor Fail Node 3
🔌 Hardware Components
Component	Quantity
ESP32 Dev Board	4
MQ2 Smoke Sensor	4
IR Flame Sensor	4
Active Buzzer	4
Red LEDs	8
Green LEDs	8
220Ω Resistors	16
SSD1306 OLED (I2C)	1
Breadboards & wires	—
📍 Pin Configuration

All nodes use the same pins.

Device	ESP32 Pin
MQ2 Analog Output	GPIO 34
Flame Sensor DO	GPIO 35
Buzzer	GPIO 32
Red LED 1	GPIO 25
Red LED 2	GPIO 33
Green LED 1	GPIO 26
Green LED 2	GPIO 27

OLED Display (Node-2 only):

OLED	ESP32
SDA	GPIO 21
SCL	GPIO 22
Address	0x3C
📂 Project Structure
fire-evacuation-esp32/
│
├── node1/
│   └── node1.ino
│
├── node2_monitor/
│   └── node2.ino
│
├── node3/
│   └── node3.ino
│
├── node4/
│   └── node4.ino
│
└── README.md
🚀 Setup Instructions
1️⃣ Install Arduino Libraries

Install these libraries in Arduino IDE:

WiFi
esp_now
Adafruit GFX
Adafruit SSD1306
2️⃣ Find ESP32 MAC Addresses

Upload this code once:

#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  Serial.println(WiFi.macAddress());
}

void loop(){}

Record MAC addresses for all ESP32 boards.

Update them inside the node codes.

3️⃣ Upload Node Programs

Upload the correct sketch to each ESP32:

Board	Code
ESP32 #1	node1
ESP32 #2	node2_monitor
ESP32 #3	node3
ESP32 #4	node4
🧪 Example System Behavior
Fire at Node-3
Node	Response
Node-3	🔴 LEDs + 🔔 Buzzer
Node-1	🟢 Normal
Node-2	OLED shows "FIRE Node 3"
Node-4	🟢 Normal
⚡ Why ESP-NOW?

ESP-NOW provides:

⚡ Ultra-low latency communication
📡 Peer-to-peer networking
🔋 Low power consumption
🌐 No WiFi router required

Perfect for real-time safety systems.

🛠 Future Improvements

Possible extensions:

📱 Mobile alerts via IoT cloud
🗺 Fire zone mapping
🚪 Smart evacuation route guidance
📊 Fire analytics dashboard
🔔 Central alarm siren
👨‍💻 Author

Ayush Kumar

Engineering Project – Smart Safety Systems

📜 License

This project is open source and available under the MIT License.