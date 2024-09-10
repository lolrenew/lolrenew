import os
import shutil
import datetime
import platform

ROOT_PATH = "D:/"

def display_prompt(current_directory):
    path = os.path.relpath(current_directory, ROOT_PATH)
    print(f"osx-lite:/{path}$ ", end="")

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

def change_directory(current_directory, path):
    if path == "..":
        return os.path.dirname(current_directory)
    elif path == "/":
        return ROOT_PATH
    else:
        new_path = os.path.join(current_directory, path)
        if os.path.isdir(new_path):
            return new_path
        else:
            print("Directory not found.")
            return current_directory

def make_directory(current_directory, directory_name):
    new_directory = os.path.join(current_directory, directory_name)
    try:
        os.mkdir(new_directory)
        print(f"Created directory {directory_name}.")
    except FileExistsError:
        print("Directory already exists.")
    except Exception as e:
        print(f"Error: {e}")

def create_file(current_directory, filename):
    file_path = os.path.join(current_directory, filename)
    try:
        with open(file_path, 'w') as f:
            pass
        print(f"Created file {filename}.")
    except Exception as e:
        print(f"Error: {e}")

def display_file_content(current_directory, filename):
    file_path = os.path.join(current_directory, filename)
    try:
        with open(file_path, 'r') as f:
            print(f.read())
    except FileNotFoundError:
        print("File not found.")
    except Exception as e:
        print(f"Error: {e}")

def echo_to_file(current_directory, filename, content):
    file_path = os.path.join(current_directory, filename)
    try:
        with open(file_path, 'w') as f:
            f.write(content)
        print(f"Wrote content to file {filename}.")
    except Exception as e:
        print(f"Error: {e}")

def remove_file(current_directory, filename):
    file_path = os.path.join(current_directory, filename)
    try:
        os.remove(file_path)
        print(f"Removed file {filename}.")
    except FileNotFoundError:
        print("File not found.")
    except Exception as e:
        print(f"Error: {e}")

def remove_directory(current_directory, directory_name):
    directory_path = os.path.join(current_directory, directory_name)
    try:
        os.rmdir(directory_path)
        print(f"Removed directory {directory_name}.")
    except FileNotFoundError:
        print("Directory not found.")
    except OSError:
        print("Directory not empty or cannot be removed.")
    except Exception as e:
        print(f"Error: {e}")

def copy_file(current_directory, source, destination):
    source_path = os.path.join(current_directory, source)
    destination_path = os.path.join(current_directory, destination)
    try:
        shutil.copyfile(source_path, destination_path)
        print(f"Copied file {source} to {destination}.")
    except FileNotFoundError:
        print("Source file not found.")
    except Exception as e:
        print(f"Error: {e}")

def move_file(current_directory, source, destination):
    source_path = os.path.join(current_directory, source)
    destination_path = os.path.join(current_directory, destination)
    try:
        os.rename(source_path, destination_path)
        print(f"Moved file {source} to {destination}.")
    except FileNotFoundError:
        print("Source file not found.")
    except Exception as e:
        print(f"Error: {e}")

def search_in_files(current_directory, keyword):
    found = False
    for root, dirs, files in os.walk(current_directory):
        for file in files:
            file_path = os.path.join(root, file)
            try:
                with open(file_path, 'r') as f:
                    if keyword in f.read():
                        print(f"Found \"{keyword}\" in file {file_path}")
                        found = True
            except Exception as e:
                print(f"Error reading file {file}: {e}")
    if not found:
        print(f"Content \"{keyword}\" not found in any files.")

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
    print("  touch <file_name>: Create a new empty file.")
    print("  cat <file_name>: Display the content of a file.")
    print("  echo <text> [> <file_name>]: Display text or write text to a file.")
    print("  rm <file_name>: Remove a file.")
    print("  rmdir <directory_name>: Remove an empty directory.")
    print("  cp <source_file> <destination_file>: Copy a file.")
    print("  mv <source_file> <destination_file>: Move or rename a file.")
    print("  search <keyword>: Search for a keyword in file contents.")
    print("  pwd: Print the current working directory.")
    print("  date: Display the current date and time.")
    print("  whoami: Display the current user's name (simulated).")
    print("  kernel: Display the kernel version.")
    print("  help: Display this help message.")
    print("  clear: Clear the screen.")
    print("  exit: Exit the program.")

def print_working_directory(current_directory):
    path = os.path.relpath(current_directory, ROOT_PATH)
    print(f"/{path}")

def display_date():
    print(f"Current date and time: {datetime.datetime.now()}")

def display_whoami():
    print("Current user: osx-lite_user")

def clear_screen():
    os.system('cls' if os.name == 'nt' else 'clear')

def exit_program():
    exit()

def parse_command(current_directory, command_line):
    parts = command_line.split()
    command = parts[0]
    args = parts[1:]

    if command == "ls":
        list_files(current_directory)
    elif command == "cd" and args:
        current_directory = change_directory(current_directory, args[0])
    elif command == "mkdir" and args:
        make_directory(current_directory, args[0])
    elif command == "touch" and args:
        create_file(current_directory, args[0])
    elif command == "cat" and args:
        display_file_content(current_directory, args[0])
    elif command == "echo" and args:
        content = " ".join(args[:-2]) if ">" in args else " ".join(args)
        if ">" in args:
            filename = args[-1]
            echo_to_file(current_directory, filename, content)
        else:
            print(content)
    elif command == "rm" and args:
        remove_file(current_directory, args[0])
    elif command == "rmdir" and args:
        remove_directory(current_directory, args[0])
    elif command == "cp" and len(args) == 2:
        copy_file(current_directory, args[0], args[1])
    elif command == "mv" and len(args) == 2:
        move_file(current_directory, args[0], args[1])
    elif command == "search" and args:
        search_in_files(current_directory, args[0])
    elif command == "pwd":
        print_working_directory(current_directory)
    elif command == "date":
        display_date()
    elif command == "whoami":
        display_whoami()
    elif command == "kernel":
        display_kernel_version()
    elif command == "help":
        display_help()
    elif command == "clear":
        clear_screen()
    elif command == "exit":
        exit_program()
    else:
        print(f"Unknown command: {command}")

    return current_directory

def main():
    current_directory = ROOT_PATH
    while True:
        display_prompt(current_directory)
        command_line = input()
        current_directory = parse_command(current_directory, command_line)

if __name__ == "__main__":
    main()
