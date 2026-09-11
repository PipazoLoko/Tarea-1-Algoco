import pandas as pd
import matplotlib.pyplot as plt
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

def generate_matrix_plots():
    # Definición de rutas relativas
    input_file = Path("../data/measurements/matrix_results.csv")
    output_dir = Path("../data/plots")
    output_dir.mkdir(parents=True, exist_ok=True)

    if not input_file.exists():
        print(f"Error: No se encontró el archivo {input_file}")
        return

    # Leer datos y promediar muestras
    df = pd.read_csv(input_file)
    group_cols = ['Algorithm', 'N', 'Type', 'Domain']
    df_mean = df.groupby(group_cols)[['Time_us', 'Memory_KB']].mean().reset_index()

    metrics = {
        'Time_us': 'Tiempo de ejecución (μs)',
        'Memory_KB': 'Consumo de Memoria (KB)'
    }

    # Diferenciación y jitter para algoritmos de matrices
    algo_styles = {
        'Naive':    {'color': '#0072B2', 'marker': 'o', 'ls': '-',  'lw': 2.5, 'jitter': 0.96},
        'Strassen': {'color': '#D55E00', 'marker': 's', 'ls': '--', 'lw': 2.0, 'jitter': 1.04}
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
                        alpha=0.7,
                        markersize=8
                    )

                # Mantener logaritmo base 2 y ticks explícitos
                ax.set_xscale('log', base=2)
                ax.set_xticks([16, 64, 256, 1024])
                ax.get_xaxis().set_major_formatter(plt.ScalarFormatter())
                
                ax.set_xlabel('Dimensión de la matriz ($N \\times N$)')
                ax.set_ylabel(metric_label)
                ax.set_title(f'Rendimiento de Multiplicación de Matrices\n(Type: {t.capitalize()} | Domain: {d})')
                
                plt.legend(title='Algoritmo', frameon=True, shadow=True)

                filename = f"matrix_{metric_col.lower()}_{t}_{d}.png"
                plt.savefig(output_dir / filename)
                plt.close(fig)
                
    print(f"Gráficos de matrices generados con éxito en: {output_dir.resolve()}")

if __name__ == "__main__":
    generate_matrix_plots()