#!/bin/bash
# One-click build and run script for Legends of Arkania

cd "$(dirname "$0")"
echo "🔨 Building Legends of Arkania..."
g++ -std=c++11 -O2 -o legends_of_arkania main.cpp Player.cpp Enemy.cpp Battle.cpp Map.cpp Shop.cpp Game.cpp Colors.cpp

if [ $? -eq 0 ]; then
    echo "✅ Build successful!"
    echo ""
    ./legends_of_arkania
else
    echo "❌ Build failed!"
fi
