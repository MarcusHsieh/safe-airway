#!/bin/bash

# Install Qt6 development environment in Ubuntu/WSL
# Run this script with: sudo bash install-qt.sh

echo "Installing Qt6 development environment..."

# Update package lists
apt update

# Install essential build tools
apt install -y build-essential cmake ninja-build

# Install Qt6 base development packages
apt install -y \
    qt6-base-dev \
    qt6-base-dev-tools \
    libqt6core6 \
    libqt6gui6 \
    libqt6widgets6 \
    libqt6printsupport6 \
    qt6-qmake

# Install additional Qt6 modules that might be useful
apt install -y \
    qt6-tools-dev \
    qt6-tools-dev-tools \
    libqt6opengl6-dev

# Verify installation
echo "Checking Qt6 installation..."
qmake6 --version || echo "qmake6 not found, checking qmake..."
qmake --version || echo "qmake not available"

echo "Qt6 installation complete!"
echo "You can now build the Safe Airway application using:"
echo "  mkdir build && cd build"
echo "  cmake .. && make"
echo "or"
echo "  qmake6 ../safe-airway.pro && make"