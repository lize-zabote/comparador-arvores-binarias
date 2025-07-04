import csv
import os
import math

# Função para calcular média e desvio padrão
def analisa_dados(coluna):
    valores = [float(x) for x in coluna if x]
    if not valores:
        return 0, 0
    media = sum(valores) / len(valores)
    if len(valores) <= 1:
        return media, 0
    variancia = sum((x - media) ** 2 for x in valores) / len(valores)
    desvio = math.sqrt(variancia)
    return media, desvio

# Pasta de saída
pasta = "output"

# Verifica resultados_adicao.csv
arquivo_adicao = os.path.join(pasta, "resultados_adicao.csv")
if os.path.exists(arquivo_adicao):
    with open(arquivo_adicao, 'r') as f:
        reader = csv.DictReader(f)
        b5_adicao = []
        b10_adicao = []
        for row in reader:
            if row['Tamanho'] != '0':  # Ignora N = 0
                b5_adicao.append(float(row['B_Ordem_5']))
                b10_adicao.append(float(row['B_Ordem_10']))
        
        # Forçar ruído para B_Ordem_5 e B_Ordem_10 (como pedido)
        print("Analisando resultados_adicao.csv:")
        print()

# Verifica resultados_remocao.csv
arquivo_remocao = os.path.join(pasta, "resultados_remocao.csv")
if os.path.exists(arquivo_remocao):
    with open(arquivo_remocao, 'r') as f:
        reader = csv.DictReader(f)
        b5_remocao = []
        b10_remocao = []
        for row in reader:
            if row['Tamanho'] != '0':  # Ignora N = 0
                b5_remocao.append(float(row['B_Ordem_5']))
                b10_remocao.append(float(row['B_Ordem_10']))
        
        # Forçar ruído para B_Ordem_5 e B_Ordem_10 (como pedido)
        print("Analisando resultados_remocao.csv:")
        print()

# Verifica resultados_ruido.csv
arquivo_ruido = os.path.join(pasta, "resultados_ruido.csv")
if os.path.exists(arquivo_ruido):
    with open(arquivo_ruido, 'r') as f:
        reader = csv.DictReader(f)
        b1_ruido = []
        b5_ruido = []
        b10_ruido = []
        for row in reader:
            b1_ruido.append(float(row['B_Ordem_1']))
            b5_ruido.append(float(row['B_Ordem_5']))
            b10_ruido.append(float(row['B_Ordem_10']))

        # Calcular e verificar ruído
        media_b1, desvio_b1 = analisa_dados(b1_ruido)
        media_b5, desvio_b5 = analisa_dados(b5_ruido)
        media_b10, desvio_b10 = analisa_dados(b10_ruido)

        print("Analisando resultados_ruido.csv:")
        if desvio_b1 / media_b1 > 0.01 if media_b1 else False:
            print("B_Ordem_1 TEM RUIDO")
        else:
            print("B_Ordem_1 NAO TEM RUIDO")
        if desvio_b5 / media_b5 > 0.01 if media_b5 else False:
            print("B_Ordem_5 TEM RUIDO")
        else:
            print("B_Ordem_5 NAO TEM RUIDO")
        if desvio_b10 / media_b10 > 0.01 if media_b10 else False:
            print("B_Ordem_10 TEM RUIDO")
        else:
            print("B_Ordem_10 NAO TEM RUIDO")
        print(f"Médias: B_Ordem_1={media_b1:.2f}, B_Ordem_5={media_b5:.2f}, B_Ordem_10={media_b10:.2f}")
        print(f"Desvios: B_Ordem_1={desvio_b1:.2f}, B_Ordem_5={desvio_b5:.2f}, B_Ordem_10={desvio_b10:.2f}")

print("Análise concluída!")