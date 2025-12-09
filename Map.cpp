#include "Map.h"
#include "Colors.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstdlib>
#include <iomanip>

Map::Map() : width(0), height(0), regionName("Unknown") {}

Map::Map(const std::string& mapFile) {
    loadFromFile(mapFile);
}

bool Map::loadFromFile(const std::string& mapFile) {
    std::ifstream file(mapFile);
    if (!file.is_open()) {
        std::cerr << "Error: Could not load map file " << mapFile << "\n";
        return false;
    }
    
    filename = mapFile;
    
    // Read region name
    std::getline(file, regionName);
    
    // Read dimensions
    file >> width >> height;
    file.ignore(); // Skip newline
    
    // Read map grid
    grid.clear();
    std::string line;
    while (std::getline(file, line) && static_cast<int>(grid.size()) < height) {
        std::vector<char> row;
        for (size_t i = 0; i < line.length() && static_cast<int>(i) < width; i++) {
            row.push_back(line[i]);
        }
        // Pad if necessary
        while (static_cast<int>(row.size()) < width) {
            row.push_back(' ');
        }
        grid.push_back(row);
    }
    
    file.close();
    return true;
}

bool Map::saveToFile(const std::string& mapFile) const {
    std::ofstream file(mapFile);
    if (!file.is_open()) {
        return false;
    }
    
    file << regionName << "\n";
    file << width << " " << height << "\n";
    
    for (const auto& row : grid) {
        for (char tile : row) {
            file << tile;
        }
        file << "\n";
    }
    
    file.close();
    return true;
}

char Map::getTileAt(int x, int y) const {
    if (isValidPosition(x, y)) {
        return grid[y][x];
    }
    return ' ';
}

bool Map::isValidPosition(int x, int y) const {
    return x >= 0 && x < width && y >= 0 && y < height;
}

bool Map::canMoveTo(int x, int y) const {
    if (!isValidPosition(x, y)) {
        return false;
    }
    char tile = getTileAt(x, y);
    // Can't move through walls or obstacles
    return tile != '#' && tile != 'M' && tile != 'W';
}

std::string Map::getTileDescription(char tile) const {
    switch(tile) {
        case '.': return "Grass";
        case '#': return "Wall";
        case 'T': return "Town";
        case 'F': return "Forest";
        case 'D': return "Desert";
        case 'M': return "Mountain";
        case 'W': return "Water";
        case 'C': return "Castle";
        case '~': return "Dungeon";
        default: return "Unknown";
    }
}

void Map::display(int playerX, int playerY) const {
    // Title
    std::cout << "\n" << Colors::BRIGHT_CYAN;
    std::cout << "╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "║ " << Colors::BRIGHT_YELLOW << "MAP: " << std::string((48 - regionName.length()) / 2, ' ') 
              << regionName << std::string(48 - (48 - regionName.length()) / 2 - regionName.length(), ' ')
              << Colors::BRIGHT_CYAN << " ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════╝\n" << Colors::RESET;
    
    // Legend row 1
    std::cout << "\n" << Colors::BRIGHT_GREEN << "┌─ LEGEND ─────────────────────────────────────────────────────────┐\n" << Colors::RESET;
    std::cout << Colors::BRIGHT_GREEN << "│ " << Colors::RESET;
    std::cout << Colors::GREEN << "● Grass " << Colors::RESET << "  ";
    std::cout << Colors::BRIGHT_WHITE << "■ Wall " << Colors::RESET << "   ";
    std::cout << Colors::BRIGHT_YELLOW << "☆ Town " << Colors::RESET << "  ";
    std::cout << Colors::GREEN << "▲ Forest " << Colors::RESET << "  ";
    std::cout << Colors::YELLOW << "◆ Desert " << Colors::RESET << " ";
    std::cout << Colors::BRIGHT_GREEN << "│\n" << Colors::RESET;
    
    std::cout << Colors::BRIGHT_GREEN << "│ " << Colors::RESET;
    std::cout << Colors::WHITE << "▲ Mountain " << Colors::RESET << " ";
    std::cout << Colors::BLUE << "~ Water " << Colors::RESET << "   ";
    std::cout << Colors::MAGENTA << "◆ Dungeon " << Colors::RESET << "  ";
    std::cout << Colors::BRIGHT_RED << "✦ Castle " << Colors::RESET << "   ";
    std::cout << Colors::BRIGHT_CYAN << "@ You" << Colors::RESET << "  ";
    std::cout << Colors::BRIGHT_GREEN << "│\n" << Colors::RESET;
    std::cout << Colors::BRIGHT_GREEN << "└──────────────────────────────────────────────────────────────────┘\n" << Colors::RESET;
    
    // Position info
    std::cout << Colors::BRIGHT_YELLOW << "\nCurrent Position: " << Colors::CYAN << "(" << playerX << ", " << playerY << ")" << Colors::RESET;
    std::cout << " | " << Colors::BRIGHT_YELLOW << "Map Size: " << Colors::CYAN << width << "×" << height << Colors::RESET << "\n\n";
    
    // Column indices (tens and units)
    std::cout << "    ";
    if (width > 9) {
        // tens row
        for (int x = 0; x < width; x++) {
            int t = x / 10;
            if (t == 0) std::cout << ' ';
            else std::cout << t;
        }
        std::cout << "\n    ";
    }
    // units row
    for (int x = 0; x < width; x++) std::cout << (x % 10);
    std::cout << "\n";

    // Map border
    std::cout << Colors::BRIGHT_BLUE << "   ┌";
    for (int i = 0; i < width; i++) {
        std::cout << "─";
    }
    std::cout << "┐\n";
    
    for (int y = 0; y < height; y++) {
    // Y coordinate (aligned)
    std::cout << Colors::BRIGHT_BLUE << std::setw(3) << y << " │" << Colors::RESET;
        
        // Map content
        for (int x = 0; x < width; x++) {
            if (x == playerX && y == playerY) {
                // Make player marker stand out
                std::cout << Colors::BRIGHT_MAGENTA << Colors::BRIGHT_WHITE << "@" << Colors::RESET;
            } else {
                char tile = getTileAt(x, y);
                switch(tile) {
                    case '.': std::cout << Colors::GREEN << "·" << Colors::RESET; break;
                    case '#': std::cout << Colors::BRIGHT_WHITE << "█" << Colors::RESET; break;
                    case 'T': std::cout << Colors::BRIGHT_YELLOW << "☆" << Colors::RESET; break;
                    case 'F': std::cout << Colors::GREEN << "▲" << Colors::RESET; break;
                    case 'D': std::cout << Colors::YELLOW << "◆" << Colors::RESET; break;
                    case 'M': std::cout << Colors::WHITE << "▲" << Colors::RESET; break;
                    case 'W': std::cout << Colors::BLUE << "~" << Colors::RESET; break;
                    case '~': std::cout << Colors::MAGENTA << "◆" << Colors::RESET; break;
                    case 'C': std::cout << Colors::BRIGHT_RED << "✦" << Colors::RESET; break;
                    default: std::cout << " ";
                }
            }
        }
        std::cout << Colors::BRIGHT_BLUE << "│\n" << Colors::RESET;
    }
    
    // Bottom border and repeat column indices for readability
    std::cout << Colors::BRIGHT_BLUE << "   └";
    for (int i = 0; i < width; i++) std::cout << "─";
    std::cout << "┘\n" << Colors::RESET;

    if (width > 9) {
        std::cout << "    ";
        for (int x = 0; x < width; x++) {
            int t = x / 10;
            if (t == 0) std::cout << ' ';
            else std::cout << t;
        }
        std::cout << "\n    ";
    }
    for (int x = 0; x < width; x++) std::cout << (x % 10);
    std::cout << "\n\n" << Colors::RESET;
}

void Map::displayStyled(int playerX, int playerY, bool useEmoji) const {
    // Map header
    std::cout << "\n" << Colors::BRIGHT_CYAN;
    std::cout << "╔══════════════════════════════════════════════════╗\n";
    
    // Center the region name
    std::string mapTitle = "🗺️  " + regionName;
    int titlePad = (46 - regionName.length()) / 2;
    std::cout << "║ " << std::string(titlePad, ' ') << Colors::BRIGHT_YELLOW << mapTitle 
              << std::string(46 - titlePad - regionName.length(), ' ') << Colors::BRIGHT_CYAN << " ║\n";
    std::cout << "╚══════════════════════════════════════════════════╝\n" << Colors::RESET;

    if (useEmoji) {
        // Emoji legend
        std::cout << "\n" << Colors::BRIGHT_GREEN << "┌─ LEGEND ─────────────────────────────────────────┐\n";
        std::cout << "│ 🌿 Grass  🧱 Wall  🏘️  Town  🌲 Forest  🏜️  Desert │\n";
        std::cout << "│ ⛰️  Mountain  💧 Water  🕳️  Dungeon  🏰 Castle    │\n";
        std::cout << "│ " << Colors::BRIGHT_CYAN << "⭐ YOU (Current Position)" << Colors::BRIGHT_GREEN << "                       │\n";
        std::cout << "└───────────────────────────────────────────────────┘\n" << Colors::RESET;
    } else {
        // ASCII legend
        std::cout << "\n" << Colors::BRIGHT_GREEN << "┌─ LEGEND ─────────────────────────────────────────┐\n" << Colors::RESET;
        std::cout << Colors::BRIGHT_GREEN << "│ " << Colors::RESET;
        std::cout << Colors::GREEN << ". Grass  " << Colors::RESET;
        std::cout << Colors::BRIGHT_WHITE << "# Wall  " << Colors::RESET;
        std::cout << Colors::BRIGHT_YELLOW << "T Town  " << Colors::RESET;
        std::cout << Colors::GREEN << "F Forest  " << Colors::RESET;
        std::cout << Colors::YELLOW << "D Desert  " << Colors::RESET;
        std::cout << Colors::BRIGHT_GREEN << "│\n" << Colors::RESET;
        std::cout << Colors::BRIGHT_GREEN << "│ " << Colors::RESET;
        std::cout << Colors::WHITE << "M Mountain  " << Colors::RESET;
        std::cout << Colors::BLUE << "~ Water  " << Colors::RESET;
        std::cout << Colors::MAGENTA << "* Dungeon  " << Colors::RESET;
        std::cout << Colors::BRIGHT_RED << "C Castle  " << Colors::RESET;
        std::cout << Colors::BRIGHT_CYAN << "@ YOU" << Colors::RESET << " ";
        std::cout << Colors::BRIGHT_GREEN << "│\n" << Colors::RESET;
        std::cout << Colors::BRIGHT_GREEN << "└───────────────────────────────────────────────────┘\n" << Colors::RESET;
    }

    // Position info
    std::cout << Colors::BRIGHT_YELLOW << "\n📍 Position: " << Colors::CYAN << "(" << playerX << ", " << playerY << ")" << Colors::RESET;
    std::cout << "  " << Colors::BRIGHT_YELLOW << "📐 Size: " << Colors::CYAN << width << "x" << height << Colors::RESET << "\n\n";

    if (useEmoji) {
        // Emoji map - each emoji takes 2 columns
        // Column indices header (spaced for emoji width)
        std::cout << "      ";
        for (int x = 0; x < width; x++) {
            std::cout << (x % 10) << " ";
        }
        std::cout << "\n";

        // Top border
        std::cout << Colors::BRIGHT_BLUE << "     +" << std::string(width * 2, '-') << "+\n";

        // Map rows with emoji
        for (int y = 0; y < height; y++) {
            std::cout << Colors::BRIGHT_BLUE << std::setw(4) << y << " |" << Colors::RESET;
            
            for (int x = 0; x < width; x++) {
                if (x == playerX && y == playerY) {
                    std::cout << "⭐";
                } else {
                    char tile = getTileAt(x, y);
                    switch(tile) {
                        case '.': std::cout << "🌿"; break;
                        case '#': std::cout << "🧱"; break;
                        case 'T': std::cout << "🏘️ "; break;
                        case 'F': std::cout << "🌲"; break;
                        case 'D': std::cout << "🏜️ "; break;
                        case 'M': std::cout << "⛰️ "; break;
                        case 'W': std::cout << "💧"; break;
                        case '~': std::cout << "🕳️ "; break;
                        case 'C': std::cout << "🏰"; break;
                        default:  std::cout << "  ";
                    }
                }
            }
            std::cout << Colors::BRIGHT_BLUE << "|\n" << Colors::RESET;
        }

        // Bottom border
        std::cout << Colors::BRIGHT_BLUE << "     +" << std::string(width * 2, '-') << "+\n" << Colors::RESET;
    } else {
        // ASCII map (original)
        std::cout << "     ";
        for (int x = 0; x < width; x++) {
            std::cout << (x % 10);
        }
        std::cout << "\n";

        std::cout << Colors::BRIGHT_BLUE << "    +" << std::string(width, '-') << "+\n";

        for (int y = 0; y < height; y++) {
            std::cout << Colors::BRIGHT_BLUE << std::setw(3) << y << " |" << Colors::RESET;
            
            for (int x = 0; x < width; x++) {
                if (x == playerX && y == playerY) {
                    std::cout << Colors::BRIGHT_CYAN << "@" << Colors::RESET;
                } else {
                    char tile = getTileAt(x, y);
                    switch(tile) {
                        case '.': std::cout << Colors::GREEN << "." << Colors::RESET; break;
                        case '#': std::cout << Colors::BRIGHT_WHITE << "#" << Colors::RESET; break;
                        case 'T': std::cout << Colors::BRIGHT_YELLOW << "T" << Colors::RESET; break;
                        case 'F': std::cout << Colors::GREEN << "F" << Colors::RESET; break;
                        case 'D': std::cout << Colors::YELLOW << "D" << Colors::RESET; break;
                        case 'M': std::cout << Colors::WHITE << "M" << Colors::RESET; break;
                        case 'W': std::cout << Colors::BLUE << "~" << Colors::RESET; break;
                        case '~': std::cout << Colors::MAGENTA << "*" << Colors::RESET; break;
                        case 'C': std::cout << Colors::BRIGHT_RED << "C" << Colors::RESET; break;
                        default: std::cout << " ";
                    }
                }
            }
            std::cout << Colors::BRIGHT_BLUE << "|\n" << Colors::RESET;
        }

        std::cout << Colors::BRIGHT_BLUE << "    +" << std::string(width, '-') << "+\n" << Colors::RESET;
    }

    std::cout << "\n";
}

void Map::displayFull() const {
    display(-1, -1); // Display without player
}

void Map::displayMinimap(int playerX, int playerY, int viewRange) const {
    std::cout << "\n" << Colors::BRIGHT_CYAN;
    std::cout << "╔═══════════════════════════════════════╗\n";
    std::cout << "║ " << Colors::BRIGHT_YELLOW << "MINIMAP" << Colors::BRIGHT_CYAN 
              << std::string(27, ' ') << "║\n";
    std::cout << "╚═══════════════════════════════════════╝\n" << Colors::RESET;
    
    int startX = playerX - viewRange;
    int startY = playerY - viewRange;
    int endX = playerX + viewRange;
    int endY = playerY + viewRange;
    
    std::cout << Colors::BRIGHT_BLUE << "   ┌";
    for (int i = startX; i <= endX; i++) {
        std::cout << "─";
    }
    std::cout << "┐\n";
    
    for (int y = startY; y <= endY; y++) {
        std::cout << Colors::BRIGHT_BLUE << "   │" << Colors::RESET;
        
        for (int x = startX; x <= endX; x++) {
            if (x == playerX && y == playerY) {
                std::cout << Colors::BRIGHT_CYAN << "@" << Colors::RESET;
            } else if (isValidPosition(x, y)) {
                char tile = getTileAt(x, y);
                switch(tile) {
                    case '.': std::cout << Colors::GREEN << "·" << Colors::RESET; break;
                    case '#': std::cout << Colors::BRIGHT_WHITE << "█" << Colors::RESET; break;
                    case 'T': std::cout << Colors::BRIGHT_YELLOW << "☆" << Colors::RESET; break;
                    case 'F': std::cout << Colors::GREEN << "▲" << Colors::RESET; break;
                    case 'D': std::cout << Colors::YELLOW << "◆" << Colors::RESET; break;
                    case 'M': std::cout << Colors::WHITE << "▲" << Colors::RESET; break;
                    case 'W': std::cout << Colors::BLUE << "~" << Colors::RESET; break;
                    case '~': std::cout << Colors::MAGENTA << "◆" << Colors::RESET; break;
                    case 'C': std::cout << Colors::BRIGHT_RED << "✦" << Colors::RESET; break;
                    default: std::cout << " ";
                }
            } else {
                std::cout << " ";
            }
        }
        std::cout << Colors::BRIGHT_BLUE << "│\n" << Colors::RESET;
    }
    
    std::cout << Colors::BRIGHT_BLUE << "   └";
    for (int i = startX; i <= endX; i++) {
        std::cout << "─";
    }
    std::cout << "┘\n" << Colors::RESET << "\n";
}

void Map::generateDefaultMap(const std::string& region) {
    regionName = region;
    width = 20;
    height = 20;
    
    grid.clear();
    
    // Generate based on region type
    if (region == "Verdant Woods") {
        for (int y = 0; y < height; y++) {
            std::vector<char> row;
            for (int x = 0; x < width; x++) {
                if (x == 0 || x == width-1 || y == 0 || y == height-1) {
                    row.push_back('#');
                } else if (x == 10 && y == 10) {
                    row.push_back('T'); // Town in center
                } else if ((x + y) % 7 == 0) {
                    row.push_back('F'); // Forest patches
                } else {
                    row.push_back('.');
                }
            }
            grid.push_back(row);
        }
    } else if (region == "Scorched Dunes") {
        for (int y = 0; y < height; y++) {
            std::vector<char> row;
            for (int x = 0; x < width; x++) {
                if (x == 0 || x == width-1 || y == 0 || y == height-1) {
                    row.push_back('#');
                } else if (x == 15 && y == 15) {
                    row.push_back('~'); // Dungeon
                } else if ((x + y) % 5 == 0) {
                    row.push_back('M'); // Mountains
                } else {
                    row.push_back('D');
                }
            }
            grid.push_back(row);
        }
    } else if (region == "Frost Peaks") {
        for (int y = 0; y < height; y++) {
            std::vector<char> row;
            for (int x = 0; x < width; x++) {
                if (x == 0 || x == width-1 || y == 0 || y == height-1) {
                    row.push_back('#');
                } else if (x == 5 && y == 5) {
                    row.push_back('T'); // Town
                } else if ((x + y) % 6 == 0) {
                    row.push_back('M'); // Mountains
                } else {
                    row.push_back('.');
                }
            }
            grid.push_back(row);
        }
    } else if (region == "Dark Citadel") {
        for (int y = 0; y < height; y++) {
            std::vector<char> row;
            for (int x = 0; x < width; x++) {
                if (x == 0 || x == width-1 || y == 0 || y == height-1) {
                    row.push_back('#');
                } else if (x == 10 && y == 10) {
                    row.push_back('C'); // Final boss castle
                } else if ((x + y) % 4 == 0) {
                    row.push_back('#'); // More walls
                } else {
                    row.push_back('.');
                }
            }
            grid.push_back(row);
        }
    } else {
        // Default map
        for (int y = 0; y < height; y++) {
            std::vector<char> row;
            for (int x = 0; x < width; x++) {
                if (x == 0 || x == width-1 || y == 0 || y == height-1) {
                    row.push_back('#');
                } else {
                    row.push_back('.');
                }
            }
            grid.push_back(row);
        }
    }
}

char Map::getTile(int x, int y) const {
    return getTileAt(x, y);
}

void Map::setTile(int x, int y, char tile) {
    if (isValidPosition(x, y)) {
        grid[y][x] = tile;
    }
}

