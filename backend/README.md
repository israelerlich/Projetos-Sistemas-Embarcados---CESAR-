# 📚 Backend - Guia do Aluno

## 📋 Descrição
Servidor backend em Python que recebe dados do ESP32 via MQTT e fornece uma API REST para o frontend.

## 🔧 Requisitos
```bash
pip install flask flask-cors flask-sqlalchemy paho-mqtt
```

## 🚀 Como Executar

### 1. Instalar Dependências
```bash
cd backend
pip install -r requirements.txt
```

### 2. Executar o Servidor
```bash
python app.py
```

O servidor iniciará em: `http://localhost:5000`

## 📊 Endpoints da API

### GET /api/data
Retorna as últimas 100 leituras de umidade.

**Exemplo de resposta:**
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
    "timestamp": "2025-12-03T17:00:02"
  }
]
```

### GET /api/current
Retorna apenas a leitura mais recente.

**Exemplo de resposta:**
```json
{
  "id": 100,
  "value": 47.3,
  "timestamp": "2025-12-03T17:20:00"
}
```

## 🔍 Entendendo o Código

### Arquitetura
```
┌─────────────┐
│   ESP32     │ Publica dados via MQTT
└──────┬──────┘
       │
       ▼
┌─────────────┐
│ MQTT Broker │ test.mosquitto.org
└──────┬──────┘
       │
       ▼
┌─────────────┐
│  on_message │ Callback que recebe dados
└──────┬──────┘
       │
       ▼
┌─────────────┐
│   SQLite    │ Armazena no banco
└──────┬──────┘
       │
       ▼
┌─────────────┐
│  API REST   │ Frontend consulta
└─────────────┘
```

### Componentes Principais

#### 1. Flask App
```python
app = Flask(__name__)
CORS(app)  # Permite requisições do frontend
```

#### 2. Banco de Dados
```python
# Modelo da tabela
class HumidityReading(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    value = db.Column(db.Float, nullable=False)
    timestamp = db.Column(db.DateTime, default=datetime.now)
```

#### 3. Cliente MQTT
```python
def on_message(client, userdata, msg):
    # Recebe dados do ESP32
    # Salva no banco
```

#### 4. Threading
```python
# MQTT roda em thread separada
mqtt_thread = threading.Thread(target=start_mqtt)
mqtt_thread.daemon = True
mqtt_thread.start()
```

## 🗄️ Banco de Dados

### Estrutura da Tabela
```sql
CREATE TABLE humidity_reading (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    value FLOAT NOT NULL,
    timestamp DATETIME NOT NULL
);
```

### Localização
O arquivo `humidity.db` é criado automaticamente na pasta `backend/`.

### Visualizar Dados
Use o **DB Browser for SQLite** ou execute:
```bash
sqlite3 humidity.db
SELECT * FROM humidity_reading ORDER BY timestamp DESC LIMIT 10;
```

## 🐛 Problemas Comuns

### Porta 5000 já em uso
```python
# Mude a porta no final do app.py
app.run(debug=True, port=5001)
```

### MQTT não conecta
- ✅ Verifique sua conexão com internet
- ✅ Teste o broker: `telnet test.mosquitto.org 1883`
- ✅ Use outro broker se necessário

### Dados não aparecem na API
1. Verifique se o ESP32 está publicando
2. Olhe os logs do servidor (deve mostrar "Dados salvos")
3. Teste diretamente: `curl http://localhost:5000/api/data`

### Erro de CORS
```python
# Já está configurado, mas se precisar ajustar:
CORS(app, resources={r"/api/*": {"origins": "*"}})
```

## 📖 Conceitos Aprendidos

### REST API
- **GET**: Buscar dados
- **JSON**: Formato de troca de dados
- **Endpoints**: URLs que retornam dados

### ORM (SQLAlchemy)
- Trabalha com objetos Python ao invés de SQL
- `db.session.add()`: adiciona registro
- `db.session.commit()`: salva no banco
- `.query.filter()`: busca com filtros

### Threading
- Permite executar código em paralelo
- MQTT e Flask rodam simultaneamente
- `daemon=True`: thread morre com o programa

### MQTT
- **Publish/Subscribe**: padrão de mensageria
- **Broker**: servidor intermediário
- **Topic**: canal de comunicação

## 🎯 Desafios Extras

1. **Fácil**: Adicione endpoint para deletar dados antigos
2. **Médio**: Implemente autenticação com JWT
3. **Difícil**: Migre para PostgreSQL

## 📚 Referências
- [Flask Documentation](https://flask.palletsprojects.com/)
- [SQLAlchemy](https://www.sqlalchemy.org/)
- [Paho MQTT](https://www.eclipse.org/paho/index.php?page=clients/python/index.php)
