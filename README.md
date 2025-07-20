# Vermit - Version Control System (VCS)
A local version control system for small projects.

## How to Run
Run `./vermit` in ...\vermit-vcs

All commands are run through a REPL-like Shell.

## Repository Structure
```
repo/
├── .vermit/
│    ├── commits/
│    │    └── cmt-hexcode/
│    │         └── file.ext
│    └── log.json
├── your_files.ext
└── your_directory/
     └── your_files.ext
```

Note: Do not put any files inside .vermit folder since they will not be trackable.

## Features
### Mini Shell
Vermit uses a REPL-like mini Shell to run commands and set the current repository.

### Commands
- `cwd <path>`
  - Sets the current working directory for all commands.
    
- `vermit init <name>`
  - Initializes a vermit repository directory with the given name. The initialized repository includes a '.vermit' folder which contains a 'commits' folder and a log.json.
  - Example: `vermit init exampleRepo` creates the repository folder with name 'exampleRepo' and changes the current working directory.
 
- `vermit create <file_name>`
  - Creates an empty file inside current working directory.
  - Example: `vermit create main.py`
    
- `vermit mkdir <folder_name>`
  - Creates a directory/folder inside current working directory.
  - Example: `vermit mkdir docs`
    
- `vermit track <file_name>`
  - Tracks a file inside current working directory. Saved in log.json.
  - Example: `vermit track main.py`
    
- `vermit untrack <filen_name>`
  - Untracks an already tracked file inside current working directory. Updates changes in log.json.
  - Example: `vermit untrack main.py`
    
- `vermit commit <summary>`
  - Commits changes to 'commits' folder with a unique hex ID.
  - Example: `vermit commit initial` creates a folder inside 'commits' with changed tracked files, named with a hex ID.
  - Note: hex IDs have a possibility of clashing, so vermit adds an index (-01, -02, etc.) after to distinguish between commits.
    
- `vermit revert <ID>`
  - Reverts to a commit specified by the ID of the commit.
  - Example: `vermit revert cmt-6595ed`
 
- `vermit log` or `vermit log <var>` or `vermit log all`
  - Returns the contents of log.json (i.e., commits with data and tracked files). Can specify what field of log.json to return or return all contents (including hidden).
  - Example: `vermit log tracking` returns tracked files; `vermit log datetime` returns datetime for all commits.
  - log vars: `id`, `summary`, `datetime`, `files`, `tracking`

More commands to be added...

## Credits

This project uses the [nlohmann/json](https://github.com/nlohmann/json) C++ Json Library.

&copy; 2013-2025 Niels Lohmann - Licensed under the MIT license.

The full MIT license text is included in this repository under 'LICENSE'.
