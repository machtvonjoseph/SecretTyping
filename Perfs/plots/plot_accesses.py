import pandas as pd
import matplotlib.pyplot as plt
import argparse
import os
from scipy.interpolate import make_interp_spline
from itertools import product
import numpy as np

# Valid configs
THREAD_CONFIGS = ["numa", "regular", "reverse"]
DS_CONFIGS = ["numa", "regular"]
ALL_CONFIGS = list(product(THREAD_CONFIGS, DS_CONFIGS))

def plot_csv(thread_config, ds_config):
    file_name = f"../LocalRemoteStat/local_remote_{thread_config}_{ds_config}.csv"
    if not os.path.exists(file_name):
        print(f" File not found: {file_name}")
        return

    df = pd.read_csv(file_name)

    time = df['Time'].values
    remote = df['Remote DRAM Accesses'].values / 1e6  # Millions
    local = df['Local DRAM Accesses'].values / 1e6

    x_dense = np.linspace(time.min(), time.max(), 500)
    remote_smooth = make_interp_spline(time, remote, k=3)(x_dense)
    local_smooth = make_interp_spline(time, local, k=3)(x_dense)

    plt.figure(figsize=(10, 5))
    plt.plot(x_dense, remote_smooth, label='Remote DRAM Accesses',color='blue', linewidth=2)
    plt.plot(x_dense, local_smooth, label='Local DRAM Accesses',color='red', linewidth=2)

    plt.title(f"Accesses over Time ({thread_config} / {ds_config})")
    plt.xlabel("Time (s)")
    plt.ylabel("Accesses (Millions)")
    plt.grid(True)
    plt.legend()
    plt.tight_layout()

    output_file_png = f"access_plot_{thread_config}_{ds_config}.png"
    output_file_pdf = f"access_plot_{thread_config}_{ds_config}.pdf"
    plt.savefig(output_file_png, dpi=300)
    plt.savefig(output_file_pdf, dpi=300)

    plt.close()
    print(f" Plot saved as {output_file_png} and {output_file_pdf}")

def main():
    parser = argparse.ArgumentParser(description="Plot remote and local memory accesses over time.")
    parser.add_argument("--thread_config", choices=THREAD_CONFIGS, help="Thread configuration")
    parser.add_argument("--ds_config", choices=DS_CONFIGS, help="Data structure configuration")
    parser.add_argument("--all", action="store_true", help="Plot all combinations")

    args = parser.parse_args()

    if args.all:
        for th, ds in ALL_CONFIGS:
            plot_csv(th, ds)
    else:
        if not args.thread_config or not args.ds_config:
            print(" Please specify both --thread_config and --ds_config or use --all")
        else:
            plot_csv(args.thread_config, args.ds_config)

if __name__ == "__main__":
    main()