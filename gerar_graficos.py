import pandas as pd
import matplotlib.pyplot as plt

def gerar_grafico(caminho_csv, titulo_grafico, nome_arquivo_saida, escala_y_log=False): # <--- 1. NOVO PARÂMETRO
    """
    Lê um arquivo CSV de resultados e gera um gráfico de linhas comparativo.

    Args:
        caminho_csv (str): O caminho para o arquivo CSV.
        titulo_grafico (str): O título que será exibido no gráfico.
        nome_arquivo_saida (str): O nome do arquivo PNG para salvar o gráfico.
        escala_y_log (bool): Se True, o eixo Y usará uma escala logarítmica.
    """
    try:
        df = pd.read_csv(caminho_csv)
        
        plt.style.use('seaborn-v0_8-whitegrid')
        plt.figure(figsize=(12, 8))

        for coluna in df.columns[1:]:
            # Ignora colunas com valores zerados, que não podem ser plotados em escala log
            if df[coluna].sum() > 0:
                plt.plot(df['Tamanho'], df[coluna], label=coluna, marker='o', markersize=2, linestyle='-')

        # --- SEÇÃO DE PERSONALIZAÇÃO DO GRÁFICO ---
        
        plt.title(titulo_grafico, fontsize=16)
        plt.xlabel("Tamanho do Conjunto (N)", fontsize=12)
        plt.ylabel("Esforço Computacional (Nº de Operações)", fontsize=12)
        
        # <--- 2. LINHA ADICIONADA PARA A ESCALA LOGARÍTMICA ---
        if escala_y_log:
            plt.yscale('log')
            # Atualiza o rótulo do eixo Y para refletir a nova escala
            plt.ylabel("Esforço Computacional (Escala Logarítmica)", fontsize=12)
            
        plt.legend(title="Estrutura de Dados", fontsize=10)
        plt.ticklabel_format(style='plain', axis='x')

        # Para o eixo y, se não for log, usamos o formato simples. Se for, o matplotlib já lida bem.
        if not escala_y_log:
             plt.ticklabel_format(style='plain', axis='y')

        plt.tight_layout()
        plt.savefig(nome_arquivo_saida, dpi=300)
        print(f"Gráfico '{nome_arquivo_saida}' gerado com sucesso!")
        
        plt.show()

    except FileNotFoundError:
        print(f"Erro: O arquivo '{caminho_csv}' não foi encontrado.")
    except Exception as e:
        print(f"Ocorreu um erro inesperado: {e}")

# --- Execução Principal ---
if __name__ == "__main__":
    # Gerar o gráfico para ADIÇÃO com escala logarítmica no eixo Y
    gerar_grafico(
        caminho_csv='resultados_adicao.csv',
        titulo_grafico='Desempenho de Adição: Comparativo de Árvores (Eixo Y Log)',
        nome_arquivo_saida='grafico_adicao_log.png',
        escala_y_log=True  # <--- 3. MUDAMOS A CHAMADA DA FUNÇÃO
    )

    print("-" * 30)

    # Gerar o gráfico para REMOÇÃO com escala logarítmica no eixo Y
    gerar_grafico(
        caminho_csv='resultados_remocao.csv',
        titulo_grafico='Desempenho de Remoção: Comparativo de Árvores (Eixo Y Log)',
        nome_arquivo_saida='grafico_remocao_log.png',
        escala_y_log=True  # <--- 3. MUDAMOS A CHAMADA DA FUNÇÃO
    )