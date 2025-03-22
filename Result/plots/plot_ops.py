import pandas as pd
import matplotlib.pyplot as plt
from itertools import product
from scipy.interpolate import make_interp_spline
import numpy as np
import argparse



# Function to plot smooth line graphs
def plot_duration_vs_ops(df, num_threads, autonuma):
    df_filtered = df[df['num_threads'] == num_threads]
    plt.figure(figsize=(12, 6))

    for th, ds in configs:
        label = f"{th} {ds}"
        subset = df_filtered[
            (df_filtered['th_config'] == th) & 
            (df_filtered['DS_config'] == ds)
        ].sort_values(by='duration')

        if not subset.empty and len(subset) > 3:
            x = subset['duration'].values
            y = subset['Total Ops'].values / 1e9  # Scale to billions

            # Interpolate for smoothing
            x_new = np.linspace(x.min(), x.max(), 300)
            spline = make_interp_spline(x, y, k=3)  # Cubic spline
            y_smooth = spline(x_new)

            plt.plot(x_new, y_smooth, label=label)
        elif not subset.empty:
            # fallback to raw points if not enough for spline
            plt.plot(subset['duration'], subset['Total Ops'] / 1e9, label=label, marker='o')

    plt.title(f"Total Ops vs Duration (num_threads = {num_threads})")
    plt.xlabel("Duration (s)")
    plt.ylabel("Total Operations (Billions)")
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    if(autonuma == 1):
        plt.savefig(f"./Throughput/AutoNuma/TotalOps_vs_Duration_{num_threads}threads_AN.png", dpi=300)
        plt.savefig(f"./Throughput/AutoNuma/TotalOps_vs_Duration_{num_threads}threads_AN.pdf", dpi=300)
    else:
        plt.savefig(f"./Throughput/NoAutoNuma/TotalOps_vs_Duration_{num_threads}threads.png", dpi=300)
        plt.savefig(f"./Throughput/NoAutoNuma/TotalOps_vs_Duration_{num_threads}threads.pdf", dpi=300)
    plt.show()

# Plot for 40 and optionally 80 threads






if __name__ == "__main__":
    with open("/proc/sys/kernel/numa_balancing") as f: autonuma = int(f.read().strip())
    print("Plotting with autonuma= ", autonuma)
    # # Load and clean CSV

    csv_file = "../BST_Transactions_AN.csv" if autonuma == 1 else "../BST_Transactions.csv"
    df = pd.read_csv(csv_file)
    print("Input file loaded is ",csv_file)
    df.columns = [col.strip() for col in df.columns]
    df.rename(columns={'thread_config': 'th_config', 'TotalOps': 'Total Ops'}, inplace=True)

    # Normalize configs to lowercase
    df['th_config'] = df['th_config'].astype(str).str.strip().str.lower()
    df['DS_config'] = df['DS_config'].astype(str).str.strip().str.lower()

    # Define configurations
    th_configs = ['numa', 'regular', 'reverse']
    ds_configs = ['numa', 'regular']
    configs = list(product(th_configs, ds_configs))

    plot_duration_vs_ops(df, 40, autonuma)
    plot_duration_vs_ops(df, 80, autonuma)
