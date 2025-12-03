# 🌱 Sistema de Monitoramento de Umidade do Solo

Sistema IoT completo para monitoramento de umidade do solo usando ESP32, MQTT, Flask e React.

## 📋 Índice

- [Sobre o Projeto](#sobre-o-projeto)
- [Tecnologias Utilizadas](#tecnologias-utilizadas)
- [Pré-requisitos](#pré-requisitos)
- [Instalação](#instalação)
- [Como Executar](#como-executar)
- [Testando Sem Hardware](#testando-sem-hardware)
- [Estrutura do Projeto](#estrutura-do-projeto)
- [Configuração do ESP32](#configuração-do-esp32)

---

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

## ✅ Pré-requisitos

Antes de começar, você precisa ter instalado na sua máquina:

### 1. Python
- **Versão**: 3.10 ou superior
- **Download**: https://www.python.org/downloads/
- **Verificar instalação**:
  ```bash
  python --version
  ```

### 2. Node.js e npm
- **Versão**: Node.js 16+ e npm 8+
- **Download**: https://nodejs.org/
- **Verificar instalação**:
  ```bash
  node --version
  npm --version
  ```

### 3. Git
- **Download**: https://git-scm.com/
- **Verificar instalação**:
  ```bash
  git --version
  ```

### 4. PlatformIO (opcional - apenas para ESP32)
- **Instalação**: https://platformio.org/install
- Necessário apenas se for programar o ESP32

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

---

## 📁 Estrutura do Projeto

```
Projetos-Sistemas-Embarcados---CESAR-/
│
├── backend/                    # Servidor Flask
│   ├── app.py                 # Código principal do backend
│   ├── requirements.txt       # Dependências Python
│   ├── instance/
│   │   └── humidity.db       # Banco de dados SQLite (criado automaticamente)
│   └── README.md             # Documentação do backend
│
├── frontend/                  # Interface React
│   ├── src/
│   │   ├── App.jsx           # Componente principal
│   │   ├── App.css           # Estilos
│   │   └── main.jsx          # Ponto de entrada
│   ├── package.json          # Dependências Node.js
│   └── vite.config.js        # Configuração do Vite
│
├── firmware/                  # Código do ESP32
│   ├── src/
│   │   └── main.cpp          # Código principal do ESP32
│   ├── platformio.ini        # Configuração do PlatformIO
│   └── README.md             # Instruções para ESP32
│
├── test_mqtt_publisher.py    # Simulador MQTT para testes
├── .gitignore                # Arquivos ignorados pelo Git
└── README.md                 # Este arquivo
```

---

## 🔧 Configuração do ESP32

### Pré-requisitos Hardware

- ESP32 (qualquer modelo)
- Sensor de umidade do solo capacitivo
- Cabo USB para programação
- Conexão WiFi disponível

### Conexões

| ESP32 Pin | Sensor |
|-----------|--------|
| GPIO 34   | A0 (Sinal Analógico) |
| 3.3V      | VCC |
| GND       | GND |

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

---

## 🌐 Endpoints da API

O backend fornece os seguintes endpoints:

### GET /api/data
Retorna as últimas 100 leituras de umidade.

**Resposta:**
```json
[
  {
    "id": 1,
    "value": 45.5,
    "timestamp": "2025-12-03T17:00:00"
  },
  {
    "id": 2,
    "value": 46.2,
    "timestamp": "2025-12-03T17:00:03"
  }
]
```

### GET /api/current
Retorna apenas a leitura mais recente.

**Resposta:**
```json
{
  "id": 100,
  "value": 47.3,
  "timestamp": "2025-12-03T17:20:00"
}
```

---

## 🔍 Troubleshooting

### Backend não inicia

**Erro**: `Address already in use`

**Solução**: A porta 5000 está ocupada. Mude a porta no `app.py`:
```python
app.run(debug=True, port=5001, use_reloader=False)
```

### MQTT não conecta

**Erro**: `Erro na conexão MQTT`

**Solução**: Verifique sua conexão com a internet. O broker `test.mosquitto.org` é público e requer internet.

### Frontend não carrega dados

**Solução**:
1. Verifique se o backend está rodando em http://127.0.0.1:5000
2. Teste o endpoint: http://127.0.0.1:5000/api/data
3. Verifique o console do navegador (F12) para erros

### ESP32 não conecta ao WiFi

**Solução**:
1. Verifique SSID e senha no código
2. Certifique-se que o WiFi é 2.4GHz (ESP32 não suporta 5GHz)
3. Verifique o monitor serial para mensagens de erro

---

## 📊 Fluxo de Dados

```
┌─────────┐      MQTT       ┌──────────┐      HTTP      ┌──────────┐
│  ESP32  │ ─────────────> │  Backend │ ─────────────> │ Frontend │
│ Sensor  │  (Publica)      │  Flask   │   (API REST)   │  React   │
└─────────┘                 └──────────┘                └──────────┘
                                  │
                                  ▼
                            ┌──────────┐
                            │  SQLite  │
                            │ Database │
                            └──────────┘
```

1. **ESP32** lê o sensor e publica via MQTT
2. **Backend** recebe dados MQTT e salva no banco
3. **Frontend** consulta a API REST do backend
4. **Usuário** visualiza os dados em tempo real

---

## 🤝 Contribuindo

Contribuições são bem-vindas! Sinta-se à vontade para:

1. Fazer um fork do projeto
2. Criar uma branch para sua feature (`git checkout -b feature/MinhaFeature`)
3. Commit suas mudanças (`git commit -m 'Adiciona MinhaFeature'`)
4. Push para a branch (`git push origin feature/MinhaFeature`)
5. Abrir um Pull Request

---

## 📝 Licença

Este projeto é educacional e está disponível para uso livre.

---

## 👨‍💻 Autor

Projeto desenvolvido para a disciplina de Sistemas Embarcados - CESAR School

---

## 📞 Suporte

Se tiver dúvidas ou problemas:

1. Verifique a seção [Troubleshooting](#troubleshooting)
2. Consulte a documentação de cada componente:
   - [Backend README](backend/README.md)
   - [Firmware README](firmware/README.md)
3. Abra uma issue no GitHub

---

**Bom desenvolvimento! 🚀**
