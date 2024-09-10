#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <memory>
#include <algorithm>

using namespace std;

// Data structure for a file
struct File {
    string name;
    string content;
};

// Data structure for a directory
struct Directory {
    string name;
    vector<shared_ptr<File>> files;
    vector<shared_ptr<Directory>> subdirectories;
    weak_ptr<Directory> parent; // Weak pointer to parent directory
};

// Function to display the command prompt
void displayPrompt(shared_ptr<Directory> currentDirectory) {
    string path = "/";
    shared_ptr<Directory> temp = currentDirectory;
    while (auto parent = temp->parent.lock()) {
        path = temp->name + "/" + path;
        temp = parent;
    }
    cout << "osx-lite:" << path << "$ ";
}

// Function to handle the "ls" command (list files and directories)
void listFiles(const shared_ptr<Directory>& currentDirectory) {
    for (const auto& file : currentDirectory->files) {
        cout << file->name << " ";
    }
    for (const auto& subdir : currentDirectory->subdirectories) {
        cout << subdir->name << "/ ";
    }
    cout << endl;
}

// Function to find a subdirectory by name
shared_ptr<Directory> findSubdirectory(const shared_ptr<Directory>& currentDirectory, const string& path) {
    for (const auto& subdir : currentDirectory->subdirectories) {
        if (subdir->name == path) {
            return subdir;
        }
    }
    return nullptr;
}

// Function to find a file by name
shared_ptr<File> findFile(const shared_ptr<Directory>& currentDirectory, const string& filename) {
    for (const auto& file : currentDirectory->files) {
        if (file->name == filename) {
            return file;
        }
    }
    return nullptr;
}

// Function to handle the "cd" command (change directory)
shared_ptr<Directory> changeDirectory(shared_ptr<Directory> currentDirectory, const string& path) {
    if (path == "..") {
        if (auto parent = currentDirectory->parent.lock()) {
            return parent;
        } else {
            cout << "Already at the root directory." << endl;
            return currentDirectory;
        }
    } else if (path == "/") {
        while (auto parent = currentDirectory->parent.lock()) {
            currentDirectory = parent;
        }
        return currentDirectory;
    } else {
        if (auto subdir = findSubdirectory(currentDirectory, path)) {
            return subdir;
        } else {
            cout << "Directory not found." << endl;
            return currentDirectory;
        }
    }
}

// Function to handle the "mkdir" command (make new directory)
void makeDirectory(shared_ptr<Directory> currentDirectory, const string& directoryName) {
    if (findSubdirectory(currentDirectory, directoryName)) {
        cout << "Directory already exists." << endl;
        return;
    }
    auto newDirectory = make_shared<Directory>();
    newDirectory->name = directoryName;
    newDirectory->parent = currentDirectory;
    currentDirectory->subdirectories.push_back(newDirectory);
    cout << "Created directory " << directoryName << "." << endl;
}

// Function to handle the "touch" command (create new file)
void createFile(shared_ptr<Directory> currentDirectory, const string& filename) {
    if (findFile(currentDirectory, filename)) {
        cout << "File already exists." << endl;
        return;
    }
    auto newFile = make_shared<File>();
    newFile->name = filename;
    newFile->content = "";
    currentDirectory->files.push_back(newFile);
    cout << "Created file " << filename << "." << endl;
}

// Function to handle the "cat" command (display file content)
void displayFileContent(const shared_ptr<Directory>& currentDirectory, const string& filename) {
    if (auto file = findFile(currentDirectory, filename)) {
        cout << file->content << endl;
    } else {
        cout << "File not found." << endl;
    }
}

// Function to handle the "echo" command (write content to file)
void echoToFile(shared_ptr<Directory> currentDirectory, const string& filename, const string& content) {
    if (auto file = findFile(currentDirectory, filename)) {
        file->content = content;
        cout << "Wrote content to file " << filename << "." << endl;
    } else {
        cout << "File not found." << endl;
    }
}

// Function to handle the "echo" command (display text to screen)
void echoText(const string& text) {
    cout << text << endl;
}


// Function to handle the "rm" command (remove file)
void removeFile(shared_ptr<Directory> currentDirectory, const string& filename) {
    auto it = remove_if(currentDirectory->files.begin(), currentDirectory->files.end(),
                       [&filename](const shared_ptr<File>& file) { return file->name == filename; });
    if (it != currentDirectory->files.end()) {
        currentDirectory->files.erase(it, currentDirectory->files.end());
        cout << "Removed file " << filename << "." << endl;
    } else {
        cout << "File not found." << endl;
    }
}

// Function to handle the "rmdir" command (remove directory)
void removeDirectory(shared_ptr<Directory> currentDirectory, const string& directoryName) {
    auto it = remove_if(currentDirectory->subdirectories.begin(), currentDirectory->subdirectories.end(),
                       [&directoryName](const shared_ptr<Directory>& dir) { return dir->name == directoryName; });
    if (it != currentDirectory->subdirectories.end()) {
        currentDirectory->subdirectories.erase(it, currentDirectory->subdirectories.end());
        cout << "Removed directory " << directoryName << "." << endl;
    } else {
        cout << "Directory not found." << endl;
    }
}

// Function to handle the "cp" command (copy file)
void copyFile(shared_ptr<Directory> currentDirectory, const string& source, const string& destination) {
    if (auto file = findFile(currentDirectory, source)) {
        if (!findFile(currentDirectory, destination)) {
            auto newFile = make_shared<File>();
            newFile->name = destination;
            newFile->content = file->content;
            currentDirectory->files.push_back(newFile);
            cout << "Copied file " << source << " to " << destination << "." << endl;
        } else {
            cout << "Destination file already exists." << endl;
        }
    } else {
        cout << "Source file not found." << endl;
    }
}

// Function to handle the "mv" command (move file)
void moveFile(shared_ptr<Directory> currentDirectory, const string& source, const string& destination) {
    if (auto file = findFile(currentDirectory, source)) {
        if (!findFile(currentDirectory, destination)) {
            file->name = destination;
            cout << "Moved file " << source << " to " << destination << "." << endl;
        } else {
            cout << "Destination file already exists." << endl;
        }
    } else {
        cout << "Source file not found." << endl;
    }
}

// Function to handle the "search" command (search file content)
void searchInFiles(const shared_ptr<Directory>& currentDirectory, const string& keyword) {
    bool found = false;
    for (const auto& file : currentDirectory->files) {
        if (file->content.find(keyword) != string::npos) {
            cout << "Found \"" << keyword << "\" in file " << file->name << endl;
            found = true;
        }
    }
    if (!found) {
        cout << "Content \"" << keyword << "\" not found in any files." << endl;
    }
}

// Function to handle the "exit" command (exit the program)
void exitProgram() {
    exit(0);
}

// Function to parse commands
void parseCommand(shared_ptr<Directory>& currentDirectory, const string& commandLine) {
    stringstream ss(commandLine);
    string command;
    ss >> command;

    if (command == "ls") {
        listFiles(currentDirectory);
    } else if (command == "cd") {
        string path;
        ss >> path;
        currentDirectory = changeDirectory(currentDirectory, path);
    } else if (command == "mkdir") {
        string directoryName;
        ss >> directoryName;
        makeDirectory(currentDirectory, directoryName);
    } else if (command == "touch") {
        string filename;
        ss >> filename;
        createFile(currentDirectory, filename);
    } else if (command == "cat") {
        string filename;
        ss >> filename;
        displayFileContent(currentDirectory, filename);
    } else if (command == "echo") {
        string nextWord;
        ss >> nextWord;
        if (nextWord == ">") { // Output redirection to file
            string filename;
            ss >> filename;
            string content;
            string word;
            while (ss >> word) {
                content += word + " ";
            }
            content.pop_back(); // Remove trailing space
            echoToFile(currentDirectory, filename, content);
        } else {
            string text = nextWord;
            while (ss >> nextWord) {
                text += " " + nextWord;
            }
            echoText(text);
        }
    } else if (command == "rm") {
        string filename;
        ss >> filename;
        removeFile(currentDirectory, filename);
    } else if (command == "rmdir") {
        string directoryName;
        ss >> directoryName;
        removeDirectory(currentDirectory, directoryName);
    } else if (command == "cp") {
        string source, destination;
        ss >> source >> destination;
        copyFile(currentDirectory, source, destination);
    } else if (command == "mv") {
        string source, destination;
        ss >> source >> destination;
        moveFile(currentDirectory, source, destination);
    } else if (command == "search") {
        string keyword;
        ss >> keyword;
        searchInFiles(currentDirectory, keyword);
    } else if (command == "exit") {
        exitProgram();
    } else if(command=="help"){
        cout << "Available commands:" << endl;
        cout << "  ls: List files and directories in the current directory." << endl;
        cout << "  cd <path>: Change the current directory to <path>." << endl;
        cout << "  mkdir <directory_name>: Create a new directory." << endl;
        cout << "  touch <file_name>: Create a new empty file." << endl;
        cout << "  cat <file_name>: Display the content of a file." << endl;
        cout << "  echo <text> [> <file_name>]: Display text or write text to a file." << endl;
        cout << "  rm <file_name>: Remove a file." << endl;
        cout << "  rmdir <directory_name>: Remove an empty directory." << endl;
        cout << "  cp <source_file> <destination_file>: Copy a file." << endl;
        cout << "  mv <source_file> <destination_file>: Move or rename a file." << endl;
        cout << "  search <keyword>: Search for a keyword in file contents." << endl;
        cout << "  pwd: Print the current working directory." << endl;
        cout << "  date: Display the current date and time." << endl;
        cout << "  whoami: Display the current user's name (simulated)." << endl;
        cout << "  clear: Clear the screen." << endl;
        cout << "  exit: Exit the program." << endl;
    }
     else {
        cout << "Invalid command." << endl;
    }
}

int main() {
    // Initialize the root directory
    auto rootDirectory = make_shared<Directory>();
    rootDirectory->name = "/";
    rootDirectory->parent.reset();

    // The initial current directory is the root directory
    shared_ptr<Directory> currentDirectory = rootDirectory;

    string commandLine;

    while (true) {
        displayPrompt(currentDirectory);
        getline(cin, commandLine);
        parseCommand(currentDirectory, commandLine);
    }

    return 0;
}
