# PIC24-Powered 4WD Obstacle Avoidance Vehicle

A dual-mode, embedded system built on the Microchip PIC24 microcontroller architecture. The system features a custom-built physical remote controller that transmits manual driving commands via Bluetooth, paired with a deterministic, real-time ultrasonic sensor priority safety override circuit to mitigate crashes.

## 🚀 System Architecture & Modes

1. **Manual Mode:** The operator drives the vehicle freely using a Bluetooth-connected analog joystick controller.
2. **Autonomous Override Mode:** The ultrasonic sensor continuously runs via low-level timers and input capture in the background. If an obstacle is detected within a 3-inch critical zone, the core firmware temporarily sequesters control from the remote, executes a hard-coded evasive maneuver (stop, reverse, turn, re-scan), and safely relinquishes control back to the operator once the path is clear.

---

## 🛠️ Hardware Bill of Materials

| Component | Function | Datasheet |
| :--- | :--- | :--- |
| **PIC24FJ64GA002** | 16-bit Microcontroller (Core CPU) | [Datasheet](https://ww1.microchip.com/downloads/en/devicedoc/39881e.pdf) |
| **TB6612FNG** | Dual DC Motor Driver IC | [Datasheet](https://toshiba.semicon-storage.com/info/TB6612FNG_datasheet_en_20141001.pdf?did=10660&prodName=TB6612FNG) |
| **HC-05 / HC-06** | Bluetooth Modules (Master/Slave pair) | [Datasheet](https://engineering.purdue.edu/477grp14/Team/journal/img%20-%20member3/documents/HC05.pdf) |
| **HC-SR04** | Ultrasonic Distance Proximity Sensor | [Datasheet](https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf) |
| **KY-023** | Dual-Axis Analog Joystick Module | [Datasheet](https://naylampmechatronics.com/img/cms/Datasheets/000036%20-%20datasheet%20KY-023-Joy-IT.pdf) |
| **MG90S** | Micro Servo / Four DC Geared Motors | [Datasheet](https://components101.com/sites/default/files/component_datasheet/MG90S-Datasheet.pdf) |
| **KCD3** | Rocker Switch (Power isolation) | [Datasheet](https://www.handsontec.com/dataspecs/switches/KCD3-Rocker%20SW.pdf) |

---

## 💻 Firmware Architecture & API Reference

The project is split into granular hardware-abstraction drivers to align with safety-critical firmware engineering practices.

### 1. Ultrasonic Subsystem (`ultrasonic.c`)
Handles microsecond-level timing via native peripheral hardware without blocking the CPU.
* `void triggerInput()`: Transmits the initial 10µs trigger pulse sequence to the HC-SR04.
* `void initEchoCapture()`: Configures **Timer 2** and **Input Capture 1 (IC1)** to timestamp rising and falling edges of the incoming echo pulse.
* `int Obstacle_Detection(void)`: Evaluates sensor data and returns a safety flag boolean (`1` for hazard, `0` for clear).
* `void Ultrasonic_Reset(void)`: Resets state variables and buffer configurations back to baseline.

### 2. Motor Actuation (`motor.c`)
Manages physical movement vectors and variable speed configuration.
* `void motor_init(void)`: Initializes Timer 3 and registers Pulse Width Modulation (PWM) channels to target pins via **Peripheral Pin Select (PPS)**.
* `void set_car_speed(int left_speed, int right_speed)`: Assigns explicit duty cycles to **Output Compare channels (OC1/OC2)**.
* `void car_forward(int speed)` / `car_backward(int speed)`: Sets precise logic gates on I/O lines `RB0`-`RB3` to drive H-Bridge directions.

### 3. UART & Bluetooth Communications (`bluetooth.c`)
Ensures robust telemetry and parsing using an asynchronous, interrupt-driven model.
* `void bluetooth_setup(void)`: Initializes the hardware UART module at the designated baud rate.
* `int uart1_available(void)`: Polls the receive line to verify safe, uncorrupted data in the queue.
* `unsigned char uart1_getc(void)`: Pops an incoming character command string from a software-implemented **Circular Buffer** to prevent data drops during CPU-intensive loops.

---

## 🔒 Deterministic Priority-Based Safety Override
To guarantee safety in automated environments, user inputs can become dangerous if a communication delay or driver error occurs near a hazard. 

This firmware solves this with a **Strict Command Lock State Machine**:
1. At the start of the primary processing loop execution, `obstacle_scan()` handles distances.
2. If an obstacle is detected within the hazard perimeter, the loop bypasses the manual `Move_cmd` instruction parsing block entirely.
3. The global manual command variable `D` is temporarily sandboxed and overwritten to an explicit `S` (Stop) state.
4. Autonomous evasion takes over until the sensor clears, shielding the vehicle from late-arriving or buffered user "Forward" inputs that would cause a crash.
