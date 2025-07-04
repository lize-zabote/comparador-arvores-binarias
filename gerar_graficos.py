import pandas as pd
import matplotlib.pyplot as plt
import os

def gerar_grafico(caminho_csv, titulo_grafico, nome_arquivo_saida, escala_y_log=False, estruturas=None):
    try:
        df = pd.read_csv(caminho_csv)
        if df.empty:
            print(f"Erro: O arquivo '{caminho_csv}' está vazio.")
            return
        plt.style.use('seaborn-v0_8-whitegrid')
        plt.figure(figsize=(12, 8))
        colunas = [col for col in df.columns[1:] if col in estruturas] if estruturas else df.columns[1:]
        
        for coluna in colunas:
            if df[coluna].sum() > 0:
                # if coluna == 'B_Ordem_5':
                #     plt.plot(df['Tamanho'], df[coluna], label=coluna, marker='o', markersize=4, linestyle='-', color='#1f77b4', alpha=0.7, linewidth=2)
                # elif coluna == 'B_Ordem_10':
                #     plt.plot(df['Tamanho'], df[coluna], label=coluna, marker='o', markersize=4, linestyle='-', color='#d62728', alpha=0.7, linewidth=1)
                # else:
                    plt.plot(df['Tamanho'], df[coluna], label=coluna, marker='o', markersize=2, linestyle='-')
        
        plt.title(titulo_grafico, fontsize=16, pad=15)
        plt.xlabel("Tamanho do Conjunto (N)", fontsize=12)
        plt.ylabel("Esforço Computacional (Nº de Operações)", fontsize=12)
        
        if escala_y_log:
            plt.yscale('log')
            plt.ylabel("Esforço Computacional (Escala Logarítmica)", fontsize=12)
            
        plt.legend(title="Estrutura de Dados", fontsize=10, loc='best')
        plt.grid(True, which="both" if escala_y_log else "major", linestyle='--', alpha=0.7)
        plt.ticklabel_format(style='plain', axis='x')
        if not escala_y_log:
            plt.ticklabel_format(style='plain', axis='y')
        
        plt.tight_layout()
        plt.savefig(nome_arquivo_saida, dpi=300, bbox_inches='tight')
        print(f"Gráfico '{nome_arquivo_saida}' gerado com sucesso!")
        
        plt.show()
        plt.close()
    
    except FileNotFoundError:
        print(f"Erro: O arquivo '{caminho_csv}' não foi encontrado.")
    except pd.errors.EmptyDataError:
        print(f"Erro: O arquivo '{caminho_csv}' está vazio ou inválido.")
    except Exception as e:
        print(f"Ocorreu um erro inesperado: {e}")

def gerar_grafico_razao(caminho_csv, titulo_grafico, nome_arquivo_saida, numerador, denominador):
    try:
        df = pd.read_csv(caminho_csv)
        if df.empty:
            print(f"Erro: O arquivo '{caminho_csv}' está vazio.")
            return
        
        # Calcular a razão, evitando divisão por zero
        df['Razao'] = df[numerador] / df[denominador].replace(0, 1)  # Substitui 0 por 1 para evitar erros
        df = df[df['Tamanho'] > 0]  # Ignora N=0
        
        plt.style.use('seaborn-v0_8-whitegrid')
        plt.figure(figsize=(12, 8))
        
        plt.plot(df['Tamanho'], df['Razao'], label=f'{numerador} / {denominador}', marker='o', markersize=4, linestyle='-', color='#2ca02c', linewidth=2)
        
        plt.title(titulo_grafico, fontsize=16, pad=15)
        plt.xlabel("Tamanho do Conjunto (N)", fontsize=12)
        plt.ylabel(f"Razão de Operações ({numerador} / {denominador})", fontsize=12)
        
        plt.legend(title="Razão", fontsize=10, loc='best')
        plt.grid(True, linestyle='--', alpha=0.7)
        plt.ticklabel_format(style='plain', axis='x')
        plt.ticklabel_format(style='plain', axis='y')
        
        plt.tight_layout()
        plt.savefig(nome_arquivo_saida, dpi=300, bbox_inches='tight')
        print(f"Gráfico '{nome_arquivo_saida}' gerado com sucesso!")
        
        plt.show()
        plt.close()
    
    except FileNotFoundError:
        print(f"Erro: O arquivo '{caminho_csv}' não foi encontrado.")
    except pd.errors.EmptyDataError:
        print(f"Erro: O arquivo '{caminho_csv}' está vazio ou inválido.")
    except Exception as e:
        print(f"Ocorreu um erro inesperado: {e}")

if __name__ == "__main__":
    gerar_grafico('resultados_adicao.csv', 'Esforço Computacional (Adição)', 'grafico_adicao_log.png', escala_y_log=True)
    gerar_grafico('resultados_adicao.csv', 'Esforço Computacional (Adição)', 'grafico_adicao_linear.png')
    gerar_grafico('resultados_remocao.csv', 'Esforço Computacional (Remoção)', 'grafico_remocao_log.png', escala_y_log=True)
    gerar_grafico('resultados_remocao.csv', 'Esforço Computacional (Remoção)', 'grafico_remocao_linear.png')
    gerar_grafico_razao('resultados_adicao.csv', 'Razão de Operações: B_Ordem_10 vs B_Ordem_5 (Adição)', 'grafico_razao_adicao.png', 'B_Ordem_10', 'B_Ordem_5')