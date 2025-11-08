# File Explorer - Console-Based Application (C++): 

A fully functional **File Explorer Application** built using **C++** that simulates core file management operations directly in the terminal.  
This project works on **Linux/WSL (Ubuntu)** and provides features like directory navigation, file operations, recursive search, permission management, command history, and progress visualization during file copying.

-------------------------------------------------
## Features
#### Directory management : `ls`, `cd <folder>`, `cd..`, list all files/folders
#### File Operations      : `create`, `open`, `del`, `copy`, `move`, rename files/folders
#### Permissions          : `perm <file>` (view), `chmod <file> <octal>` (edit)
#### Search               : Recursive file search using `search <filename>`
#### Utility              : Help menu (`help`)

-------------------------------------------------
## Command Reference
#### `ls`                   -> List all files and folders in current directory
#### `cd <folder>`          -> Enter a folder
#### `cd..`                 -> Move up one level 
#### `create <file>`        -> Create a new empty file 
#### `open <file>`          -> View contents of a text file 
#### `del <file>`           -> Delete a file 
#### `copy <src> <dest>`    -> Copy file with progress bar 
#### `move <src> <dest>`    -> Move/Rename file or folder 
#### `search <filename>`    -> Recursively search for a file 
#### `perm <file>`          -> Display current file permissions 
#### `chmod <file> <octal>` -> Change file permission (e.g., `chmod notes.txt 0644`) 
#### `history`              -> Show all previously used commands 
#### `help`                 -> Show command reference menu 
#### `exit`                 -> Quit the file explorer 
-------------------------------------------------
