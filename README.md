obs: 

é um trabalho feito com edge impulse, windows e arduino ide

link para o projeto no edge impulse: https://studio.edgeimpulse.com/public/841193/live
link para configuração do esp32 para o edge impulse pra caso a pessoa deseje fazer testes em tempo real: https://docs.edgeimpulse.com/hardware/boards/espressif-esp32
link de um tutorial de como funciona o sensor de ecg e como funciona a ecg num geral: https://learn.sparkfun.com/tutorials/ad8232-heart-rate-monitor-hookup-guide/all
link para baixar o dataset usado: https://www.kaggle.com/datasets/shayanfazeli/heartbeat?resource=download

Diante da necessidade de identificar anomalias cardíacas com rapidez e precisão em ambientes clínicos e de baixo recurso, surgiu a proposta de desenvolver um sistema portátil e eficiente capaz de detectar irregularidades no ECG diretamente no dispositivo, utilizando Inteligência Artificial embarcada (IA na borda).

objetivos: Monitorar batimentos cardíacos e Detectar e classificar anomalias utilizando IA na borda

metodologia: Foi desenvolvido, via Edge Impulse, um modelo de IA otimizado para execução no ESP32, permitindo realizar a inferência diretamente no dispositivo de borda.
1. upload do dataset
2. treinamento da ia
3. testes

como funciona ecg:
ECG é o sinal elétrico, gerado pela despolarização e repolarização do músculo cardíaco
A variação elétrica do coração se espalha pelo corpo inteiro (água e eletrólitos)
Os sensores de ECG medem a diferença de tensão na pele.

Complexo QRS

<img width="581" height="455" alt="image" src="https://github.com/user-attachments/assets/9f670531-917c-48fd-b933-45c61eaac8c2" />

sobre o dataset:
109446 amostras a 125Hz
5 classificações diferentes: Fusion, Normal, Supraventricular, Unknown, Ventricular
187 colunas de ECG x 1 coluna de classificação
1,5 segundos por linha / amostra

sobre a ia:
A IA foi treinada com ECG filtrado, extraindo espectrograma e features espectrais. O modelo CNN-1D foi treinado com Adam, batches de 64, early stopping e validação separada, aprendendo a classificar 5 tipos de batimentos (tem o arquivo python com o codigo de treinamento usado no edge impulse

Acurácia do modelo

<img width="491" height="121" alt="image" src="https://github.com/user-attachments/assets/4132c486-8c04-4911-baa6-b62a8f34f2ed" />

como rodar o modelo? voce pode checar se os sensores estao funcionando com o esp32_testing_ecg_sensor
primeiro clone o projeto, dps configure o esp32 pra rodar no ei, dps rode o codigo esp32_ecg_simulator no esp32, dps va para live classification no edge impulse e faça o sampling de 1.5s a 125Hz (ecplique qual o comando no terminal que precisa dar pra fazer os testes no live classification

ou conecta o esp32 no edge impulse e abre o serial monitor no arduino ide e faz os testes por la

exemplo testando no edge impulse
<img width="805" height="801" alt="image" src="https://github.com/user-attachments/assets/3f82b800-dbb1-4f9c-b39f-5e9c9f92e347" />

exemplos testando direto no serial monitor
<img width="556" height="317" alt="image" src="https://github.com/user-attachments/assets/215b9a0d-1f0e-4171-9208-2219705ccbe2" />
<img width="565" height="297" alt="image" src="https://github.com/user-attachments/assets/765c6788-19ba-4136-871b-aa26c10200c9" />


fotinha do ecg aparecendo no serial plotter arduino ide
<img width="622" height="552" alt="image" src="https://github.com/user-attachments/assets/7fb7b2db-6d4b-45d8-8870-22774a41d792" />


# Heart Anomaly Detection Using Edge AI


---

## About the Project


---

## Technologies Used


---

## Prerequisites


---

## Hardware Setup

### Components

**ESP32 Microcontroller:** The main processing unit running Zephyr RTOS. Handles data acquisition, signal processing, and communication.

**AD8232 ECG Signal Conditioning Amplifier:** A specialized integrated circuit designed for biomedical signal acquisition. It:
- Amplifies weak ECG signals from body electrodes
- Removes DC offset and 60/50 Hz noise
- Provides a high-quality analog signal suitable for conversion
- Includes lead-off detection to identify poor electrode contact

**ADS1115 ADC (Analog-to-Digital Converter):** A high-precision 16-bit I2C ADC that:
- Converts the analog signal from the AD8232 to digital format
- Provides programmable gain amplification
- Supports 4 differential or single-ended input channels
- Communicates with the ESP32 via I2C protocol
- Allows multiple devices on the same I2C bus

---

### Connection Diagram 

<img width="804" height="992" alt="hardware_ecg drawio" src="https://github.com/user-attachments/assets/dd3ed273-2a57-4c54-9187-514f64c4022d" />

#### ADS1115 Connections

| Component  | Module Pin     | Connects to   | Function                |
| ---------- | -------------- | ------------- | ----------------------- |
| ADS1115    | SDA            | GPIO 21 ESP32 | I²C – Dados             |
| ADS1115    | SCL            | GPIO 22 ESP32 | I²C – Clock             |
| ADS1115    | ADDR           | GND ESP32     | Ground Connection       |
| ADS1115    | A0             | OUTPUT AD8232 | Analogic Input (ECG)    |
| ADS1115    | ADDR           | GND ESP32     | Ground Connection       |
| ADS1115    | GND            | GND ESP32     | Ground Connection       |
| ADS1115    | VCC            | 3V3 ESP32     | Power Connection        |

#### AD8232 Connections

| Component  | Module Pin     | Connects to   | Function            |
| ---------- | -------------- | ------------- | ------------------- |
| AD8232     | LO+            | GPIO 26 ESP32 | Lead-off positive   |
| AD8232     | LO-            | GPIO 27 ESP32 | Lead-off negative   |
| AD8232     | GND            | GND ESP32     | Ground Connection   |
| AD8232     | VCC            | 3V3 ESP32     | Power Connection    |

---

### Electrode Placement

<img width="434" height="399" alt="image" src="https://github.com/user-attachments/assets/26760abb-d10c-425c-914e-8ffe4258ecb2" />

Ensure good skin contact for accurate signal acquisition.

---

## Features


---

## Authors

Débora Castro: https://github.com/cyberdebb/
Matheus Francisco: https://github.com/matheus1103

---

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for more details.
