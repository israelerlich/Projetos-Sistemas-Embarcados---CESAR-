# 🌱 Sistema de Monitoramento de Umidade do Solo

Sistema IoT completo para monitoramento de umidade do solo usando ESP32, MQTT, Flask e React.

## 🎯 Sobre o Projeto

Este projeto implementa um sistema completo de monitoramento de umidade do solo com:

- **ESP32**: Lê dados do sensor de umidade e publica via MQTT
- **Backend (Flask)**: Recebe dados via MQTT e fornece API REST
- **Frontend (React)**: Interface web com gráficos em tempo real
- **MQTT**: Protocolo de comunicação entre ESP32 e Backend

---

## 🛠️ Tecnologias Utilizadas

### Backend
- Python 3.10+
- Flask (servidor web)
- Flask-CORS (comunicação com frontend)
- Flask-SQLAlchemy (banco de dados)
- Paho-MQTT (cliente MQTT)
- SQLite (banco de dados)

### Frontend
- React 18
- Vite (build tool)
- Chart.js (gráficos)
- Axios (requisições HTTP)

### Firmware
- ESP32
- PlatformIO
- Arduino Framework
- Sensor de umidade do solo

---

## 📥 Instalação

### Passo 1: Clonar o Repositório

```bash
git clone https://github.com/seu-usuario/Projetos-Sistemas-Embarcados---CESAR-.git
cd Projetos-Sistemas-Embarcados---CESAR-
```

### Passo 2: Instalar Dependências do Backend

```bash
# Navegar para a pasta do backend
cd backend

# Instalar as dependências Python
pip install -r requirements.txt

# Voltar para a raiz do projeto
cd ..
```

**Dependências instaladas:**
- flask
- flask-cors
- paho-mqtt
- flask-sqlalchemy

### Passo 3: Instalar Dependências do Frontend

```bash
# Navegar para a pasta do frontend
cd frontend

# Instalar as dependências Node.js
npm install

# Voltar para a raiz do projeto
cd ..
```

---

## 🚀 Como Executar

Você precisará de **3 terminais** abertos simultaneamente:

### Terminal 1: Backend (Flask)

```bash
# Navegar para a pasta do backend
cd backend

# Executar o servidor Flask
python app.py
```

**Saída esperada:**
```
✓ Banco de dados inicializado
========================================
  BACKEND - SISTEMA DE MONITORAMENTO    
========================================
✓ Thread MQTT iniciada
Iniciando servidor Flask...
✓ Conectado ao MQTT com código: 0
✓ Inscrito no tópico: esp32/humidity/project_se_lucas
 * Running on http://127.0.0.1:5000
```

### Terminal 2: Frontend (React)

```bash
# Navegar para a pasta do frontend
cd frontend

# Executar o servidor de desenvolvimento
npm run dev
```

**Saída esperada:**
```
VITE v5.4.21  ready in 285 ms

➜  Local:   http://localhost:5173/
➜  Network: use --host to expose
```

### Terminal 3: Simulador MQTT (Teste - Opcional)

Se você **não tem o ESP32** conectado, use o simulador para testar:

```bash
# Na raiz do projeto
python test_mqtt_publisher.py
```

**Saída esperada:**
```
MQTT TEST PUBLISHER - Simulador ESP32
✓ Conectado ao broker MQTT: test.mosquitto.org
✓ Publicando no tópico: esp32/humidity/project_se_lucas

[1] Umidade publicada: 56.5% → {"humidity": 56.5}
[2] Umidade publicada: 78.0% → {"humidity": 78.0}
```

### Acessar a Interface Web

Abra seu navegador e acesse:

```
http://localhost:5173/
```

Você verá a interface com:
- 📊 Gráfico de umidade em tempo real
- 🔢 Valor atual da umidade
- 📈 Histórico de leituras

---

## 🧪 Testando Sem Hardware

Se você não tem o ESP32 físico, pode testar o sistema completo usando o simulador:

### 1. Inicie o Backend e Frontend

Siga os passos em [Como Executar](#como-executar) para iniciar os terminais 1 e 2.

### 2. Execute o Simulador

```bash
python test_mqtt_publisher.py
```

O simulador irá:
- Conectar ao broker MQTT público
- Gerar valores aleatórios de umidade (20-80%)
- Publicar dados a cada 3 segundos
- Simular o comportamento do ESP32

### 3. Verifique o Funcionamento

- Acesse http://localhost:5173/
- Observe os dados sendo atualizados em tempo real
- O gráfico deve mostrar as variações de umidade


## 🔧 Configuração do ESP32

### Pré-requisitos Hardware

- ESP32 (qualquer modelo)
- Sensor de umidade do solo capacitivo
- Cabo USB para programação
- Conexão WiFi disponível


### Configuração do Firmware

1. **Instale o PlatformIO**:
   - Via VSCode: Instale a extensão PlatformIO IDE
   - Ou via CLI: https://platformio.org/install/cli

2. **Configure o WiFi**:
   
   Edite o arquivo `firmware/src/main.cpp`:
   
   ```cpp
   const char* ssid = "SEU_WIFI_AQUI";
   const char* password = "SUA_SENHA_AQUI";
   ```

3. **Compile e faça upload**:

   ```bash
   cd firmware
   pio run --target upload
   ```

4. **Monitore a saída serial**:

   ```bash
   pio device monitor
   ```

### Verificação

Após o upload, você deve ver no monitor serial:

```
Conectando ao WiFi...
✓ WiFi conectado
✓ IP: 192.168.x.x
✓ Conectado ao MQTT
✓ Publicando dados...
```

## 👨‍💻 Autor

Projeto desenvolvido para a disciplina de Sistemas Embarcados - CESAR School

---

