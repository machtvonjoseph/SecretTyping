import argparse
import subprocess
import csv
import re
import os

# Available DS and Thread configurations
DS_CONFIGS = ["numa", "regular"]
TH_CONFIGS = ["numa", "regular", "reverse"]

default_bin="../Output/Exprs/Examples/bin/DSExample"
def clear_output_file(output_file):
    """Clears the contents of the output file before running perf."""
    with open(output_file, "w") as f:
        pass  # Just opening in write mode clears the file
    print(f"Cleared existing contents of {output_file}")

def run_perf(th_config,ds_config,binary_path):
    """Runs perf stat for a specific DS and thread configuration."""
    output_file = f"./LocalRemoteStat/local_remote_{th_config}_{ds_config}.txt"

    # Clear output file before running perf
    clear_output_file(output_file)

    perf_command = [
        #"sudo", "numactl", "--cpunodebind=0,1", "--membind=0,1",
        "perf", "stat",
        "-e", "ocr.demand_data_rd.remote_dram",
        "-e", "ocr.demand_data_rd.local_dram",
        "-I", "2000",
        "-o", output_file,
        "--", binary_path,
        "-n", "1000000",
        "-t", "80",
        "-D", "800",
        "--DS_name=bst",
        f"--th_config={th_config}",
        f"--DS_config={ds_config}",
        "-k", "160"
    ]

    print(f"\nRunning command for Thread Config={th_config}, DS={ds_config}...\n")
    print(" ".join(perf_command))

    try:
        subprocess.run(perf_command, check=True)
    except subprocess.CalledProcessError as e:
        print(f"Error: {e}")
    except KeyboardInterrupt:
        print("Process interrupted by user.")

    return output_file  # Return the generated file name

def convert_perf_to_csv(input_file):
    """Converts a perf stat output file into a CSV file."""
    output_csv = input_file.replace(".txt", ".csv")

    with open(input_file, "r") as f:
        lines = f.readlines()

    data = []
    pattern = re.compile(r"^\s*(\d+\.\d+)\s+([\d,]+)\s+ocr\.demand_data_rd\.(remote_dram|local_dram)")

    time_to_values = {}

    for line in lines:
        match = pattern.match(line)
        if match:
            time, count, event_type = match.groups()
            time = float(time)
            count = int(count.replace(",", ""))  # Remove commas from numbers

            if time not in time_to_values:
                time_to_values[time] = {"remote_dram": 0, "local_dram": 0}

            time_to_values[time][event_type] = count

    # Convert dictionary to list
    for time, values in sorted(time_to_values.items()):
        data.append([time, values["remote_dram"], values["local_dram"]])

    # Write to CSV
    with open(output_csv, "w", newline="") as f:
        writer = csv.writer(f)
        writer.writerow(["Time", "Remote DRAM Accesses", "Local DRAM Accesses"])  # Header
        writer.writerows(data)

    print(f"Converted {input_file} to {output_csv}")
    return output_csv  # Return the CSV file path

if __name__ == "__main__":
    print("This program can also run perf on any two numa node machine: ")
    parser = argparse.ArgumentParser(
        description="Run perf stat with customizable arguments and process output.",
        epilog="Example:\n"
               "  python3 run_perf.py --DS_config=numa --th_config=numa\n"
               "  OR run all configurations:\n"
               "  python3 run_perf.py --all"
    )

    parser.add_argument("--DS_config", choices=DS_CONFIGS, help="Data structure configuration (numa, regular).")
    parser.add_argument("--th_config", choices=TH_CONFIGS, help="Thread configuration (numa, regular, reverse).")
    parser.add_argument("--all", action="store_true", help="Run all DS and thread configurations.")
    parser.add_argument("binary_path", nargs="?", default=default_bin,
                        help="Path to the binary executable (default: ../Output/Exprs/Examples/bin/DSExample).")

    args = parser.parse_args()

    generated_files = []

    if args.all:
        # Run all DS and Thread Configurations
        for th in TH_CONFIGS:
            for ds in DS_CONFIGS:
                #output_file = run_perf(th,ds, args.binary_path)
                output_file = f"./LocalRemoteStat/local_remote_{th}_{ds}.txt"
                convert_perf_to_csv(output_file)
                #generated_files.append(csv_file)

        # Merge into final CSV
        #merge_csv(generated_files, "final_perf_results.csv")

    else:
        # Use default values if not provided
        ds_config = args.DS_config if args.DS_config else "numa"
        th_config = args.th_config if args.th_config else "numa"

        # Run single configuration
        #output_file = run_perf(th_config, ds_config,args.binary_path)
        output_file = f"./LocalRemoteStat/local_remote_{th_config}_{ds_config}.txt"
        convert_perf_to_csv(output_file)