import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import seaborn as sns
from pathlib import Path

def setup_academic_style():
    """Configura el estilo visual para publicaciones académicas."""
    sns.set_theme(style="whitegrid", context="paper", font_scale=1.2)
    plt.rcParams.update({
        'figure.dpi': 300,
        'savefig.dpi': 300,
        'savefig.bbox': 'tight',
        'lines.linewidth': 2,
        'lines.markersize': 8
    })

def generate_sorting_plots():
    input_file = Path("../data/measurements/sorting_results.csv")
    output_dir = Path("../data/plots")
    output_dir.mkdir(parents=True, exist_ok=True)

    if not input_file.exists():
        print(f"Error: No se encontró el archivo {input_file}")
        return

    df = pd.read_csv(input_file)

    group_cols = ['Algorithm', 'N', 'Type', 'Domain']
    df_mean = df.groupby(group_cols)[['Time_us', 'Memory_KB']].mean().reset_index()

    metrics = {
        'Time_us': 'Tiempo de ejecución (μs)',
        'Memory_KB': 'Consumo de Memoria (KB)'
    }

    algo_styles = {
        'MergeSort':    {'color': '#0072B2', 'marker': 'o', 'ls': '-',  'lw': 3.0, 'jitter': 0.94},
        'QuickSort':    {'color': '#D55E00', 'marker': '^', 'ls': '--', 'lw': 2.5, 'jitter': 0.98},
        'PatienceSort': {'color': '#009E73', 'marker': 's', 'ls': '-.', 'lw': 2.0, 'jitter': 1.02},
        'StdSort':      {'color': '#CC79A7', 'marker': '*', 'ls': ':',  'lw': 1.5, 'jitter': 1.06}
    }

    setup_academic_style()

    for t in df_mean['Type'].unique():
        for d in df_mean['Domain'].unique():
            subset = df_mean[(df_mean['Type'] == t) & (df_mean['Domain'] == d)]
            
            if subset.empty:
                continue

            for metric_col, metric_label in metrics.items():
                fig, ax = plt.subplots(figsize=(8, 5))
                
                for algo, style in algo_styles.items():
                    algo_data = subset[subset['Algorithm'] == algo]
                    if algo_data.empty:
                        continue
                    
                    x_jittered = algo_data['N'] * style['jitter']
                    
                    ax.plot(
                        x_jittered, 
                        algo_data[metric_col], 
                        label=algo,
                        color=style['color'],
                        marker=style['marker'],
                        linestyle=style['ls'],
                        linewidth=style['lw'],
                        alpha=0.7, # Transparencia
                        markersize=8
                    )
                if metric_col == 'Time_us':
                    x_vals = np.sort(subset['N'].unique())
                    if len(x_vals) > 0:
                        y_ideal = x_vals * np.log2(x_vals)
                        
                        max_n = x_vals[-1]
                        max_time_real = subset[subset['N'] == max_n][metric_col].mean()
                        max_time_ideal = max_n * np.log2(max_n)
                        
                        constante_escala = max_time_real / max_time_ideal if max_time_ideal > 0 else 1
                        y_ideal_escalado = constante_escala * y_ideal
                        
                        ax.plot(
                            x_vals, 
                            y_ideal_escalado, 
                            label=r'Ideal $f(N) = N \log_2(N)$',
                            color='gray',     
                            linestyle=':',    
                            linewidth=2.5,
                            alpha=0.6,        
                            zorder=1          
                        )

                ax.set_xscale('log')
                ax.set_xlabel('Tamaño de la entrada ($N$)')
                ax.set_ylabel(metric_label)
                ax.set_title(f'Rendimiento de Ordenamiento\n(Type: {t.capitalize()} | Domain: {d})')
                
                plt.legend(title='Algoritmo', frameon=True, shadow=True)

                filename = f"sorting_{metric_col.lower()}_{t}_{d}.png"
                plt.savefig(output_dir / filename)
                plt.close(fig)
                               
    print(f"Gráficos de ordenamiento generados con éxito en: {output_dir.resolve()}")

if __name__ == "__main__":
    generate_sorting_plots()