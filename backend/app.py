"""
========================================
PROJETO: Sistema de Monitoramento de Umidade do Solo
DISCIPLINA: Sistemas Embarcados / IoT / Desenvolvimento Web
BACKEND: Python + Flask
AUTOR: [Seu Nome]
DATA: 2025
========================================

DESCRIÇÃO:
Este é o servidor backend que:
1. Recebe dados do ESP32 via MQTT
2. Armazena os dados em um banco SQLite
3. Fornece uma API REST para o frontend consultar os dados

BIBLIOTECAS NECESSÁRIAS:
- Flask: Framework web
- Flask-CORS: Permite requisições do frontend
- Flask-SQLAlchemy: ORM para banco de dados
- paho-mqtt: Cliente MQTT
"""

import json
import threading
from flask import Flask, jsonify, request
from flask_cors import CORS
from flask_sqlalchemy import SQLAlchemy
import paho.mqtt.client as mqtt
from datetime import datetime

# ========================================
# CONFIGURAÇÃO DO FLASK
# ========================================
app = Flask(__name__)

# Habilita CORS (Cross-Origin Resource Sharing)
# Isso permite que o frontend (React) acesse a API
CORS(app)

# ========================================
# CONFIGURAÇÃO DO BANCO DE DADOS
# ========================================
# SQLite: banco de dados simples em arquivo
# O arquivo 'humidity.db' será criado automaticamente
app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///humidity.db'
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False  # Desativa warnings desnecessários

# Inicializa o SQLAlchemy (ORM - Object Relational Mapper)
db = SQLAlchemy(app)

# ========================================
# CONFIGURAÇÃO DO MQTT
# ========================================
# Broker público para testes (substitua por privado em produção!)
MQTT_BROKER = "test.mosquitto.org"
MQTT_PORT = 1883  # Porta padrão MQTT (sem criptografia)
MQTT_TOPIC = "esp32/humidity/project_se_lucas"  # Tópico onde o ESP32 publica

# ========================================
# MODELO DE DADOS (Tabela do Banco)
# ========================================
class HumidityReading(db.Model):
    """
    Modelo que representa uma leitura de umidade.
    Cada linha na tabela é uma leitura com:
    - id: identificador único
    - value: valor da umidade (0-100%)
    - timestamp: data/hora da leitura
    """
    # Colunas da tabela
    id = db.Column(db.Integer, primary_key=True)  # ID auto-incremento
    value = db.Column(db.Float, nullable=False)    # Valor da umidade
    timestamp = db.Column(db.DateTime, default=datetime.now)  # Data/hora atual
    
    def to_dict(self):
        """
        Converte o objeto para dicionário (para enviar como JSON)
        """
        return {
            'id': self.id,
            'value': self.value,
            'timestamp': self.timestamp.isoformat()  # Formato ISO: 2025-12-03T17:00:00
        }

# ========================================
# FUNÇÕES DE CALLBACK DO MQTT
# ========================================
def on_connect(client, userdata, flags, rc):
    """
    Chamada quando conecta ao broker MQTT.
    rc = código de resultado (0 = sucesso)
    """
    print(f"✓ Conectado ao MQTT com código: {rc}")
    
    # Inscreve-se no tópico para receber mensagens
    client.subscribe(MQTT_TOPIC)
    print(f"✓ Inscrito no tópico: {MQTT_TOPIC}")

def on_message(client, userdata, msg):
    """
    Chamada quando recebe uma mensagem MQTT.
    Esta função processa os dados do ESP32 e salva no banco.
    """
    try:
        # 1. Decodifica a mensagem (bytes -> string)
        payload = msg.payload.decode()
        print(f"\n--- Nova Mensagem MQTT ---")
        print(f"Tópico: {msg.topic}")
        print(f"Payload: {payload}")
        
        # 2. Converte JSON para dicionário Python
        data = json.loads(payload)
        
        # 3. Verifica se tem o campo 'humidity'
        if 'humidity' in data:
            # 4. Salva no banco de dados
            # IMPORTANTE: Usa app_context() porque estamos em uma thread separada
            with app.app_context():
                # Cria um novo registro
                reading = HumidityReading(value=float(data['humidity']))
                
                # Adiciona à sessão do banco
                db.session.add(reading)
                
                # Confirma a transação (salva no arquivo)
                db.session.commit()
                
                print(f"✓ Dados salvos no banco: {data['humidity']}%")
                print("-------------------------\n")
        else:
            print("⚠ Mensagem sem campo 'humidity'")
            
    except json.JSONDecodeError as e:
        print(f"✗ Erro ao decodificar JSON: {e}")
    except Exception as e:
        print(f"✗ Erro ao processar mensagem: {e}")

# ========================================
# FUNÇÃO PARA INICIAR O CLIENTE MQTT
# ========================================
def start_mqtt():
    """
    Inicia o cliente MQTT em uma thread separada.
    Isso permite que o Flask e o MQTT rodem simultaneamente.
    """
    print("\n========================================")
    print("Iniciando cliente MQTT...")
    print("========================================")
    
    # Cria o cliente MQTT
    client = mqtt.Client()
    
    # Define as funções de callback
    client.on_connect = on_connect
    client.on_message = on_message
    
    try:
        # Conecta ao broker
        print(f"Conectando a {MQTT_BROKER}:{MQTT_PORT}...")
        client.connect(MQTT_BROKER, MQTT_PORT, 60)
        
        # Loop infinito para processar mensagens
        # Esta função bloqueia, por isso rodamos em thread separada
        client.loop_forever()
        
    except Exception as e:
        print(f"✗ Erro na conexão MQTT: {e}")
        print("Verifique sua conexão com a internet!")

# ========================================
# ROTAS DA API REST
# ========================================

@app.route('/api/data', methods=['GET'])
def get_data():
    """
    GET /api/data
    Retorna as últimas 100 leituras de umidade.
    
    Exemplo de resposta:
    [
        {"id": 1, "value": 45.5, "timestamp": "2025-12-03T17:00:00"},
        {"id": 2, "value": 46.2, "timestamp": "2025-12-03T17:00:02"},
        ...
    ]
    """
    # Busca no banco: ordena por data (mais recente primeiro) e limita a 100
    readings = HumidityReading.query.order_by(
        HumidityReading.timestamp.desc()
    ).limit(100).all()
    
    # Converte cada objeto para dicionário e retorna como JSON
    return jsonify([r.to_dict() for r in readings])

@app.route('/api/current', methods=['GET'])
def get_current():
    """
    GET /api/current
    Retorna apenas a leitura mais recente.
    
    Exemplo de resposta:
    {"id": 100, "value": 47.3, "timestamp": "2025-12-03T17:20:00"}
    """
    # Busca a leitura mais recente
    reading = HumidityReading.query.order_by(
        HumidityReading.timestamp.desc()
    ).first()
    
    if reading:
        return jsonify(reading.to_dict())
    else:
        # Se não houver dados, retorna objeto vazio
        return jsonify({})

# ========================================
# INICIALIZAÇÃO DO BANCO DE DADOS
# ========================================
# Cria as tabelas se não existirem
with app.app_context():
    db.create_all()
    print("\n✓ Banco de dados inicializado")

# ========================================
# PONTO DE ENTRADA DO PROGRAMA
# ========================================
if __name__ == '__main__':
    print("\n========================================")
    print("  BACKEND - SISTEMA DE MONITORAMENTO  ")
    print("========================================\n")
    
    # Inicia o cliente MQTT em uma thread separada
    # daemon=True: a thread morre quando o programa principal termina
    mqtt_thread = threading.Thread(target=start_mqtt)
    mqtt_thread.daemon = True
    mqtt_thread.start()
    
    print("\n✓ Thread MQTT iniciada")
    print("\nIniciando servidor Flask...")
    print("========================================\n")
    
    # Inicia o servidor Flask
    # debug=True: reinicia automaticamente quando o código muda
    # port=5000: porta onde o servidor vai rodar
    app.run(debug=True, port=5000, use_reloader=False)
    # use_reloader=False: evita iniciar MQTT duas vezes no modo debug

"""
========================================
NOTAS IMPORTANTES PARA O ALUNO:
========================================

1. ESTRUTURA DO PROJETO:
   - Flask: servidor web que responde requisições HTTP
   - MQTT: protocolo para receber dados do ESP32
   - SQLite: banco de dados em arquivo (simples para projetos pequenos)
   - Threading: permite MQTT e Flask rodarem simultaneamente

2. FLUXO DE DADOS:
   ESP32 → MQTT Broker → on_message() → Banco de Dados → API REST → Frontend

3. ENDPOINTS DA API:
   - GET /api/data: retorna últimas 100 leituras
   - GET /api/current: retorna leitura mais recente

4. COMO TESTAR:
   - Rode este arquivo: python app.py
   - Acesse no navegador: http://localhost:5000/api/data
   - Você deve ver um array JSON com os dados

5. TROUBLESHOOTING:
   - Erro "Address already in use": porta 5000 ocupada, mude para 5001
   - MQTT não conecta: verifique internet
   - Dados não aparecem: verifique se ESP32 está publicando

6. MELHORIAS POSSÍVEIS:
   - Adicionar autenticação (login)
   - Usar PostgreSQL ao invés de SQLite
   - Adicionar validação de dados
   - Implementar paginação na API
   - Adicionar endpoint para deletar dados antigos

========================================
"""
