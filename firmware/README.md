# 📚 Firmware ESP32 - Guia do Aluno

## 📋 Descrição do Projeto
Sistema de monitoramento de umidade do solo usando ESP32, que envia dados para um dashboard web via MQTT.

## 🔧 Requisitos de Hardware
- **Placa**: ESP32 DevKit
- **Sensor**: Sensor de Umidade do Solo (analógico)
- **Conexões**:
  - Sensor VCC → 3.3V do ESP32
  - Sensor GND → GND do ESP32
  - Sensor OUT → GPIO 34 (ADC1_CH6)

## 💻 Requisitos de Software
- **PlatformIO** (extensão do VS Code)
- **Biblioteca**: PubSubClient (MQTT)

## 🚀 Como Usar

### 1. Configurar WiFi
Abra o arquivo `src/main.cpp` e edite as linhas:
```cpp
const char* WIFI_SSID = "SEU_WIFI_AQUI";
const char* WIFI_PASSWORD = "SUA_SENHA_AQUI";
```

### 2. Configurar MQTT (Opcional)
Se quiser usar outro broker MQTT:
```cpp
const char* MQTT_BROKER = "test.mosquitto.org";
const char* MQTT_TOPIC = "esp32/humidity/project_se_lucas";
```

### 3. Compilar e Enviar
1. Conecte o ESP32 via USB
2. No VS Code, clique em "Upload" (seta →) na barra inferior
3. Aguarde a compilação e upload

### 4. Monitorar
- Clique em "Serial Monitor" (tomada 🔌) na barra inferior
- Configure para **115200 baud**
- Você verá os logs de conexão e envio de dados

## 📊 Formato dos Dados
O ESP32 envia dados no formato JSON:
```json
{
  "humidity": 45.5
}
```

## 🔍 Entendendo o Código

### Estrutura Principal
```
setup()         → Executado 1 vez ao ligar
  ├─ Conecta WiFi
  ├─ Conecta MQTT
  └─ Cria tarefa FreeRTOS

loop()          → Executado continuamente
  └─ Mantém conexão MQTT

tarefaSensor()  → Tarefa paralela (FreeRTOS)
  ├─ Lê sensor
  ├─ Publica dados
  └─ Aguarda 2 segundos
```

### Funções Importantes

#### `conectarWiFi()`
- Conecta à rede WiFi configurada
- Mostra IP obtido no Serial Monitor
- LED pisca durante conexão

#### `conectarMQTT()`
- Conecta ao broker MQTT
- Tenta reconectar se falhar
- Mostra status no Serial

#### `lerUmidade()`
- Lê valor analógico (0-4095)
- Converte para percentual (0-100%)
- Retorna valor de umidade

#### `publicarDados()`
- Cria mensagem JSON
- Envia para o broker MQTT
- Confirma sucesso/erro

#### `tarefaSensor()`
- Tarefa FreeRTOS (roda em paralelo)
- Lê sensor a cada 2 segundos
- Publica dados automaticamente

## ⚙️ Calibração do Sensor

### Método Simples
1. **Sensor no ar (seco)**:
   - Anote o valor ADC mostrado no Serial
   - Este é seu valor MÍNIMO

2. **Sensor na água**:
   - Anote o valor ADC
   - Este é seu valor MÁXIMO

3. **Ajustar código**:
```cpp
float lerUmidade() {
  int valorAnalogico = analogRead(SENSOR_PIN);
  
  // Substitua pelos seus valores calibrados
  int VALOR_SECO = 3000;   // Exemplo: valor no ar
  int VALOR_UMIDO = 1000;  // Exemplo: valor na água
  
  // Mapeia para 0-100%
  float umidade = map(valorAnalogico, VALOR_UMIDO, VALOR_SECO, 100, 0);
  umidade = constrain(umidade, 0, 100);
  
  return umidade;
}
```

## 🐛 Problemas Comuns

### Não conecta ao WiFi
- ✅ Verifique SSID e senha
- ✅ Certifique-se que é rede 2.4GHz (ESP32 não suporta 5GHz)
- ✅ Verifique se a rede está disponível

### Não publica dados MQTT
- ✅ Verifique se conectou ao WiFi primeiro
- ✅ Teste o broker (use MQTT Explorer)
- ✅ Verifique o tópico configurado

### Valores estranhos do sensor
- ✅ Verifique as conexões (VCC, GND, OUT)
- ✅ Calibre o sensor conforme acima
- ✅ Teste o sensor com um multímetro

### Upload falha
- ✅ Segure o botão BOOT durante upload
- ✅ Verifique a porta COM selecionada
- ✅ Instale drivers CH340/CP2102 se necessário

## 📖 Conceitos Aprendidos

### FreeRTOS
- **Tarefas paralelas**: O ESP32 pode executar múltiplas tarefas simultaneamente
- **`xTaskCreate()`**: Cria uma nova tarefa
- **`vTaskDelay()`**: Pausa a tarefa sem travar o sistema

### MQTT
- **Protocolo leve** para IoT
- **Publish/Subscribe**: ESP32 publica, servidor assina
- **Tópicos**: Organizam as mensagens

### ADC (Conversor Analógico-Digital)
- Converte tensão (0-3.3V) em número (0-4095)
- ESP32 tem ADC de 12 bits (2^12 = 4096 valores)
- Usado para ler sensores analógicos

## 🎯 Desafios Extras

1. **Fácil**: Adicione um LED que acende quando umidade < 30%
2. **Médio**: Implemente Deep Sleep para economizar bateria
3. **Difícil**: Adicione sensor de temperatura DHT22

## 📚 Referências
- [Documentação ESP32](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/)
- [PubSubClient Library](https://pubsubclient.knolleary.net/)
- [FreeRTOS](https://www.freertos.org/Documentation/RTOS_book.html)
