#include <iostream>
#include <fstream>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstring>
#include <vector>

using namespace std;

// ---------- Utility ----------
void printLine() { 
    cout << "========================================" << endl; 
}

// ---------- Command History ----------
void logCommand(const string &cmd) {
    ofstream log("history.txt", ios::app);
    if (log) log << cmd << endl;
}

void showHistory() {
    ifstream log("history.txt");
    string line;
    cout << "\n📜 Command History:\n";
    printLine();
    if (log) {
        while (getline(log, line))
            cout << line << endl;
    } else {
        cout << "No history found." << endl;
    }
    printLine();
}

// ---------- Progress Bar ----------
void progressBar(size_t current, size_t total) {
    int width = 40;
    float ratio = (total > 0) ? (float)current / total : 1.0;
    int filled = ratio * width;
    cout << "\r[";
    for (int i = 0; i < filled; ++i) cout << "#";
    for (int i = filled; i < width; ++i) cout << " ";
    cout << "] " << int(ratio * 100) << "%";
    cout.flush();
}

// ---------- Copy File with Progress ----------
bool copyFileWithProgress(const string &src, const string &dest) {
    ifstream in(src, ios::binary);
    ofstream out(dest, ios::binary);
    
    if (!in) {
        cerr << "\n❌ Cannot open source file: " << src << endl;
        return false;
    }
    
    if (!out) {
        cerr << "\n❌ Cannot create destination file: " << dest << endl;
        return false;
    }

    in.seekg(0, ios::end);
    size_t total = in.tellg();
    in.seekg(0);

    const size_t bufSize = 4096;
    char buffer[bufSize];
    size_t copied = 0;

    while (in.read(buffer, bufSize) || in.gcount()) {
        out.write(buffer, in.gcount());
        copied += in.gcount();
        progressBar(copied, total);
        usleep(30000); // simulate delay for visible progress
    }
    cout << endl;
    return true;
}

// ---------- List Files ----------
void listFiles() {
    DIR *dir;
    struct dirent *entry;
    struct stat fileStat;

    dir = opendir(".");
    if (dir == NULL) {
        cerr << "❌ Unable to open directory" << endl;
        return;
    }

    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    cout << "\n📂 Current Directory: " << cwd << endl;
    printLine();

    while ((entry = readdir(dir)) != NULL) {
        string name = entry->d_name;
        if (name == "." || name == "..") 
            continue;
        
        if (stat(name.c_str(), &fileStat) == -1) 
            continue;

        if (S_ISDIR(fileStat.st_mode))
            cout << "📁 " << name << "/" << endl;
        else
            cout << "📄 " << name << endl;
    }
    printLine();
    closedir(dir);
}

// ---------- Search File Recursively ----------
void searchFile(const string &target, const string &path = ".") {
    DIR *dir = opendir(path.c_str());
    if (!dir) return;

    struct dirent *entry;
    struct stat fileStat;
    string fullPath;

    while ((entry = readdir(dir)) != NULL) {
        string name = entry->d_name;
        if (name == "." || name == "..") 
            continue;

        fullPath = path + "/" + name;
        if (stat(fullPath.c_str(), &fileStat) == -1) 
            continue;

        if (name == target)
            cout << "✅ Found: " << fullPath << endl;

        if (S_ISDIR(fileStat.st_mode))
            searchFile(target, fullPath);
    }
    closedir(dir);
}

// ---------- Permissions ----------
void viewPermissions(const string &filename) {
    struct stat fileStat;
    
    if (stat(filename.c_str(), &fileStat) == -1) {
        perror("❌ Error reading file");
        return;
    }

    cout << "\n🔒 Permissions for: " << filename << endl;
    cout << "User: "
         << ((fileStat.st_mode & S_IRUSR) ? "r" : "-")
         << ((fileStat.st_mode & S_IWUSR) ? "w" : "-")
         << ((fileStat.st_mode & S_IXUSR) ? "x" : "-") << endl;
    cout << "Group: "
         << ((fileStat.st_mode & S_IRGRP) ? "r" : "-")
         << ((fileStat.st_mode & S_IWGRP) ? "w" : "-")
         << ((fileStat.st_mode & S_IXGRP) ? "x" : "-") << endl;
    cout << "Others: "
         << ((fileStat.st_mode & S_IROTH) ? "r" : "-")
         << ((fileStat.st_mode & S_IWOTH) ? "w" : "-")
         << ((fileStat.st_mode & S_IXOTH) ? "x" : "-") << endl;
}

void changePermissions(const string &filename, mode_t mode) {
    if (chmod(filename.c_str(), mode) == 0)
        cout << "✅ Permissions updated for: " << filename << endl;
    else
        perror("❌ Failed to change permissions");
}

// ---------- Move/Rename File ----------
void moveFile(const string &source, const string &dest) {
    if (rename(source.c_str(), dest.c_str()) == 0)
        cout << "🚚 Moved/Renamed: " << source << " → " << dest << endl;
    else
        perror("❌ Move failed");
}

// ---------- View File Contents ----------
void viewFile(const string &filename) {
    ifstream file(filename);
    if (file) {
        cout << "\n📖 Contents of " << filename << ":\n";
        printLine();
        string line;
        while (getline(file, line))
            cout << line << endl;
        printLine();
    } else {
        cerr << "❌ Cannot open file: " << filename << endl;
    }
}

// ---------- Help ----------
void showHelp() {
    printLine();
    cout << "📘 Available Commands:\n"
         << "  ls                        → list files\n"
         << "  cd <folder>               → open folder\n"
         << "  cd..                      → go up one level\n"
         << "  touch <file>              → create file\n"
         << "  del <file>                → delete file\n"
         << "  copy <src> <dest>         → copy file with progress bar\n"
         << "  move <src> <dest>         → move/rename file or folder\n"
         << "  open <file>               → view file contents\n"
         << "  search <filename>         → recursive search\n"
         << "  perm <file>               → view permissions\n"
         << "  chmod <file> <octal>      → change permissions (e.g., 0644)\n"
         << "  history                   → show command history\n"
         << "  help                      → show this help menu\n"
         << "  exit                      → quit explorer\n";
    printLine();
}

// ---------- Main ----------
int main() {
    string command;
    
    cout << "========================================\n"
         << "   🧭 FILE EXPLORER - ENHANCED PRO   \n"
         << "========================================" << endl;

    listFiles();
    showHelp();

    while (true) {
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        cout << "\n" << cwd << " > ";
        getline(cin, command);

        logCommand(command); // store every command

        if (command == "exit") {
            cout << "👋 Exiting File Explorer..." << endl;
            break;
        }
        else if (command == "help") {
            showHelp();
        }
        else if (command == "ls") {
            listFiles();
        }
        else if (command == "history") {
            showHistory();
        }
        else if (command == "cd..") {
            if (chdir("..") == 0) {
                listFiles();
            } else {
                perror("❌ Cannot move up");
            }
        }
        else if (command.rfind("cd ", 0) == 0 && command.length() > 3) {
            string path = command.substr(3);
            if (chdir(path.c_str()) == 0) {
                listFiles();
            } else {
                perror("❌ Cannot change directory");
            }
        }
        else if (command.rfind("touch ", 0) == 0 && command.length() > 6) {
            string filename = command.substr(6);
            ofstream file(filename);
            if (file) {
                cout << "✅ Created file: " << filename << endl;
                listFiles();
            } else {
                cerr << "❌ Failed to create file" << endl;
            }
        }
        else if (command.rfind("del ", 0) == 0 && command.length() > 4) {
            string filename = command.substr(4);
            if (remove(filename.c_str()) == 0) {
                cout << "🗑️  Deleted: " << filename << endl;
                listFiles();
            } else {
                perror("❌ Cannot delete file");
            }
        }
        else if (command.rfind("open ", 0) == 0 && command.length() > 5) {
            string filename = command.substr(5);
            viewFile(filename);
        }
        else if (command.rfind("copy ", 0) == 0 && command.length() > 5) {
            string args = command.substr(5);
            size_t sp = args.find(' ');
            if (sp != string::npos) {
                string src = args.substr(0, sp);
                string dest = args.substr(sp + 1);
                if (copyFileWithProgress(src, dest)) {
                    cout << "📋 Successfully copied: " << src << " → " << dest << endl;
                }
            } else {
                cout << "⚠️  Usage: copy <src> <dest>" << endl;
            }
        }
        else if (command.rfind("move ", 0) == 0 && command.length() > 5) {
            string args = command.substr(5);
            size_t sp = args.find(' ');
            if (sp != string::npos) {
                string src = args.substr(0, sp);
                string dest = args.substr(sp + 1);
                moveFile(src, dest);
            } else {
                cout << "⚠️  Usage: move <src> <dest>" << endl;
            }
        }
        else if (command.rfind("search ", 0) == 0 && command.length() > 7) {
            string filename = command.substr(7);
            cout << "🔍 Searching for: " << filename << endl;
            searchFile(filename, ".");
        }
        else if (command.rfind("perm ", 0) == 0 && command.length() > 5) {
            string filename = command.substr(5);
            viewPermissions(filename);
        }
        else if (command.rfind("chmod ", 0) == 0 && command.length() > 6) {
            string args = command.substr(6);
            size_t sp = args.find(' ');
            if (sp != string::npos) {
                string filename = args.substr(0, sp);
                string modeStr = args.substr(sp + 1);
                mode_t mode = strtol(modeStr.c_str(), NULL, 8);
                changePermissions(filename, mode);
            } else {
                cout << "⚠️  Usage: chmod <file> <octal>" << endl;
            }
        }
        else {
            cout << "⚠️  Unknown command! Type 'help' to see options." << endl;
        }
    }
    
    return 0;
}
