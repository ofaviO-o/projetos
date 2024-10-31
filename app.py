import numpy as np
import pandas as pd
import plotly.graph_objs as go
from dash import Dash, dcc, html, Input, Output
import websocket
import json
import threading
from binance.client import Client
from tensorflow.keras.models import load_model, Sequential
from tensorflow.keras.layers import LSTM, Dense, Dropout
from sklearn.preprocessing import MinMaxScaler
import pytz

api_key = 'PARA TESTAR O CODIGO VOCÊ PRECISA DE UMA API NA BINANCE'
api_secret = 'PARA TESTAR O CODIGO VOCÊ PRECISA DE UMA API NA BINANCE'

client = Client(api_key, api_secret)

# Variáveis globais para armazenar dados
global df
df = pd.DataFrame(columns=['timestamp', 'close'])

# Escalonador para normalização de dados
scaler = MinMaxScaler(feature_range=(0, 1))

# Definir o fuso horário para Brasília
br_timezone = pytz.timezone('America/Sao_Paulo')

# Função para calcular indicadores técnicos
def calculate_indicators(data):
    # Média Móvel de 14 períodos
    data['SMA_14'] = data['close'].rolling(window=14).mean()
    # Índice de Força Relativa (RSI)
    delta = data['close'].diff()
    gain = (delta.where(delta > 0, 0)).rolling(window=14).mean()
    loss = (-delta.where(delta < 0, 0)).rolling(window=14).mean()
    rs = gain / loss
    data['RSI'] = 100 - (100 / (1 + rs))
    return data

# Função para criar o modelo LSTM
def create_model():
    model = Sequential()
    model.add(LSTM(50, return_sequences=True, input_shape=(60, 1)))
    model.add(Dropout(0.2))  # Adicionando Dropout
    model.add(LSTM(50, return_sequences=False))
    model.add(Dropout(0.2))  # Adicionando Dropout
    model.add(Dense(1))  # Previsão de preço
    model.compile(optimizer='adam', loss='mean_squared_error')
    return model

# Carregar ou criar um modelo
MODEL_PATH = 'lstm_model.h5'
try:
    model = load_model(MODEL_PATH)
except:
    model = create_model()

# Função para atualizar dados em tempo real usando o WebSocket da Binance
def on_message(ws, message):
    global df
    json_message = json.loads(message)
    candle = json_message['k']
    is_candle_closed = candle['x']
    close = candle['c']
    
    # Capturar timestamp e garantir que esteja ciente do fuso horário
    timestamp = pd.to_datetime(candle['T'], unit='ms')
    
    # Converter para o fuso horário de Brasília
    if timestamp.tzinfo is None:
        timestamp = timestamp.tz_localize('UTC').tz_convert(br_timezone)

    if is_candle_closed:
        new_row = pd.DataFrame({'timestamp': [timestamp], 'close': [float(close)]})
        new_row.dropna(inplace=True)
        df = pd.concat([df, new_row], ignore_index=True)
        # Manter apenas os últimos 100 pontos de dados
        df = df.tail(100)

# Iniciar a conexão WebSocket com a Binance para dados em tempo real
ws = websocket.WebSocketApp("wss://stream.binance.com:9443/ws/btcusdt@kline_6s", on_message=on_message)
wst = threading.Thread(target=ws.run_forever)
wst.daemon = True
wst.start()

# Configuração do aplicativo Dash
app = Dash(__name__)

app.layout = html.Div(children=[
    html.H1(children='Previsão de Preços de Criptomoedas com LSTM'),
    dcc.Graph(id='live-price-graph', animate=True),  # Gráfico de preço real
    html.Button('Atualizar Previsão', id='update-button', n_clicks=0),  # Botão para atualizar a previsão
    dcc.Graph(id='forecast-graph', animate=True)  # Gráfico de preço previsto
])

# Callback para atualizar o gráfico de preço real
@app.callback(Output('live-price-graph', 'figure'),
              Input('update-button', 'n_clicks'))  # Este callback é chamado sempre que o botão é clicado
def update_live_price_graph(n_clicks):
    global df

    # Certificar-se de que temos dados para exibir
    if df.empty:
        historical_data = client.get_historical_klines("BTCUSDT", Client.KLINE_INTERVAL_1MINUTE, "100 minutes ago UTC")
        df = pd.DataFrame(historical_data, columns=['timestamp', 'open', 'high', 'low', 'close', 'volume', 'close_time',
                                                    'quote_asset_volume', 'number_of_trades', 'taker_buy_base_asset_volume',
                                                    'taker_buy_quote_asset_volume', 'ignore'])
        
        # Converter para datetime e localizá-lo
        df['timestamp'] = pd.to_datetime(df['timestamp'], unit='ms')
        df['timestamp'] = df['timestamp'].dt.tz_localize('UTC').dt.tz_convert(br_timezone)
        df['close'] = df['close'].astype(float)
        df = df[['timestamp', 'close']]

    # Adicionar os dados mais recentes ao DataFrame
    if not df.empty:
        # Adicionar novo timestamp baseado no fuso horário UTC, para manter consistência
        last_timestamp = df['timestamp'].iloc[-1]  # Pegar o último timestamp da série
        new_price = float(client.get_symbol_ticker(symbol="BTCUSDT")['price'])
        
        # Gerar novo timestamp incremental, evitando desvio de horário
        new_timestamp = last_timestamp + pd.Timedelta(seconds=6)  # Tempo do próximo candle

        # Adicionar novo dado
        new_row = pd.DataFrame({'timestamp': [new_timestamp], 'close': [new_price]})
        df = pd.concat([df, new_row], ignore_index=True)  # Adicionar os dados atualizados

        # Manter apenas os últimos 100 pontos de dados
        df = df.tail(100)

    # Calcular indicadores técnicos
    df = calculate_indicators(df)

    # Criar a figura
    fig = go.Figure()

    # Adicionar a trace do preço real
    fig.add_trace(go.Scatter(
        x=df['timestamp'],
        y=df['close'],
        mode='lines',
        name='Preço Real'
    ))

    fig.update_layout(
        xaxis_title='Tempo',
        yaxis_title='Preço do Bitcoin (R$)',
        showlegend=True,
        xaxis=dict(rangeslider=dict(visible=True)),
        yaxis=dict(tickformat=',.2f')  # Formatação com vírgula nas casas decimais
    )

    return fig

# Callback para atualizar o gráfico de previsão quando o botão é clicado
@app.callback(Output('forecast-graph', 'figure'),
              Input('update-button', 'n_clicks'))
def update_forecast_graph(n_clicks):
    global df

    if df.empty:
        return go.Figure()  # Retornar figura vazia se não houver dados

    # Preparar dados para previsão
    data = df['close'].values.reshape(-1, 1)
    scaled_data = scaler.fit_transform(data)
    n_passos = 60

    predicted_prices = []  # Lista para armazenar preços previstos para plotagem

    if len(scaled_data) >= n_passos:
        for i in range(len(scaled_data) - n_passos):
            x_test = []
            x_test.append(scaled_data[i:i + n_passos])
            x_test = np.array(x_test)
            x_test = np.reshape(x_test, (x_test.shape[0], x_test.shape[1], 1))

            # Fazer previsão
            predicted_price = model.predict(x_test)
            predicted_price = scaler.inverse_transform(predicted_price)
            predicted_prices.append(predicted_price[0][0])

    # Criar a figura
    fig = go.Figure()

    # Adicionar a trace do preço previsto como pontos
    if predicted_prices:
        future_timestamps = df['timestamp'].iloc[n_passos:].reset_index(drop=True)
        fig.add_trace(go.Scatter(
            x=future_timestamps,
            y=predicted_prices,
            mode='markers+lines',  # Pontos e linhas
            name='Preço Previsto',
            marker=dict(color='red', size=5),
            line=dict(color='red', dash='dash')
        ))

    fig.update_layout(
        xaxis_title='Tempo',
        yaxis_title='Preço Previsto do Bitcoin (R$)',
        showlegend=True,
        xaxis=dict(rangeslider=dict(visible=True)),
        yaxis=dict(tickformat=',.2f')  # Formatação com vírgula nas casas decimais
    )

    return fig
print(model.summary())
if __name__ == '__main__':
    app.run_server(debug=False, host='0.0.0.0', port=8050)
