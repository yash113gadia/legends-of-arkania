#include "Game.h"
#include "Colors.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <cctype>
#include <vector>
#include <string>
#include <limits>
#include <iomanip>
#include <unistd.h>
#include <mach-o/dyld.h>
#include <libgen.h>

// Get the directory where the executable is located
static std::string getExecutableDir() {
    char path[1024];
    uint32_t size = sizeof(path);
    if (_NSGetExecutablePath(path, &size) == 0) {
        char* dir = dirname(path);
        return std::string(dir);
    }
    return ".";
}

static std::string executableDir = getExecutableDir();

Game::Game() : player(nullptr), currentRegion("Verdant Woods"), shop(nullptr), gameRunning(false) {
    srand(time(nullptr));
    initializeRegions();
    shop = new Shop("Adventurer's Emporium");
}

Game::~Game() {
    delete player;
    delete shop;
    for (auto& pair : regions) {
        delete pair.second;
    }
}

void Game::initializeRegions() {
    // Try to load maps from files, or generate defaults
    std::vector<std::string> regionNames = {
        "Verdant Woods",
        "Scorched Dunes",
        "Frost Peaks",
        "Dark Citadel"
    };
    
    for (const auto& regionName : regionNames) {
        Map* map = new Map();
        // Use executable directory to find maps folder
        std::string filename = executableDir + "/maps/" + regionName + ".txt";
        
        if (!map->loadFromFile(filename)) {
            // Generate default map if file doesn't exist
            map->generateDefaultMap(regionName);
            // Save it for future use
            map->saveToFile(filename);
        }
        
        regions[regionName] = map;
    }
}

void Game::run() {
    Colors::printTitle(Colors::colorize("⚔ LEGENDS OF ARKANIA ⚔", Colors::BRIGHT_YELLOW));
    std::cout << Colors::colorize("🌍 A Turn-Based RPG Adventure in a Fantasy World 🌍", Colors::CYAN) << "\n\n";
    
    displayMainMenu();
    
    if (!player) {
        return;
    }
    
    gameRunning = true;
    currentRegion = player->getCurrentRegion();
    
    // Set initial position if new game
    if (player->getX() == 0 && player->getY() == 0) {
        player->setPosition(1, 1);
    }
    
    std::cout << "\nYou find yourself in " << currentRegion << "...\n";
    regions[currentRegion]->displayStyled(player->getX(), player->getY(), true);
    
    while (gameRunning && player->getHealth() > 0) {
        // In-Game Menu UI with emojis
        std::cout << "\n" << Colors::BRIGHT_CYAN;
        std::cout << "╔══════════════════════════════════════════════════╗\n";
        std::cout << "║               🎮 ACTIONS 🎮                      ║\n";
        std::cout << "╠══════════════════════════════════════════════════╣\n";
        std::cout << "║  " << Colors::YELLOW << "[W/A/S/D]" << Colors::WHITE << " 🚶 Move                            " << Colors::BRIGHT_CYAN << "║\n";
        std::cout << "║  " << Colors::YELLOW << "[I]      " << Colors::WHITE << " 🎒 Inventory                       " << Colors::BRIGHT_CYAN << "║\n";
        std::cout << "║  " << Colors::YELLOW << "[P]      " << Colors::WHITE << " 📜 Player Stats                    " << Colors::BRIGHT_CYAN << "║\n";
        std::cout << "║  " << Colors::YELLOW << "[M]      " << Colors::WHITE << " 🗺️  Map                             " << Colors::BRIGHT_CYAN << "║\n";
        std::cout << "║  " << Colors::YELLOW << "[H]      " << Colors::WHITE << " ❓ Help                            " << Colors::BRIGHT_CYAN << "║\n";
        std::cout << "║  " << Colors::BRIGHT_RED << "[Q]      " << Colors::WHITE << " 🚪 Quit                            " << Colors::BRIGHT_CYAN << "║\n";
        std::cout << "╚══════════════════════════════════════════════════╝\n" << Colors::RESET;
        
        std::cout << Colors::BRIGHT_GREEN << "Command: " << Colors::RESET;
        
        // Read a full line from input to avoid leftover-newline issues when mixing >> and getline
        std::string inputLine;
        std::getline(std::cin, inputLine);

        // If getline fails (EOF or error), handle gracefully
        if (!std::cin) {
            if (std::cin.eof()) {
                std::cout << "\nInput stream closed (EOF). Exiting game.\n";
                gameRunning = false;
                break;
            } else {
                std::cin.clear();
                std::cout << "\nInput error detected. Please try again.\n";
                continue;
            }
        }

        // Trim leading spaces
        size_t pos = inputLine.find_first_not_of(" \t\r\n");
        if (pos == std::string::npos) {
            // Empty line — prompt again
            continue;
        }

        char command = std::toupper(static_cast<unsigned char>(inputLine[pos]));
        
        switch(command) {
            case 'W':
            case 'A':
            case 'S':
            case 'D':
                handleMovement(command);
                break;
            case 'I': {
                player->displayInventory();
                std::cout << Colors::BRIGHT_YELLOW << "\nUse an item? " << Colors::WHITE << "(enter name or 'no'): " << Colors::RESET;
                std::string itemName;
                std::getline(std::cin, itemName);
                // Trim whitespace
                size_t start = itemName.find_first_not_of(" \t\r\n");
                size_t end = itemName.find_last_not_of(" \t\r\n");
                if (start != std::string::npos) {
                    itemName = itemName.substr(start, end - start + 1);
                }
                if (itemName != "no" && !itemName.empty()) {
                    player->useItem(itemName);
                }
                break;
            }
            case 'P':
                player->displayStats();
                break;
            case 'M':
                regions[currentRegion]->displayStyled(player->getX(), player->getY(), true);
                break;
            case 'H':
                displayHelp();
                break;
            case 'Q': {
                std::cout << "Save game? (y/n): ";
                std::string _line;
                std::getline(std::cin, _line);
                if (!std::cin) {
                    if (std::cin.eof()) { gameRunning = false; break; }
                    std::cin.clear();
                }
                size_t _p = _line.find_first_not_of(" \t\r\n");
                char save = (_p == std::string::npos) ? 'n' : std::toupper(static_cast<unsigned char>(_line[_p]));
                if (save == 'Y') {
                    saveGame();
                }
                gameRunning = false;
                break;
            }
            default:
                std::cout << "Invalid command.\n";
        }
        
        // Passive slow healing (regenerate 1-2% of max HP per turn when not at full health)
        if (player->getHealth() > 0 && player->getHealth() < player->getMaxHealth()) {
            int regenAmount = std::max(1, player->getMaxHealth() / 50); // ~2% per turn, minimum 1 HP
            int oldHealth = player->getHealth();
            player->heal(regenAmount);
            if (player->getHealth() > oldHealth) {
                std::cout << Colors::GREEN << "💚 You recover " << (player->getHealth() - oldHealth) 
                          << " HP (passive regen). Health: " << player->getHealth() 
                          << "/" << player->getMaxHealth() << Colors::RESET << "\n";
            }
        }

        // Check win condition
        if (currentRegion == "Dark Citadel" && 
            regions[currentRegion]->getTileAt(player->getX(), player->getY()) == 'C') {
            std::cout << "\n" << Colors::BRIGHT_GREEN;
            std::cout << "╔══════════════════════════════════════╗\n";
            std::cout << "║   " << Colors::BRIGHT_YELLOW << "★ VICTORY! ★" << Colors::BRIGHT_GREEN << "                     ║\n";
            std::cout << "║   You have conquered the Dark Citadel ║\n";
            std::cout << "║   Peace has been restored to Arkania! ║\n";
            std::cout << "╚══════════════════════════════════════╝\n" << Colors::RESET;
            gameRunning = false;
        }
    }
    
    if (player->getHealth() <= 0) {
        std::cout << "\n" << Colors::BRIGHT_RED;
        std::cout << "╔══════════════════════════════════════╗\n";
        std::cout << "║   Game Over! You have been defeated...║\n";
        std::cout << "╚══════════════════════════════════════╝\n" << Colors::RESET;
    }
}

void Game::displayMainMenu() {
    const int width = 50;
    std::string border;
    for (int i = 0; i < width - 2; ++i) border += "─";
    
    std::cout << "\n" << Colors::BRIGHT_YELLOW;
    std::cout << "┌" << border << "┐\n";
    
    std::string title = " MAIN MENU ";
    int pad = (width - 2 - title.length()) / 2;
    std::cout << "│" << std::string(pad, ' ') << Colors::BOLD << title << Colors::RESET << Colors::BRIGHT_YELLOW << std::string(width - 2 - pad - title.length(), ' ') << "│\n";
    
    std::cout << "├" << border << "┤\n";
    std::cout << "│" << std::string(width - 2, ' ') << "│\n"; // spacer
    
    // Option 1
    std::string opt1 = "1. New Game";
    std::cout << "│  " << Colors::CYAN << Colors::Emoji::SWORD << " " << std::left << std::setw(width - 8) << opt1 << Colors::BRIGHT_YELLOW << "│\n";
    
    // Option 2
    std::string opt2 = "2. Load Game";
    std::cout << "│  " << Colors::CYAN << Colors::Emoji::SCROLL << " " << std::left << std::setw(width - 8) << opt2 << Colors::BRIGHT_YELLOW << "│\n";
    
    // Option 3
    std::string opt3 = "3. Exit";
    std::cout << "│  " << Colors::RED << Colors::Emoji::CROSS << " " << std::left << std::setw(width - 8) << opt3 << Colors::BRIGHT_YELLOW << "│\n";
    
    std::cout << "│" << std::string(width - 2, ' ') << "│\n"; // spacer
    std::cout << "└" << border << "┘\n" << Colors::RESET;
    std::cout << Colors::BRIGHT_GREEN << "Choice: " << Colors::RESET;
    
    int choice = 0;
    {
        std::string line;
        std::getline(std::cin, line);
        if (!std::cin) {
            if (std::cin.eof()) return;
            std::cin.clear();
        }

        // Trim and parse
        size_t p = line.find_first_not_of(" \t\r\n");
        if (p != std::string::npos) {
            try {
                choice = std::stoi(line.substr(p));
            } catch (...) {
                choice = 0;
            }
        }
    }

    switch(choice) {
        case 1:
            createNewGame();
            break;
        case 2:
            if (!loadGame()) {
                std::cout << "No save file found. Starting new game...\n";
                createNewGame();
            }
            break;
        case 3:
            std::cout << "Goodbye!\n";
            break;
        default:
            std::cout << "Invalid choice. Starting new game...\n";
            createNewGame();
    }
}

void Game::createNewGame() {
    Colors::printTitle(Colors::colorize("⚔ CREATE CHARACTER ⚔", Colors::BRIGHT_YELLOW));
    std::cout << Colors::CYAN << "👤 Enter your name: " << Colors::WHITE;
    std::string name;
    std::cin.ignore();
    std::getline(std::cin, name);
    
    std::cout << "\n" << Colors::BRIGHT_YELLOW << "Choose your class:\n" << Colors::RESET;
    std::cout << Colors::Emoji::WARRIOR << Colors::WHITE << "1.Warrior " << Colors::DARK_GRAY << "(High HP, High Strength, Low Mana)\n";
    std::cout << Colors::Emoji::MAGE << " " << Colors::WHITE << "2.Mage " << Colors::DARK_GRAY << "(Low HP, High Mana, Magic Abilities)\n";
    std::cout << Colors::Emoji::ARCHER << " " << Colors::WHITE << "3.Archer " << Colors::DARK_GRAY << "(Balanced, High Agility)\n";
    std::cout << Colors::BRIGHT_GREEN << "🎮 Choice: " << Colors::RESET;
    
    int classChoice = 0;
    {
        std::string line;
        std::getline(std::cin, line);
        if (!std::cin) {
            if (std::cin.eof()) return;
            std::cin.clear();
        }
        size_t p = line.find_first_not_of(" \t\r\n");
        if (p != std::string::npos) {
            try {
                classChoice = std::stoi(line.substr(p));
            } catch (...) {
                classChoice = 1; // default to warrior on parse failure
            }
        } else {
            classChoice = 1;
        }
    }
    
    PlayerClass pClass;
    switch(classChoice) {
        case 1: pClass = PlayerClass::WARRIOR; break;
        case 2: pClass = PlayerClass::MAGE; break;
        case 3: pClass = PlayerClass::ARCHER; break;
        default: pClass = PlayerClass::WARRIOR; break;
    }
    
    player = new Player(name, pClass);
    player->setPosition(1, 1);
    player->setRegion("Verdant Woods");
    
    // Play animated intro story
    Colors::playIntroStory(name, player->getClassName());
    
    std::cout << Colors::BRIGHT_GREEN << "\n✨ Welcome, " << Colors::BRIGHT_WHITE << name 
              << Colors::BRIGHT_GREEN << " the " << Colors::BRIGHT_YELLOW << player->getClassName() 
              << Colors::BRIGHT_GREEN << "! ✨\n" << Colors::RESET;
    player->displayStats();
}

bool Game::loadGame() {
    player = new Player("", PlayerClass::WARRIOR);
    std::string savePath = executableDir + "/savegame.txt";
    if (player->loadFromFile(savePath)) {
        currentRegion = player->getCurrentRegion();
        std::cout << "\nGame loaded successfully!\n";
        player->displayStats();
        return true;
    }
    delete player;
    player = nullptr;
    return false;
}

void Game::saveGame() {
    if (player) {
        std::string savePath = executableDir + "/savegame.txt";
        player->saveToFile(savePath);
        std::cout << "Game saved!\n";
    }
}

void Game::handleMovement(char direction) {
    int newX = player->getX();
    int newY = player->getY();
    
    switch(direction) {
        case 'W': newY--; break;
        case 'S': newY++; break;
        case 'A': newX--; break;
        case 'D': newX++; break;
    }
    
    Map* currentMap = regions[currentRegion];
    
    if (currentMap->canMoveTo(newX, newY)) {
        player->setPosition(newX, newY);
        char tile = currentMap->getTileAt(newX, newY);
        
        std::cout << "\n" << Colors::BRIGHT_GREEN << "💫 ";
        std::cout << "You move to " << currentMap->getTileDescription(tile) << ".\n" << Colors::RESET;
        
        // Handle special tiles
        if (tile == 'T') {
            handleTownInteraction();
        } else if (tile == '~') {
            handleDungeon();
        } else if (tile == 'C') {
            handleCastle();
        } else {
            // Random encounter chance
            if (rand() % 100 < 25) { // 25% chance
                handleRandomEncounter();
            }
        }
        
        // Display map after every move
    currentMap->displayStyled(newX, newY, true);
    } else {
        std::cout << Colors::BRIGHT_RED << "❌ You can't move there!\n" << Colors::RESET;
    }
}

void Game::handleRandomEncounter() {
    Enemy* enemy = generateRandomEnemy();
    Battle battle(player, enemy);
    bool playerWon = battle.start();
    delete enemy;
    
    if (!playerWon) {
        // Player lost - restore some health and continue
        player->heal(player->getMaxHealth() / 2);
        std::cout << "You wake up at the edge of the region, weakened but alive...\n";
        player->setPosition(1, 1);
    }
}

Enemy* Game::generateRandomEnemy() {
    std::vector<std::string> enemyNames;
    int enemyLevel = player->getLevel() + (rand() % 3) - 1; // ±1 level variation
    enemyLevel = std::max(1, enemyLevel);
    
    if (currentRegion == "Verdant Woods") {
        enemyNames = {"Goblin", "Wolf", "Bandit", "Forest Troll"};
    } else if (currentRegion == "Scorched Dunes") {
        enemyNames = {"Sand Worm", "Desert Bandit", "Scorpion", "Fire Elemental"};
    } else if (currentRegion == "Frost Peaks") {
        enemyNames = {"Ice Golem", "Frost Wolf", "Yeti", "Ice Mage"};
    } else if (currentRegion == "Dark Citadel") {
        enemyNames = {"Dark Knight", "Shadow Demon", "Necromancer", "Dark Lord"};
        enemyLevel = std::max(enemyLevel, player->getLevel() + 2);
    } else {
        enemyNames = {"Monster", "Enemy", "Foe"};
    }
    
    std::string enemyName = enemyNames[rand() % enemyNames.size()];
    return new Enemy(enemyName, enemyLevel, currentRegion);
}

void Game::handleTownInteraction() {
    std::cout << "\n" << Colors::BRIGHT_CYAN;
    std::cout << "╔════════════════════════════════════════════════╗\n";
    std::cout << "║           🏘️  WELCOME TO TOWN 🏘️               ║\n";
    std::cout << "╠════════════════════════════════════════════════╣\n";
    std::cout << "║  " << Colors::YELLOW << "1." << Colors::WHITE << " 🛒 Visit Shop                            " << Colors::BRIGHT_CYAN << "║\n";
    std::cout << "║  " << Colors::YELLOW << "2." << Colors::WHITE << " 🛏️  Rest at Inn (Restore HP/MP)          " << Colors::BRIGHT_CYAN << "║\n";
    std::cout << "║  " << Colors::YELLOW << "3." << Colors::WHITE << " 🚪 Leave Town                            " << Colors::BRIGHT_CYAN << "║\n";
    std::cout << "╚════════════════════════════════════════════════╝\n" << Colors::RESET;
    std::cout << Colors::BRIGHT_GREEN << "🎮 Choice: " << Colors::RESET;
    
    int choice = 0;
    {
        std::string line;
        std::getline(std::cin, line);
        if (!std::cin) {
            if (std::cin.eof()) return;
            std::cin.clear();
        }
        size_t p = line.find_first_not_of(" \t\r\n");
        if (p != std::string::npos) {
            try { choice = std::stoi(line.substr(p)); } catch (...) { choice = 3; }
        } else { choice = 3; }
    }

    switch(choice) {
        case 1: {
            shop->displayShop(player);
            std::cout << Colors::BRIGHT_YELLOW << "Enter item name to buy " << Colors::WHITE << "(or 'leave'): " << Colors::RESET;
            std::string itemName;
            std::getline(std::cin, itemName);
            // Trim
            size_t start = itemName.find_first_not_of(" \t\r\n");
            size_t end = itemName.find_last_not_of(" \t\r\n");
            if (start != std::string::npos) {
                itemName = itemName.substr(start, end - start + 1);
            }
            if (itemName != "leave" && !itemName.empty()) {
                shop->buyItem(player, itemName);
            }
            break;
        }
        case 2:
            player->heal(player->getMaxHealth());
            player->restoreMana(player->getMaxMana());
            Colors::animateLoading("Resting at the inn", 1000);
            std::cout << Colors::BRIGHT_GREEN << "💤 You rest at the inn and restore all health and mana!\n" << Colors::RESET;
            break;
        case 3:
        default:
            std::cout << Colors::CYAN << "👋 You leave the town.\n" << Colors::RESET;
            break;
    }
}

void Game::handleDungeon() {
    std::cout << "\n" << Colors::BRIGHT_MAGENTA;
    std::cout << "╔════════════════════════════════════════════════╗\n";
    std::cout << "║            🕳️  DUNGEON ENTRANCE 🕳️             ║\n";
    std::cout << "╠════════════════════════════════════════════════╣\n";
    std::cout << "║  " << Colors::WHITE << "A dark dungeon entrance looms before you..." << Colors::BRIGHT_MAGENTA << "  ║\n";
    std::cout << "║  " << Colors::YELLOW << "⚠️  Warning: Multiple battles await inside!" << Colors::BRIGHT_MAGENTA << "   ║\n";
    std::cout << "╚════════════════════════════════════════════════╝\n" << Colors::RESET;
    std::cout << Colors::BRIGHT_YELLOW << "🚪 Enter the dungeon? " << Colors::WHITE << "(y/n): " << Colors::RESET;
    {
        std::string line;
        std::getline(std::cin, line);
        if (!std::cin) {
            if (std::cin.eof()) return;
            std::cin.clear();
        }
        size_t p = line.find_first_not_of(" \t\r\n");
        char choice = (p == std::string::npos) ? 'n' : std::toupper(static_cast<unsigned char>(line[p]));
        if (choice == 'Y') {
            std::cout << Colors::BRIGHT_MAGENTA << "\nYou venture into the darkness...\n\n" << Colors::RESET;
            // Multiple battles in dungeon
            int battles = 2 + (rand() % 3);
            for (int i = 0; i < battles; i++) {
                std::cout << Colors::BRIGHT_CYAN << "--- Battle " << (i + 1) << " of " << battles << " ---\n" << Colors::RESET;
                Enemy* enemy = generateRandomEnemy();
                Battle battle(player, enemy);
                bool won = battle.start();
                delete enemy;
                
                if (!won) {
                    std::cout << Colors::BRIGHT_RED << "You retreat from the dungeon...\n" << Colors::RESET;
                    return;
                }
            }
            
            // Dungeon reward
            int goldReward = 100 + (rand() % 100);
            player->addGold(goldReward);
            std::cout << "\n" << Colors::BRIGHT_YELLOW;
            std::cout << "╔════════════════════════════════════════════════╗\n";
            std::cout << "║           💎 TREASURE FOUND! 💎                ║\n";
            std::cout << "╚════════════════════════════════════════════════╝\n" << Colors::RESET;
            std::cout << Colors::BRIGHT_GREEN << "💰 You found " << goldReward << " gold in a treasure chest!\n" << Colors::RESET;
        }
    }
}

void Game::handleCastle() {
    if (currentRegion == "Dark Citadel") {
        std::cout << "\n" << Colors::BRIGHT_RED;
        std::cout << "╔════════════════════════════════════════════════╗\n";
        std::cout << "║          🏰 THE DARK CITADEL 🏰                ║\n";
        std::cout << "╠════════════════════════════════════════════════╣\n";
        std::cout << "║  " << Colors::WHITE << "⚔️  The Dark Lord awaits within..." << Colors::BRIGHT_RED << "           ║\n";
        std::cout << "║  " << Colors::YELLOW << "💀 This is the FINAL BATTLE!" << Colors::BRIGHT_RED << "                 ║\n";
        std::cout << "╚════════════════════════════════════════════════╝\n" << Colors::RESET;
        Colors::typewriter("Do you dare enter and face your destiny? ", 30);
        std::cout << Colors::WHITE << "(y/n): " << Colors::RESET;
        {
            std::string line;
            std::getline(std::cin, line);
            if (!std::cin) {
                if (std::cin.eof()) return;
                std::cin.clear();
            }
            size_t p = line.find_first_not_of(" \t\r\n");
            char choice = (p == std::string::npos) ? 'n' : std::toupper(static_cast<unsigned char>(line[p]));
            if (choice == 'Y') {
                Enemy* finalBoss = new Enemy("Dark Lord", player->getLevel() + 5, currentRegion);
                Battle battle(player, finalBoss);
                battle.start();
                delete finalBoss;
            }
        }
    } else {
        std::cout << "\n" << Colors::BRIGHT_CYAN;
        std::cout << "╔════════════════════════════════════════════════╗\n";
        std::cout << "║               🏰 CASTLE 🏰                     ║\n";
        std::cout << "╚════════════════════════════════════════════════╝\n" << Colors::RESET;
        std::cout << Colors::WHITE << "A grand castle stands before you, but it's locked.\n" << Colors::RESET;
    }
}

void Game::displayHelp() {
    std::cout << "\n" << Colors::BRIGHT_YELLOW;
    std::cout << "╔══════════════════════════════════════════════════╗\n";
    std::cout << "║                  ❓ HELP ❓                       ║\n";
    std::cout << "╠══════════════════════════════════════════════════╣\n" << Colors::RESET;
    
    std::cout << Colors::BRIGHT_YELLOW << "║ " << Colors::BRIGHT_CYAN << "🚶 MOVEMENT:" << std::string(36, ' ') << Colors::BRIGHT_YELLOW << "║\n";
    std::cout << Colors::BRIGHT_YELLOW << "║   " << Colors::WHITE << "W - ⬆️  North    S - ⬇️  South" << std::string(13, ' ') << Colors::BRIGHT_YELLOW << "║\n";
    std::cout << Colors::BRIGHT_YELLOW << "║   " << Colors::WHITE << "A - ⬅️  West     D - ➡️  East" << std::string(13, ' ') << Colors::BRIGHT_YELLOW << "║\n";
    
    std::cout << Colors::BRIGHT_YELLOW << "╠──────────────────────────────────────────────────╣\n";
    
    std::cout << Colors::BRIGHT_YELLOW << "║ " << Colors::BRIGHT_CYAN << "🗺️  MAP TILES:" << std::string(34, ' ') << Colors::BRIGHT_YELLOW << "║\n";
    std::cout << Colors::BRIGHT_YELLOW << "║   " << Colors::GREEN << "🌿 Grass" << Colors::WHITE << " - Safe terrain" << std::string(19, ' ') << Colors::BRIGHT_YELLOW << "║\n";
    std::cout << Colors::BRIGHT_YELLOW << "║   " << Colors::BRIGHT_YELLOW << "🏘️  Town" << Colors::WHITE << "  - Shop & rest" << std::string(19, ' ') << Colors::BRIGHT_YELLOW << "║\n";
    std::cout << Colors::BRIGHT_YELLOW << "║   " << Colors::GREEN << "🌲 Forest" << Colors::WHITE << "- May encounter enemies" << std::string(12, ' ') << Colors::BRIGHT_YELLOW << "║\n";
    std::cout << Colors::BRIGHT_YELLOW << "║   " << Colors::MAGENTA << "🕳️  Dungeon" << Colors::WHITE << " - Multiple battles" << std::string(13, ' ') << Colors::BRIGHT_YELLOW << "║\n";
    std::cout << Colors::BRIGHT_YELLOW << "║   " << Colors::BRIGHT_RED << "🏰 Castle" << Colors::WHITE << " - Final boss location" << std::string(11, ' ') << Colors::BRIGHT_YELLOW << "║\n";
    std::cout << Colors::BRIGHT_YELLOW << "║   " << Colors::BRIGHT_WHITE << "🧱 Wall" << Colors::WHITE << "   - Cannot pass" << std::string(19, ' ') << Colors::BRIGHT_YELLOW << "║\n";
    
    std::cout << Colors::BRIGHT_YELLOW << "╠──────────────────────────────────────────────────╣\n";
    
    std::cout << Colors::BRIGHT_YELLOW << "║ " << Colors::BRIGHT_CYAN << "⚔️  COMBAT:" << std::string(37, ' ') << Colors::BRIGHT_YELLOW << "║\n";
    std::cout << Colors::BRIGHT_YELLOW << "║   " << Colors::WHITE << "1 - ⚔️  Attack (deal damage)" << std::string(15, ' ') << Colors::BRIGHT_YELLOW << "║\n";
    std::cout << Colors::BRIGHT_YELLOW << "║   " << Colors::WHITE << "2 - ✨ Skills (special abilities)" << std::string(10, ' ') << Colors::BRIGHT_YELLOW << "║\n";
    std::cout << Colors::BRIGHT_YELLOW << "║   " << Colors::WHITE << "3 - 🛡️  Defend (reduce damage)" << std::string(12, ' ') << Colors::BRIGHT_YELLOW << "║\n";
    std::cout << Colors::BRIGHT_YELLOW << "║   " << Colors::WHITE << "4 - 🧪 Item (use potions)" << std::string(18, ' ') << Colors::BRIGHT_YELLOW << "║\n";
    
    std::cout << Colors::BRIGHT_YELLOW << "╠──────────────────────────────────────────────────╣\n";
    
    std::cout << Colors::BRIGHT_YELLOW << "║ " << Colors::BRIGHT_CYAN << "🎯 GOAL:" << std::string(40, ' ') << Colors::BRIGHT_YELLOW << "║\n";
    std::cout << Colors::BRIGHT_YELLOW << "║   " << Colors::WHITE << "Explore regions, level up, and defeat" << std::string(7, ' ') << Colors::BRIGHT_YELLOW << "║\n";
    std::cout << Colors::BRIGHT_YELLOW << "║   " << Colors::WHITE << "the Dark Lord in the Dark Citadel!" << std::string(10, ' ') << Colors::BRIGHT_YELLOW << "║\n";
    
    std::cout << Colors::BRIGHT_YELLOW << "╚══════════════════════════════════════════════════╝\n" << Colors::RESET;
}

