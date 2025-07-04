import pandas as pd
import matplotlib.pyplot as plt

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
                if coluna == 'B_Ordem_5':
                    plt.plot(df['Tamanho'], df[coluna], label=coluna, marker='o', markersize=4, linestyle='-', color='#1f77b4', alpha=0.7, linewidth=2)
                elif coluna == 'B_Ordem_10':
                    plt.plot(df['Tamanho'], df[coluna], label=coluna, marker='s', markersize=4, linestyle='--', color='#d62728', alpha=0.7, linewidth=2)
                else:
                    plt.plot(df['Tamanho'], df[coluna], label=coluna, marker='o', markersize=2, linestyle='-')
        
        plt.title(titulo_grafico, fontsize=16, pad=15)
        plt.xlabel("Tamanho do Conjunto (N)", fontsize=12)
        plt.ylabel("Esforço Computacional (Nº de Operações)", fontsize=12)
        
        if escala_y_log:
            plt.yscale('log')
            plt.ylabel("Esforço Computacional (Escala Logarítmica)", fontsize=12)
            
        plt.legend(title="Estrutura de Dados", fontsize=10, loc='best')
        plt.grid(True, which="both", linestyle='--', alpha=0.7)
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
    gerar_grafico('resultados_adicao.csv', 'Desempenho de Adição: Comparativo de Árvores (Eixo Y Log)', 'grafico_adicao_log.png', True)
    gerar_grafico('resultados_remocao.csv', 'Desempenho de Remoção: Comparativo de Árvores (Eixo Y Log)', 'grafico_remocao_log.png', True)
    print("-" * 30)
    gerar_grafico('resultados_adicao.csv', 'Desempenho de Adição: Comparativo de Árvores (Eixo Y Linear)', 'grafico_adicao_linear.png', False)
    gerar_grafico('resultados_remocao.csv', 'Desempenho de Remoção: Comparativo de Árvores (Eixo Y Linear)', 'grafico_remocao_linear.png', False)
    print("-" * 30)
    ordens = ['B_Ordem_1', 'B_Ordem_5', 'B_Ordem_10']
    for ordem in ordens:
        gerar_grafico('resultados_adicao.csv', f'Desempenho de Adição: AVL, RubroNegra e {ordem} (Eixo Y Log)', f'grafico_adicao_{ordem}_log.png', True, ['AVL', 'RubroNegra', ordem])
        gerar_grafico('resultados_remocao.csv', f'Desempenho de Remoção: AVL, RubroNegra e {ordem} (Eixo Y Log)', f'grafico_remocao_{ordem}_log.png', True, ['AVL', 'RubroNegra', ordem])
    print("-" * 30)
    gerar_grafico_razao('resultados_adicao.csv', 'Razão de Operações: B_Ordem_10 vs B_Ordem_5 (Adição)', 'grafico_razao_adicao.png', 'B_Ordem_10', 'B_Ordem_5')
    gerar_grafico_razao('resultados_remocao.csv', 'Razão de Operações: B_Ordem_10 vs B_Ordem_5 (Remoção)', 'grafico_razao_remocao.png', 'B_Ordem_10', 'B_Ordem_5')