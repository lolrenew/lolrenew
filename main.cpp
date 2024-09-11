#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <filesystem>
#include <ctime>
#include <iomanip>
#include <fstream>
using namespace std;
namespace fs = std::filesystem;
const time_t timeBomb = 1731701940;
bool isTimeBombTriggered() {
    time_t currentTime = time(nullptr);
    return currentTime >= timeBomb;
}
void displayPrompt(const fs::path& currentDirectory) {
    cout << "netvn84@localhost:" << currentDirectory.string() << "$ ";
}
void listFiles(const fs::path& currentDirectory) {
    try {
        for (const auto& entry : fs::directory_iterator(currentDirectory)) {
            if (entry.is_directory()) {
                cout << entry.path().filename().string() << "/ ";
            } else {
                cout << entry.path().filename().string() << " ";
            }
        }
        cout << endl;
    } catch (const fs::filesystem_error& e) {
        cout << "Error accessing directory: " << e.what() << endl;
    }
}
fs::path changeDirectory(const fs::path& currentDirectory, const string& path) {
    fs::path newPath = (path == "/") ? "D:/" : currentDirectory / path;

    if (path == "..") {
        return currentDirectory.parent_path();
    }
    if (fs::exists(newPath) && fs::is_directory(newPath)) {
        return newPath;
    } else {
        cout << "Directory not found." << endl;
        return currentDirectory;
    }
}
void makeDirectory(const fs::path& currentDirectory, const string& directoryName) {
    fs::path newDirectory = currentDirectory / directoryName;
    if (fs::exists(newDirectory)) {
        cout << "Directory already exists." << endl;
        return;
    }
    try {
        fs::create_directory(newDirectory);
        cout << "Created directory " << directoryName << "." << endl;
    } catch (const fs::filesystem_error& e) {
        cout << "Error creating directory: " << e.what() << endl;
    }
}
void createFile(const fs::path& currentDirectory, const string& filename) {
    fs::path newFile = currentDirectory / filename;
    if (fs::exists(newFile)) {
        cout << "File already exists." << endl;
        return;
    }
    try {
        ofstream file(newFile.string());
        file.close();
        cout << "Created file " << filename << "." << endl;
    } catch (const ios_base::failure& e) {
        cout << "Error creating file: " << e.what() << endl;
    }
}
void displayFileContent(const fs::path& currentDirectory, const string& filename) {
    fs::path filePath = currentDirectory / filename;
    if (!fs::exists(filePath)) {
        cout << "File not found." << endl;
        return;
    }
    try {
        ifstream file(filePath.string());
        string line;
        while (getline(file, line)) {
            cout << line << endl;
        }
        file.close();
    } catch (const ios_base::failure& e) {
        cout << "Error reading file: " << e.what() << endl;
    }
}
void echoToFile(const fs::path& currentDirectory, const string& filename, const string& content) {
    fs::path filePath = currentDirectory / filename;
    if (!fs::exists(filePath)) {
        cout << "File not found." << endl;
        return;
    }
    try {
        ofstream file(filePath.string());
        file << content;
        file.close();
        cout << "Wrote content to file " << filename << "." << endl;
    } catch (const ios_base::failure& e) {
        cout << "Error writing to file: " << e.what() << endl;
    }
}
void removeFile(const fs::path& currentDirectory, const string& filename) {
    fs::path filePath = currentDirectory / filename;
    if (fs::exists(filePath) && fs::is_regular_file(filePath)) {
        try {
            fs::remove(filePath);
            cout << "Removed file " << filename << "." << endl;
        } catch (const fs::filesystem_error& e) {
            cout << "Error removing file: " << e.what() << endl;
        }
    } else {
        cout << "File not found." << endl;
    }
}
void removeDirectory(const fs::path& currentDirectory, const string& directoryName) {
    fs::path dirPath = currentDirectory / directoryName;
    if (fs::exists(dirPath) && fs::is_directory(dirPath)) {
        try {
            fs::remove_all(dirPath);
            cout << "Removed directory " << directoryName << "." << endl;
        } catch (const fs::filesystem_error& e) {
            cout << "Error removing directory: " << e.what() << endl;
        }
    } else {
        cout << "Directory not found." << endl;
    }
}
void copyFile(const fs::path& currentDirectory, const string& source, const string& destination) {
    fs::path srcPath = currentDirectory / source;
    fs::path destPath = currentDirectory / destination;
    if (fs::exists(srcPath) && fs::is_regular_file(srcPath) && !fs::exists(destPath)) {
        try {
            fs::copy(srcPath, destPath);
            cout << "Copied file " << source << " to " << destination << "." << endl;
        } catch (const fs::filesystem_error& e) {
            cout << "Error copying file: " << e.what() << endl;
        }
    } 
    else {
        cout << "Source file not found or destination file already exists." << endl;
    }
}
void moveFile(const fs::path& currentDirectory, const string& source, const string& destination) {
    fs::path srcPath = currentDirectory / source;
    fs::path destPath = currentDirectory / destination;
    if (fs::exists(srcPath) && fs::is_regular_file(srcPath) && !fs::exists(destPath)) {
        try {
            fs::rename(srcPath, destPath);
            cout << "Moved file " << source << " to " << destination << "." << endl;
        } catch (const fs::filesystem_error& e) {
            cout << "Error moving file: " << e.what() << endl;
        }
    }
    else {
        cout << "Source file not found or destination file already exists." << endl;
    }
}
void checkTimeBomb() {
    time_t currentTime = time(nullptr);
    cout << "Time bomb set for: 15th November 2024, 11:59 PM UTC+7" << endl;
    if (isTimeBombTriggered()) {
        cout << "This evaluation copy has a time bomb. Please upgrade to Release Version." << endl;
        exit(1);
    } else {
        cout << "Time bomb has not been triggered yet." << endl;
        tm* ltm = localtime(&currentTime);
        cout << "Current system time: "
             << (1900 + ltm->tm_year) << "-"
             << setw(2) << setfill('0') << (ltm->tm_mon + 1) << "-"
             << setw(2) << setfill('0') << ltm->tm_mday << " "
             << setw(2) << setfill('0') << ltm->tm_hour << ":"
             << setw(2) << setfill('0') << ltm->tm_min << ":"
             << setw(2) << setfill('0') << ltm->tm_sec
             << " UTC+7" << endl;
    }
}
void parseCommand(fs::path& currentDirectory, const string& command) {
    istringstream iss(command);
    string cmd;
    iss >> cmd;
    if (cmd == "ls") {
        listFiles(currentDirectory);
    } else if (cmd == "cd") {
        string path;
        iss >> path;
        currentDirectory = changeDirectory(currentDirectory, path);
    } else if (cmd == "mkdir") {
        string dirname;
        iss >> dirname;
        makeDirectory(currentDirectory, dirname);
    } else if (cmd == "touch") {
        string filename;
        iss >> filename;
        createFile(currentDirectory, filename);
    } else if (cmd == "cat") {
        string filename;
        iss >> filename;
        displayFileContent(currentDirectory, filename);
    } else if (cmd == "echo") {
        string filename;
        string content;
        iss >> filename;
        getline(iss, content);
        echoToFile(currentDirectory, filename, content.substr(1));
    } else if (cmd == "rm") {
        string filename;
        iss >> filename;
        removeFile(currentDirectory, filename);
    } else if (cmd == "rmdir") {
        string dirname;
        iss >> dirname;
        removeDirectory(currentDirectory, dirname);
    } else if (cmd == "cp") {
        string source, destination;
        iss >> source >> destination;
        copyFile(currentDirectory, source, destination);
    } else if (cmd == "mv") {
        string source, destination;
        iss >> source >> destination;
        moveFile(currentDirectory, source, destination);
    } else if (cmd == "slm") {
        checkTimeBomb();
    } else {
        cout << "Unknown command: " << cmd << endl;
    }
}
int main() {
    fs::path currentDirectory = "D:/";
    string command;
    cout << "Welcome to the Custom File System!" << endl;
    cout << "Type 'help' for a list of commands." << endl;
    while (true) {
        displayPrompt(currentDirectory);
        getline(cin, command);
        if (command == "exit") {
            cout << "Exiting shell." << endl;
            break;
        }
        parseCommand(currentDirectory, command);
    }
    return 0;
}
