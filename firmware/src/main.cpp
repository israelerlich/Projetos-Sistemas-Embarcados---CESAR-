/*
 * ========================================
 * PROJETO: Sistema de Monitoramento de Umidade do Solo
 * DISCIPLINA: Sistemas Embarcados / IoT
 * HARDWARE: ESP32
 * AUTOR: [Seu Nome]
 * DATA: 2025
 * ========================================
 * 
 * DESCRIÇÃO:
 * Este código lê a umidade do solo através de um sensor analógico
 * e envia os dados para um servidor via protocolo MQTT.
 * Utiliza FreeRTOS para gerenciar tarefas concorrentes.
 * 
 * BIBLIOTECAS NECESSÁRIAS:
 * - WiFi.h (nativa do ESP32)
 * - PubSubClient.h (MQTT)
 * 
 * CONEXÕES DE HARDWARE:
 * - Sensor de Umidade: Pino GPIO 34 (ADC1_CH6)
 * - LED Indicador (opcional): Pino GPIO 2
 */

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

// ========================================
// CONFIGURAÇÕES DE REDE WiFi
// ========================================
// IMPORTANTE: Substitua pelos dados da sua rede
const char* WIFI_SSID = "SEU_WIFI_AQUI";           // Nome da rede WiFi
const char* WIFI_PASSWORD = "SUA_SENHA_AQUI";     // Senha da rede WiFi

// ========================================
// CONFIGURAÇÕES DO BROKER MQTT
// ========================================
// Broker público para testes (substitua por um privado em produção)
const char* MQTT_BROKER = "test.mosquitto.org";   // Endereço do servidor MQTT
const int MQTT_PORT = 1883;                        // Porta padrão MQTT (sem SSL)
const char* MQTT_TOPIC = "esp32/humidity/project_se_lucas";  // Tópico onde os dados serão publicados
const char* MQTT_CLIENT_ID = "ESP32_Humidity_Monitor";       // ID único do cliente

// ========================================
// CONFIGURAÇÕES DE HARDWARE
// ========================================
const int SENSOR_PIN = 34;        // Pino analógico do sensor (ADC)
const int LED_PIN = 2;            // LED interno do ESP32
const int INTERVALO_LEITURA = 2000;  // Intervalo entre leituras (2 segundos)

// ========================================
// OBJETOS GLOBAIS
// ========================================
WiFiClient clienteWiFi;                    // Cliente WiFi
PubSubClient clienteMQTT(clienteWiFi);     // Cliente MQTT usando o WiFi

// ========================================
// FUNÇÃO: Conectar ao WiFi
// ========================================
// Esta função tenta conectar o ESP32 à rede WiFi configurada
// Fica em loop até conseguir conexão
void conectarWiFi() {
  Serial.println("\n========================================");
  Serial.println("Iniciando conexão WiFi...");
  Serial.print("SSID: ");
  Serial.println(WIFI_SSID);
  
  // Inicia a conexão WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  // Aguarda até conectar (mostra pontos na tela)
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    
    // Pisca o LED enquanto tenta conectar
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
  }
  
  // Conexão bem-sucedida!
  digitalWrite(LED_PIN, HIGH);  // LED aceso = conectado
  Serial.println("\n✓ WiFi conectado com sucesso!");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
  Serial.println("========================================\n");
}

// ========================================
// FUNÇÃO: Conectar ao Broker MQTT
// ========================================
// Tenta estabelecer conexão com o servidor MQTT
// Fica em loop até conseguir conectar
void conectarMQTT() {
  // Enquanto não estiver conectado ao MQTT
  while (!clienteMQTT.connected()) {
    Serial.println("Tentando conectar ao broker MQTT...");
    Serial.print("Broker: ");
    Serial.print(MQTT_BROKER);
    Serial.print(":");
    Serial.println(MQTT_PORT);
    
    // Tenta conectar com o ID do cliente
    if (clienteMQTT.connect(MQTT_CLIENT_ID)) {
      Serial.println("✓ Conectado ao broker MQTT!");
      Serial.print("Tópico de publicação: ");
      Serial.println(MQTT_TOPIC);
      Serial.println("========================================\n");
    } else {
      // Se falhar, mostra o código de erro
      Serial.print("✗ Falha na conexão MQTT. Código de erro: ");
      Serial.println(clienteMQTT.state());
      Serial.println("Tentando novamente em 5 segundos...\n");
      delay(5000);
    }
  }
}

// ========================================
// FUNÇÃO: Ler Sensor de Umidade
// ========================================
// Lê o valor analógico do sensor e converte para percentual
// Retorna: valor de 0 a 100 (percentual de umidade)
float lerUmidade() {
  // Lê o valor analógico (0 a 4095 no ESP32)
  int valorAnalogico = analogRead(SENSOR_PIN);
  
  // Converte para percentual (0-100%)
  // NOTA: Esta conversão é simplificada. Em um projeto real,
  // você deve calibrar o sensor com valores reais de umidade
  float percentualUmidade = (valorAnalogico / 4095.0) * 100.0;
  
  // Mostra informações de debug no Serial
  Serial.println("--- Leitura do Sensor ---");
  Serial.print("Valor ADC: ");
  Serial.println(valorAnalogico);
  Serial.print("Umidade: ");
  Serial.print(percentualUmidade, 1);  // 1 casa decimal
  Serial.println("%");
  
  return percentualUmidade;
}

// ========================================
// FUNÇÃO: Publicar Dados via MQTT
// ========================================
// Cria uma mensagem JSON e envia para o broker MQTT
void publicarDados(float umidade) {
  // Cria a mensagem no formato JSON
  // Exemplo: {"humidity": 45.5}
  String mensagemJSON = "{\"humidity\": " + String(umidade, 1) + "}";
  
  // Converte String para array de caracteres (necessário para MQTT)
  char mensagem[50];
  mensagemJSON.toCharArray(mensagem, 50);
  
  // Publica a mensagem no tópico MQTT
  if (clienteMQTT.publish(MQTT_TOPIC, mensagem)) {
    Serial.println("✓ Dados publicados com sucesso!");
    Serial.print("Mensagem: ");
    Serial.println(mensagem);
  } else {
    Serial.println("✗ Erro ao publicar dados");
  }
  
  Serial.println("-------------------------\n");
}

// ========================================
// TAREFA FreeRTOS: Leitura e Publicação
// ========================================
// Esta tarefa roda em paralelo com o loop principal
// Responsável por ler o sensor e publicar os dados
void tarefaSensor(void * parametro) {
  Serial.println("Tarefa do sensor iniciada!");
  
  // Loop infinito da tarefa
  for(;;) {
    // 1. Ler o sensor
    float umidade = lerUmidade();
    
    // 2. Verificar se está conectado ao MQTT
    if (clienteMQTT.connected()) {
      // 3. Publicar os dados
      publicarDados(umidade);
    } else {
      Serial.println("⚠ MQTT desconectado! Tentando reconectar...");
      conectarMQTT();
    }
    
    // 4. Aguardar o intervalo definido antes da próxima leitura
    // vTaskDelay é a função do FreeRTOS para delay em tarefas
    vTaskDelay(INTERVALO_LEITURA / portTICK_PERIOD_MS);
  }
}

// ========================================
// SETUP: Executado uma vez ao ligar
// ========================================
void setup() {
  // Inicializa comunicação serial (para debug)
  Serial.begin(115200);
  delay(1000);  // Aguarda estabilizar
  
  Serial.println("\n\n");
  Serial.println("========================================");
  Serial.println("  SISTEMA DE MONITORAMENTO DE UMIDADE  ");
  Serial.println("========================================");
  
  // Configura o pino do LED como saída
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  // Conecta ao WiFi
  conectarWiFi();
  
  // Configura o servidor MQTT
  clienteMQTT.setServer(MQTT_BROKER, MQTT_PORT);
  
  // Conecta ao broker MQTT
  conectarMQTT();
  
  // Cria a tarefa FreeRTOS para leitura do sensor
  // Parâmetros:
  // - tarefaSensor: função que será executada
  // - "SensorTask": nome da tarefa (para debug)
  // - 10000: tamanho da pilha (stack) em bytes
  // - NULL: parâmetro para a tarefa (não usado)
  // - 1: prioridade (1 = baixa, quanto maior o número, maior a prioridade)
  // - NULL: handle da tarefa (não usado)
  xTaskCreate(
    tarefaSensor,      // Função da tarefa
    "SensorTask",      // Nome da tarefa
    10000,             // Tamanho da stack
    NULL,              // Parâmetros
    1,                 // Prioridade
    NULL               // Handle
  );
  
  Serial.println("Sistema inicializado com sucesso!");
  Serial.println("Iniciando monitoramento...\n");
}

// ========================================
// LOOP: Executado continuamente
// ========================================
void loop() {
  // Mantém a conexão MQTT ativa
  // Esta função processa mensagens recebidas e mantém o heartbeat
  if (!clienteMQTT.connected()) {
    conectarMQTT();
  }
  clienteMQTT.loop();
  
  // Pequeno delay para não sobrecarregar o processador
  // A tarefa FreeRTOS cuida das leituras, então o loop pode ser leve
  delay(10);
}

/*
 * ========================================
 * NOTAS IMPORTANTES PARA O ALUNO:
 * ========================================
 * 
 * 1. CALIBRAÇÃO DO SENSOR:
 *    - O cálculo atual é simplificado (linear)
 *    - Para melhor precisão, meça valores reais:
 *      * Sensor no ar (seco) = valor mínimo
 *      * Sensor na água = valor máximo
 *    - Use esses valores para fazer uma conversão mais precisa
 * 
 * 2. SEGURANÇA:
 *    - NUNCA coloque senhas no código em produção
 *    - Use variáveis de ambiente ou arquivos de configuração
 *    - O broker test.mosquitto.org é PÚBLICO (apenas para testes)
 * 
 * 3. MELHORIAS POSSÍVEIS:
 *    - Adicionar Deep Sleep para economizar bateria
 *    - Implementar reconexão WiFi automática
 *    - Adicionar mais sensores (temperatura, pH, etc)
 *    - Criar um sistema de alertas
 * 
 * 4. TROUBLESHOOTING:
 *    - Se não conectar ao WiFi: verifique SSID e senha
 *    - Se não publicar: verifique o broker MQTT
 *    - Use o Serial Monitor (115200 baud) para debug
 * 
 * ========================================
 */
