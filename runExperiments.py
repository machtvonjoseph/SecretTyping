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
        
        
def clear_file(file_path):
    with open(file_path, 'w') as file:
        # Opening in 'w' mode clears the file contents
        pass


def run_command(command):
    try:
        # Run the command as a shell process
        result = subprocess.run(command, shell=True, check=True, text=True, capture_output=True)
        print("Command output:", result.stdout)
    except subprocess.CalledProcessError as e:
        print(f"Command failed with error: {e.stderr}")
        sys.exit(1)
        
        
def run_command_and_redirect_output(command, output_file):
    try:
        # Run the command as a shell process
        with open(output_file, 'w') as file:
            result = subprocess.run(command, shell=True, check=True, text=True, stdout=file)
            print("Command output:", result.stdout)
    except subprocess.CalledProcessError as e:
        print(f"Command failed with error: {e.stderr}")
        sys.exit(1)
        
def create_file(directory, filename):
    # Ensure the directory exists
    Path(directory).mkdir(parents=True, exist_ok=True)
    
    # Create the file within the specified directory
    file_path = Path(directory) / filename
    file_path.touch(exist_ok=True)  # Creates the file if it doesn't exist

    print(f"File created: {file_path}")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Copy folder and run a command with an additional argument.")
    parser.add_argument("exprName", type=str, help="experimentName to be run on the src to src tool")
    
    
    args = parser.parse_args()

    # Example source and destination directories
    experiment_folder= f"./{args.exprName}*"
    clang_tool_input= "./numa-clang-tool/input/"
    clang_tool_output= f"./numa-clang-tool/output2/{args.exprName}"
    experiment_output= f"./Output/"
    
    create_file(f"./Result/", f"{args.exprName}.csv")

    # Clear the output folder
    clear_file(f"./Result/{args.exprName}.csv")
    # Copy the folder
    #print(f"Copying folder from {experiment_folder} to {clang_tool_input}")
    # copy_folder(experiment_folder, clang_tool_input)
    command = f"cp -rf ./{args.exprName} {clang_tool_input}"
    run_command(command)

    # Command to be run after copying (you can modify this as needed)
    command = f"cd numa-clang-tool && ./run.sh {args.exprName}"  # Example command

    # Run the command
    run_command(command)
    
    copy_folder(clang_tool_output, experiment_output)
    
    command = f"cd ./Output && make clean"
    
    run_command(command)
    
    command = f"cd ./Output && make"
    
    run_command(command)
    
    command = "cd ./Output && python3 meta.py ./Examples/bin/DSExample --meta n:1...4 --meta t:1...4 --meta D:2 --meta DS_name:stack:bst:ll --meta th_config:numa:regular:split --meta DS_config:numa:regular"
    
    run_command_and_redirect_output(command, f"./Result/{args.exprName}.csv")
    
    
    