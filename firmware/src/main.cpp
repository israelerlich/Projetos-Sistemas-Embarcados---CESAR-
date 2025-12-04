#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

// Pinos do seu hardware (NÚMERO DE GPIO)
#define RELAY_PIN 2   // Relé no GPIO2 (pin 4 físico da placa)
#define SOIL_PIN  4   // Sensor de umidade no GPIO4 (pin 5 físico, ADC)

// Tempos em ms
const TickType_t RELAY_HALF_PERIOD = pdMS_TO_TICKS(2500); // 2,5 s ON + 2,5 s OFF = ciclo de 5 s
const TickType_t SENSOR_PERIOD     = pdMS_TO_TICKS(5000); // 5 s entre leituras

// Task que controla o relé
void taskRelay(void *pvParameters) {
  (void) pvParameters;

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // começa desligado

  for (;;) {
    digitalWrite(RELAY_PIN, HIGH);
    Serial.println("[RELAY] ON");
    vTaskDelay(RELAY_HALF_PERIOD);

    digitalWrite(RELAY_PIN, LOW);
    Serial.println("[RELAY] OFF");
    vTaskDelay(RELAY_HALF_PERIOD);
  }
}

// Task que lê o sensor de umidade e imprime o valor
void taskSoilSensor(void *pvParameters) {
  (void) pvParameters;

  // GPIO4 já é entrada por padrão, não precisa de pinMode aqui

  for (;;) {
    int raw = analogRead(SOIL_PIN); // leitura ADC (0–4095 no ESP32)

    Serial.print("[UMIDADE] Leitura ADC no GPIO4 (pin 5 da placa): ");
    Serial.println(raw);

    vTaskDelay(SENSOR_PERIOD);
  }
}

void setup() {
  Serial.begin(115200);

  xTaskCreate(
    taskRelay,
    "RelayTask",
    2048,
    NULL,
    1,
    NULL
  );

  xTaskCreate(
    taskSoilSensor,
    "SoilTask",
    2048,
    NULL,
    1,
    NULL
  );
}

void loop() {
  vTaskDelay(portMAX_DELAY);
}
