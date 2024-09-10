import os
import subprocess
import platform
import readline  # For command history and autocomplete on Unix-like systems
import shutil
import paramiko  # For SSH
import telnetlib  # For Telnet

ROOT_PATH = "D:/"

def display_prompt(current_directory):
    path = os.path.relpath(current_directory, ROOT_PATH)
    print(f"osx-lite:/{path}$ ", end="")

def run_system_command(command):
    try:
        result = subprocess.run(command, shell=True, check=True, text=True, capture_output=True)
        print(result.stdout)
    except subprocess.CalledProcessError as e:
        print(f"Error: {e.stderr}")

def change_directory(path, current_directory):
    try:
        os.chdir(path)
        return os.getcwd()
    except FileNotFoundError:
        print("Directory not found.")
        return current_directory

def list_files(current_directory):
    try:
        for item in os.listdir(current_directory):
            if os.path.isdir(os.path.join(current_directory, item)):
                print(f"{item}/", end=" ")
            else:
                print(item, end=" ")
        print()
    except FileNotFoundError:
        print("Directory not found.")

def make_directory(current_directory, directory_name):
    new_directory = os.path.join(current_directory, directory_name)
    try:
        os.mkdir(new_directory)
        print(f"Created directory {directory_name}.")
    except FileExistsError:
        print("Directory already exists.")
    except Exception as e:
        print(f"Error: {e}")

def ssh_connect(host, username, password):
    """Connect to a remote host using SSH."""
    try:
        client = paramiko.SSHClient()
        client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
        client.connect(host, username=username, password=password)
        print(f"Connected to {host} via SSH.")
        return client
    except Exception as e:
        print(f"SSH connection failed: {e}")
        return None

def telnet_connect(host, port):
    """Connect to a remote host using Telnet."""
    try:
        tn = telnetlib.Telnet(host, port)
        print(f"Connected to {host} on port {port} via Telnet.")
        return tn
    except Exception as e:
        print(f"Telnet connection failed: {e}")
        return None

def display_kernel_version():
    if platform.system() == "Windows":
        build = platform.version()
        print(f"Windows NT Kernel Version: {build} (OS Build 27831)")
    else:
        print(f"Kernel Version: {platform.release()}")

def display_help():
    print("Available commands:")
    print("  ls: List files and directories in the current directory.")
    print("  cd <path>: Change the current directory to <path>.")
    print("  mkdir <directory_name>: Create a new directory.")
    print("  ssh <host> <username> <password>: Connect to a remote host via SSH.")
    print("  telnet <host> <port>: Connect to a remote host via Telnet.")
    print("  kernel: Display the kernel version.")
    print("  help: Display this help message.")
    print("  exit: Exit the program.")

def parse_command(current_directory, command_line):
    parts = command_line.split()
    command = parts[0]
    args = parts[1:]

    if command == "ls":
        list_files(current_directory)
    elif command == "cd" and args:
        current_directory = change_directory(args[0], current_directory)
    elif command == "mkdir" and args:
        make_directory(current_directory, args[0])
    elif command == "ssh" and len(args) == 3:
        ssh_connect(args[0], args[1], args[2])
    elif command == "telnet" and len(args) == 2:
        telnet_connect(args[0], args[1])
    elif command == "kernel":
        display_kernel_version()
    elif command == "help":
        display_help()
    elif command == "exit":
        exit()
    else:
        # Fallback to system command
        run_system_command(command_line)

    return current_directory

def main():
    current_directory = ROOT_PATH
    while True:
        display_prompt(current_directory)
        command_line = input()
        current_directory = parse_command(current_directory, command_line)

if __name__ == "__main__":
    main()
