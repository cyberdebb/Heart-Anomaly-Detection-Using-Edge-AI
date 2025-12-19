# Heart Anomaly Detection Using Edge AI

This project implements a portable and efficient system for detecting and classifying heart anomalies using embedded Artificial Intelligence (Edge AI). The system uses an ESP32 microcontroller to run a Convolutional Neural Network (CNN) model, enabling real-time ECG signal analysis directly on the edge device.

The AI model is trained to classify heartbeats into five categories based on the MIT-BIH Arrhythmia Dataset:
*   Normal
*   Supraventricular Ectopic Beat
*   Ventricular Ectopic Beat
*   Fusion Beat
*   Unknown Beat

Project developed for a college assignment by [Débora Castro](https://github.com/cyberdebb/) and [Matheus Francisco](https://github.com/matheus1103).

**Public Edge Impulse Project:** [https://studio.edgeimpulse.com/public/841193/live](https://studio.edgeimpulse.com/public/841193/live)

---

## Table of Contents
- [Features](#features)
- [Technologies Used](#technologies-used)
- [AI Model](#ai-model)
- [Hardware Setup](#hardware-setup)
- [How to Use](#how-to-use)
- [Project Structure](#project-structure)
- [License](#license)

---

## Features
- **Real-time Classification:** Detects and classifies heart anomalies directly on the ESP32.
- **5 Anomaly Classes:** Identifies Normal, Supraventricular, Ventricular, Fusion, and Unknown heartbeats.
- **Low Power, Low Latency:** Optimized for edge devices, providing quick results without cloud dependency.
- **ECG Simulator:** Includes an Arduino sketch ([`esp32_ecg_simulator.ino`](esp32_ecg_simulator/esp32_ecg_simulator.ino)) to test the model using pre-recorded samples from the MIT-BIH dataset.
- **Sensor Test Utility:** A separate sketch ([`esp32_testing_ecg_sensor.ino`](esp32_testing_ecg_sensor/esp32_testing_ecg_sensor.ino)) to verify the functionality of the AD8232 ECG sensor.

---

## Technologies Used
- **Hardware:**
    - ESP32 Microcontroller
    - AD8232 ECG Signal Conditioning Amplifier
    - ADS1115 16-bit ADC
- **Software & Services:**
    - [Edge Impulse](https://www.edgeimpulse.com/) for model training and deployment.
    - Arduino IDE for programming the ESP32.
    - Python with TensorFlow/Keras for model definition.

---

## AI Model
The model is a lightweight 1D Convolutional Neural Network (CNN) designed for efficiency on microcontrollers. The architecture is defined in [`edge_ai_training.py`](edge_ai_training.py).

- **Input:** 187 data points per sample (1.5 seconds of ECG data at 125Hz).
- **Architecture:** Consists of three `Conv1D` blocks with `ReLU` activation and `MaxPooling1D`, followed by a `Flatten` layer and a `Dense` classifier.
- **Training:** The model was trained using the Adam optimizer, a batch size of 64, and an early stopping callback to prevent overfitting.

![Model Accuracy](https://github.com/user-attachments/assets/4132c486-8c04-4911-baa6-b62a8f34f2ed)

---

## Hardware Setup

### Components
- **ESP32 Microcontroller:** The main processing unit.
- **AD8232 ECG Amplifier:** Amplifies and filters the raw ECG signal.
- **ADS1115 ADC:** Converts the analog signal from the AD8232 to a digital format for the ESP32.

### Connection Diagram
![Hardware Diagram](https://github.com/user-attachments/assets/dd3ed273-2a57-4c54-9187-514f64c4022d)

| Module | Pin   | Connects to   |
|--------|-------|---------------|
| **ADS1115** | VCC   | 3V3 ESP32     |
|        | GND   | GND ESP32     |
|        | SCL   | GPIO 22 ESP32 |
|        | SDA   | GPIO 21 ESP32 |
|        | A0    | OUTPUT AD8232 |
| **AD8232**  | VCC   | 3V3 ESP32     |
|        | GND   | GND ESP32     |
|        | LO+   | GPIO 26 ESP32 |
|        | LO-   | GPIO 27 ESP32 |

### Electrode Placement
For best results, place the ECG electrodes as shown below. Ensure good skin contact.

![Electrode Placement](https://github.com/user-attachments/assets/26760abb-d10c-425c-914e-8ffe4258ecb2)

---

## How to Use

### 1. Clone the Repository
```sh
git clone https://github.com/cyberdebb/Heart-Anomaly-Detection-Using-Edge-AI.git
cd Heart-Anomaly-Detection-Using-Edge-AI
```

### 2. Test the Model with the ECG Simulator
The simulator allows you to test the deployed model without needing the physical sensors. It uses pre-recorded data from the [`ecg_samples_10each.h`](esp32_ecg_simulator/ecg_samples_10each.h) file.

1.  Open [`esp32_ecg_simulator/esp32_ecg_simulator.ino`](esp32_ecg_simulator/esp32_ecg_simulator.ino) in the Arduino IDE.
2.  Select your ESP32 board and port.
3.  Upload the sketch to your ESP32.
4.  Open the Serial Monitor at **115200 baud**.
5.  Use the following commands to run inference:
    - `A`: Test a random sample from all classes.
    - `T`: Test all 50 available samples sequentially and calculate accuracy.
    - `0`-`9`: Test a specific "Normal" sample.

### 3. Test the ECG Sensor
Before running a full real-time inference, you can verify that your AD8232 sensor is working correctly.

1.  Open [`esp32_testing_ecg_sensor/esp32_testing_ecg_sensor.ino`](esp32_testing_ecg_sensor/esp32_testing_ecg_sensor.ino) in the Arduino IDE.
2.  Upload the sketch to your ESP32.
3.  Open the **Serial Plotter** (Tools -> Serial Plotter) to visualize your live heartbeat data.

![Serial Plotter ECG](https://github.com/user-attachments/assets/7fb7b2db-6d4b-45d8-8870-22774a41d792)

---

## Project Structure
```
.
├── edge_ai_training.py             # Python script for model training in Edge Impulse
├── esp32_ecg_simulator/
│   ├── esp32_ecg_simulator.ino     # Arduino sketch to simulate ECG signals and test the model
│   └── ecg_samples_10each.h        # Header with 50 pre-recorded ECG samples
├── esp32_testing_ecg_sensor/
│   └── esp32_testing_ecg_sensor.ino  # Arduino sketch to test the AD8232 sensor
├── LICENSE                         # Project License
└── README.md                       # This file
```

---

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
