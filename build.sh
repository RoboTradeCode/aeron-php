#!/bin/sh

# Сборка расширения
echo "Building Extension..."
mkdir -p build/Debug
cd build/Debug || exit 1
cmake ../..
cmake --build . --target 'aeron'
sudo make install
cmake --build . --target 'build'
