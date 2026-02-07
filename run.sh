#!/bin/bash
set -e

clear

# Check for required tools
if ! command -v cmake &> /dev/null; then
    echo "Error: CMake is required but not installed."
    echo "Please install it using your package manager and run this script again:"
    echo "  macOS:   brew install cmake"
    echo "  Ubuntu:  sudo apt install cmake"
    echo "  Fedora:  sudo dnf install cmake"
    echo "  Arch:    sudo pacman -S cmake"
    echo "  Windows: https://cmake.org/download/"
    exit 1
fi

# Build and run
echo "Configuring project..."
cmake -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

echo "Building project..."
cmake --build build

echo "Running atomic-vec..."
./build/atomic-vec