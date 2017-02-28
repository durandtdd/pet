# PET
PET (Portable Executable Tool) is a software / library used to extract and 
display information from a Portable Executable windows file (*.exe, *.dll...)

Currently supported functions:
 1. Show file, linker and minimum os versions
 2. Show data directories and sections
 3. Show all exported functions
 4. Dump file by section
 
Future functions:
 1. Show all imported functions
 2. Show resources

# Usage
## Command line
Usage:
    pet [options] <filename>
Options
    -d, --datadir    Show all data directories
    -e, --exported   Show all exported functions
    -h, --help       Show this help
    -i, --imported   Show all imported functions
    -s, --sections   Show all sections
    -v, --version    Show image, os and linker versions
    --dump           Dump file

## Library
1. Copy the content of the include directory in your project
2. Link to the library file (*.lib for static linking, *.dll for dynamic linking)

Linked libraries must have been compiled with the exact same compiler as they 
export C++ classes using STL elements


# Project structure
|Folder| Description |
| ----- | ------------ |
|bin    | Output files |
|build  | Build files |
|doc    | Documentation |
|include| Public include |
|src    | Sources |
|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;core| Library sources | 
|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;exe | Executable sources|
|test   | Tests files    |
