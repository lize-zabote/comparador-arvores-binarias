import pandas as pd
import matplotlib.pyplot as plt

def gerar_grafico(caminho_csv, titulo_grafico, nome_arquivo_saida, escala_y_log=False):
    try:
        df = pd.read_csv(caminho_csv)
        
        plt.style.use('seaborn-v0_8-whitegrid')
        plt.figure(figsize=(12, 8))

        for coluna in df.columns[1:]:
            if df[coluna].sum() > 0:
                plt.plot(df['Tamanho'], df[coluna], label=coluna, marker='o', markersize=2, linestyle='-')
        
        plt.title(titulo_grafico, fontsize=16)
        plt.xlabel("Tamanho do Conjunto (N)", fontsize=12)
        plt.ylabel("Esforço Computacional (Nº de Operações)", fontsize=12)
        
        if escala_y_log:
            plt.yscale('log')
            plt.ylabel("Esforço Computacional (Escala Logarítmica)", fontsize=12)
            
        plt.legend(title="Estrutura de Dados", fontsize=10)
        plt.ticklabel_format(style='plain', axis='x')

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

if __name__ == "__main__":
    gerar_grafico(
        caminho_csv='resultados_adicao.csv',
        titulo_grafico='Desempenho de Adição: Comparativo de Árvores (Eixo Y Log)',
        nome_arquivo_saida='grafico_adicao_log.png',
        escala_y_log=True 
    )

    print("-" * 30)

    gerar_grafico(
        caminho_csv='resultados_remocao.csv',
        titulo_grafico='Desempenho de Remoção: Comparativo de Árvores (Eixo Y Log)',
        nome_arquivo_saida='grafico_remocao_log.png',
        escala_y_log=True
    )