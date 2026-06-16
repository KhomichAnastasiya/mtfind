# mtfind

Multithreaded pattern search in text files with wildcard support ("?" = any character).

## Build
mkdir build && cd build
cmake ..
make

## Usage
./mtfind <filename> <pattern>

Example:
./mtfind input.txt "?ad"

Output:
3
5 5 bad
6 6 mad
7 6 had

## Tests
./mtfind --test

## Requirements
- C++17
- CMake 3.16+
- GCC or MSVC
