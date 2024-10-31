# app.py
# Previsor de Preços de Criptomoedas com LSTM 🚀

Este projeto usa uma rede neural LSTM para prever o preço de criptomoedas em tempo real, com dados fornecidos pela API da Binance. O projeto inclui uma interface com Dash que exibe o preço real e a previsão.

## Tecnologias Utilizadas
- **Python**: linguagem de programação principal.
- **Dash**: para criar a interface gráfica.
- **Plotly**: para visualização dos gráficos.
- **TensorFlow e Keras**: para criar e treinar a rede LSTM.
- **Binance API**: para obter dados de mercado em tempo real.

## Funcionalidades
- Exibe o preço atual do Bitcoin em um gráfico atualizado automaticamente.
- Previsão de preço baseada em uma rede neural LSTM.
- Atualização do gráfico de previsão por meio de um botão, com visualização de indicadores técnicos (como SMA e RSI).

## Instalação

1. Clone este repositório:
   ```bash
   git clone https://github.com/seu_usuario/nome_do_projeto.git
   cd nome_do_projeto

2. Instale as dependências:
   ```bash
   pip install -r requirements.txt

4. Defina suas credenciais da Binance:
- **Para utilizar a API da Binance, é necessário uma API Key e Secret Key. Coloque-as no código onde indicado.**

## USO
1. Execute o arquivo principal:
   ```bash
   python nome_do_arquivo.py
   
3. Abra o navegador e acesse para ver a interface gráfica:
   ```bash
   http://localhost:8050


## Estrutura do Código
- **LSTM Model**: Utiliza uma rede neural LSTM para previsão de preços baseada em dados históricos.
- **Indicators**: Calcula a média móvel (SMA) e o índice de força relativa (RSI) para análises técnicas.
- **Dashboard com Dash**: Interface que exibe o gráfico de preços em tempo real e as previsões da rede neural.
