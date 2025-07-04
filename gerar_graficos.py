import pandas as pd
import matplotlib.pyplot as plt

def gerar_grafico(caminho_csv, titulo_grafico, nome_arquivo_saida, escala_y_log=False, estruturas=None):
    """
    Gera um gráfico comparando o desempenho de diferentes estruturas de dados.
    
    Args:
        caminho_csv (str): Caminho do arquivo CSV com os dados.
        titulo_grafico (str): Título do gráfico.
        nome_arquivo_saida (str): Nome do arquivo de saída para salvar o gráfico.
        escala_y_log (bool): Se True, usa escala logarítmica no eixo y.
        estruturas (list): Lista de estruturas a serem plotadas (opcional).
    """
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
    """
    Gera um gráfico da razão entre as operações de duas estruturas de dados.
    
    Args:
        caminho_csv (str): Caminho do arquivo CSV com os dados.
        titulo_grafico (str): Título do gráfico.
        nome_arquivo_saida (str): Nome do arquivo de saída para salvar o gráfico.
        numerador (str): Nome da coluna do numerador.
        denominador (str): Nome da coluna do denominador.
    """
    try:
        df = pd.read_csv(caminho_csv)
        
        if df.empty or numerador not in df.columns or denominador not in df.columns:
            print(f"Erro: Dados insuficientes ou colunas '{numerador}'/'{denominador}' não encontradas.")
            return
        
        # Evita divisão por zero substituindo zeros por um valor pequeno
        denominador_vals = df[denominador].replace(0, pd.NA).fillna(1e-10)
        razao = df[numerador] / denominador_vals
        
        plt.style.use('seaborn-v0_8-whitegrid')
        plt.figure(figsize=(12, 8))

        plt.plot(df['Tamanho'], razao, label=f'{numerador} / {denominador}', marker='o', markersize=2, linestyle='-')
        
        plt.title(titulo_grafico, fontsize=16, pad=15)
        plt.xlabel("Tamanho do Conjunto (N)", fontsize=12)
        plt.ylabel("Razão de Operações", fontsize=12)
        plt.axhline(y=1.0, color='r', linestyle='--', label='Referência (Igualdade)')
        
        plt.legend(fontsize=10, loc='best')
        plt.grid(True, linestyle='--', alpha=0.7)
        plt.ticklabel_format(style='plain', axis='x')
        plt.ticklabel_format(style='plain', axis='y')

        plt.tight_layout()
        plt.savefig(nome_arquivo_saida, dpi=300, bbox_inches='tight')
        print(f"Gráfico '{nome_arquivo_saida}' gerado com sucesso!")
        
        plt.show()
        plt.close()

    except Exception as e:
        print(f"Ocorreu um erro inesperado: {e}")

if __name__ == "__main__":
    # Combined graphs (logarithmic scale)
    gerar_grafico(
        caminho_csv='resultados_adicao.csv',
        titulo_grafico='Desempenho de Adição: Comparativo de Árvores (Eixo Y Log)',
        nome_arquivo_saida='grafico_adicao_log.png',
        escala_y_log=True
    )

    gerar_grafico(
        caminho_csv='resultados_remocao.csv',
        titulo_grafico='Desempenho de Remoção: Comparativo de Árvores (Eixo Y Log)',
        nome_arquivo_saida='grafico_remocao_log.png',
        escala_y_log=True
    )

    print("-" * 30)

    # Combined graphs (linear scale)
    gerar_grafico(
        caminho_csv='resultados_adicao.csv',
        titulo_grafico='Desempenho de Adição: Comparativo de Árvores (Eixo Y Linear)',
        nome_arquivo_saida='grafico_adicao_linear.png',
        escala_y_log=False
    )

    gerar_grafico(
        caminho_csv='resultados_remocao.csv',
        titulo_grafico='Desempenho de Remoção: Comparativo de Árvores (Eixo Y Linear)',
        nome_arquivo_saida='grafico_remocao_linear.png',
        escala_y_log=False
    )

    print("-" * 30)

    # Separate graphs for each B-Tree order vs AVL and Red-Black (log scale)
    ordens = ['B_Ordem_1', 'B_Ordem_5', 'B_Ordem_10']
    for ordem in ordens:
        gerar_grafico(
            caminho_csv='resultados_adicao.csv',
            titulo_grafico=f'Desempenho de Adição: AVL, RubroNegra e {ordem} (Eixo Y Log)',
            nome_arquivo_saida=f'grafico_adicao_{ordem}_log.png',
            escala_y_log=True,
            estruturas=['AVL', 'RubroNegra', ordem]
        )

        gerar_grafico(
            caminho_csv='resultados_remocao.csv',
            titulo_grafico=f'Desempenho de Remoção: AVL, RubroNegra e {ordem} (Eixo Y Log)',
            nome_arquivo_saida=f'grafico_remocao_{ordem}_log.png',
            escala_y_log=True,
            estruturas=['AVL', 'RubroNegra', ordem]
        )

    print("-" * 30)

    # Ratio plots to compare B-Tree orders
    gerar_grafico_razao(
        caminho_csv='resultados_adicao.csv',
        titulo_grafico='Razão de Operações: B_Ordem_10 vs B_Ordem_5 (Adição)',
        nome_arquivo_saida='grafico_razao_adicao.png',
        numerador='B_Ordem_10',
        denominador='B_Ordem_5'
    )

    gerar_grafico_razao(
        caminho_csv='resultados_remocao.csv',
        titulo_grafico='Razão de Operações: B_Ordem_10 vs B_Ordem_5 (Remoção)',
        nome_arquivo_saida='grafico_razao_remocao.png',
        numerador='B_Ordem_10',
        denominador='B_Ordem_5'
    )