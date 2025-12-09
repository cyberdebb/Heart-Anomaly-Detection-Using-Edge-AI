#include <Wire.h>
#include <Adafruit_ADS1X15.h>

Adafruit_ADS1115 ads;

// Pinos do ESP32
#define LO_PLUS_PIN 26
#define LO_MINUS_PIN 27

// Ganho do ADS1115 (ajuste se quiser)
adsGain_t gain = GAIN_ONE;   // ±4.096V
// Data rate do ADS1115: 128 SPS (próximo de 125 Hz)
int dataRate = 128;

void setup() {
  Serial.begin(115200);
  // Inicia I2C no ESP32 (SDA=21, SCL=22)
  Wire.begin(21, 22);

  if (!ads.begin()) {
    // Se falhar, fica preso — não imprimir nada (Edge Impulse precisa de saída limpa)
    while (1) delay(100);
  }

  ads.setGain(gain);
  ads.setDataRate(dataRate);

  pinMode(LO_PLUS_PIN, INPUT);
  pinMode(LO_MINUS_PIN, INPUT);

  // pequena espera para estabilizar
  delay(1000);
}

void loop() {
  // Se algum eletrodo estiver solto, envie 0.0 (mantém formato EI)
  bool loPlus = digitalRead(LO_PLUS_PIN);
  bool loMinus = digitalRead(LO_MINUS_PIN);

  if (loPlus == 1 || loMinus == 1) {
    Serial.println("0.0");
    // espere o tempo de amostragem para não inundar a serial
    delayMicroseconds(8000);
    return;
  }

  // Lê do ADS1115, canal A0
  int16_t raw = ads.readADC_SingleEnded(0);

  // Calcula Full Scale Range de acordo com o ganho
  float fsr;
  switch (gain) {
    case GAIN_TWOTHIRDS: fsr = 6.144; break;
    case GAIN_ONE:       fsr = 4.096; break;
    case GAIN_TWO:       fsr = 2.048; break;
    case GAIN_FOUR:      fsr = 1.024; break;
    case GAIN_EIGHT:     fsr = 0.512; break;
    case GAIN_SIXTEEN:   fsr = 0.256; break;
    default:             fsr = 4.096; break;
  }

  // Converte para volts
  float voltage = (raw / 32767.0f) * fsr;

  // Envia somente o número por linha (requisito Edge Impulse)
  Serial.println(voltage, 6);

  // Temporização: ~125 Hz => 8 ms por amostra
  // ADS1115 em 128 SPS leva ~7.8 ms por conversão; adicionamos ~8000 us para
  // aproximar 125 Hz no envio.
  delayMicroseconds(8000);
}