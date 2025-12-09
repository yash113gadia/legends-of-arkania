/*
 * build.cpp - Build script for Legends of Arkania
 * 
 * This file replaces the Makefile. Compile and run this to build the game.
 * 
 * Usage:
 *   g++ build.cpp -o build && ./build
 *   
 * Or simply:
 *   g++ build.cpp -o build -std=c++11 && ./build
 */

#include <cstdlib>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    std::cout << "🔨 Building Legends of Arkania...\n\n";
    
    // Source files to compile
    std::string sources[] = {
        "main.cpp",
        "Player.cpp",
        "Enemy.cpp",
        "Battle.cpp",
        "Map.cpp",
        "Shop.cpp",
        "Game.cpp",
        "Colors.cpp"
    };
    
    // Compiler settings
    std::string compiler = "g++";
    std::string flags = "-std=c++11 -Wall -Wextra -O2";
    std::string output = "legends_of_arkania";
    
    // Check for clean argument
    if (argc > 1 && std::string(argv[1]) == "clean") {
        std::cout << "🧹 Cleaning build artifacts...\n";
        
        // Remove object files
        for (const auto& src : sources) {
            std::string objFile = src.substr(0, src.length() - 4) + ".o";
            std::string cmd = "rm -f " + objFile;
            system(cmd.c_str());
        }
        
        // Remove executable
        system(("rm -f " + output).c_str());
        
        std::cout << "✅ Clean complete!\n";
        return 0;
    }
    
    // Check for run argument (build and run)
    bool runAfterBuild = (argc > 1 && std::string(argv[1]) == "run");
    
    // Build all source files into one executable
    std::string allSources = "";
    for (const auto& src : sources) {
        allSources += src + " ";
    }
    
    std::string buildCommand = compiler + " " + flags + " -o " + output + " " + allSources;
    
    std::cout << "📦 Compiling: " << buildCommand << "\n\n";
    
    int result = system(buildCommand.c_str());
    
    if (result == 0) {
        std::cout << "\n✅ Build successful!\n";
        std::cout << "🎮 Run the game with: ./" << output << "\n";
        
        // Run the game if requested
        if (runAfterBuild) {
            std::cout << "\n🚀 Starting game...\n\n";
            system(("./" + output).c_str());
        }
    } else {
        std::cout << "\n❌ Build failed! Check the errors above.\n";
        return 1;
    }
    
    return 0;
}
