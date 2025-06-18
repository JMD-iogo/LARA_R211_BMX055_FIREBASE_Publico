import os
import serial  # pip install pyserial
import time
import logging
import csv

logging.basicConfig(level=logging.INFO, format="%(asctime)s - %(levelname)s - %(message)s")

def capturar_dados_serial(porta, baudrate, nome_arquivo, tempo_aquisicao):
    """
    Captura dados da porta serial e os armazena em um arquivo CSV.
    Se o arquivo já existir, os dados serão adicionados como novas linhas.
    
    Parâmetros:
    - porta: porta serial (ex. "COM10").
    - baudrate: taxa de transmissão.
    - nome_arquivo: nome do arquivo CSV a ser salvo.
    - tempo_aquisicao: tempo de aquisição em segundos.
    """
    try:
        # Verifica se o arquivo já existe
        arquivo_existe = os.path.exists(nome_arquivo)
        
        with serial.Serial(porta, baudrate, timeout=1) as ser, open(nome_arquivo, mode='a', newline='') as arquivo_csv:
            writer = csv.writer(arquivo_csv)
            
            tempo_inicio = time.time()
            tempo_valido = 0  # Tempo efetivo de aquisição válido
            nomes_cabecalho = []
            ultimo_tempo_valido = time.time()

            while tempo_valido < tempo_aquisicao:
                linha = ser.readline().decode().strip()
                if linha:
                    partes = linha.split(',')
                    
                    # Verifica se a quantidade de valores é par (nome, valor)
                    if len(partes) % 2 == 0:
                        tempo_atual = time.time()
                        tempo_valido += tempo_atual - ultimo_tempo_valido
                        ultimo_tempo_valido = tempo_atual
                        
                        print(f"Tempo decorrido: {tempo_valido:.2f} segundos (de {tempo_aquisicao})")
                        
                        # Extrair nomes e valores
                        nomes = partes[::2]
                        valores = partes[1::2]
                        
                        # Escrever cabeçalho apenas uma vez, se o arquivo for novo
                        if not nomes_cabecalho and not arquivo_existe:
                            nomes_cabecalho.extend(nomes)
                            writer.writerow(nomes_cabecalho)
                        
                        # Adicionar os valores na linha correta
                        writer.writerow(valores)
                        
                        # Exibir os dados na console
                        print(dict(zip(nomes, valores)))
                    else:
                        logging.warning(f"Dados inválidos recebidos: {linha}")
        
        print(f"Arquivo salvo como {nome_arquivo}")
    
    except serial.SerialException as e:
        logging.error(f"Erro ao acessar a porta serial: {e}")

# Chamada da função para capturar os dados e armazená-los em CSV
capturar_dados_serial("COM10", 115200, "dados.csv", 15)