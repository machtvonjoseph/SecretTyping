#!/usr/bin/python

import shutil
import subprocess
import sys
import argparse
from pathlib import Path

def copy_folder(src_folder, dest_folder):
    try:
        # Copy the folder from source to destination
        shutil.copytree(src_folder, dest_folder, dirs_exist_ok=True)
        print(f"Folder copied from {src_folder} to {dest_folder}")
    except Exception as e:
        print(f"Failed to copy folder: {e}")
        sys.exit(1)

def run_command(command):
    try:
        # Run the command as a shell process
        result = subprocess.run(command, shell=True, check=True, text=True, capture_output=True)
        print("Command output:", result.stdout)
    except subprocess.CalledProcessError as e:
        print(f"Command failed with error: {e.stderr}")
        sys.exit(1)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Copy folder and run a command with an additional argument.")
    parser.add_argument("exprName", type=str, help="experimentName to be run on the src to src tool")
    
    
    args = parser.parse_args()

    # Example source and destination directories
    experiment_folder= f"./{args.exprName}"
    clang_tool_input= "./numa-clang-tool/input/"
    clang_tool_output= f"./numa-clang-tool/output2/{args.exprName}"
    experiment_output= f"./Result{args.exprName}"

    # Copy the folder
    copy_folder(experiment_folder, clang_tool_input)

    # Command to be run after copying (you can modify this as needed)
    command = f"cd numa-clang-tool && ./run.sh {args.exprName}"  # Example command

    # Run the command
    run_command(command)
    
    copy_folder(clang_tool_output, experiment_output)