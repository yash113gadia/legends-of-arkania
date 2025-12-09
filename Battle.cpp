#include "Battle.h"
#include "Colors.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <iomanip>

Battle::Battle(Player* p, Enemy* e) : player(p), enemy(e), playerTurn(true) {
    srand(time(nullptr));
}

bool Battle::start() {
    std::cout << "\n" << Colors::BRIGHT_RED;
    std::cout << "╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                    ⚔️  BATTLE BEGINS! ⚔️                    ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════╝\n" << Colors::RESET;
    
    // Animated enemy entrance
    Colors::typewriter("👹 A wild ", 30);
    std::cout << Colors::BRIGHT_RED << enemy->getName() << Colors::RESET;
    Colors::typewriter(" appears!\n\n", 30);
    Colors::delay(500);
    
    while (player->getHealth() > 0 && enemy->isAlive()) {
        displayBattleStatus();
        
        if (playerTurn) {
            playerAction();
            playerTurn = false;
        } else {
            enemyAction();
            playerTurn = true;
        }
        
        // Check if battle is over
        if (!enemy->isAlive()) {
            Colors::delay(300);
            std::cout << "\n" << Colors::BRIGHT_GREEN;
            std::cout << "╔════════════════════════════════════════════════════════════╗\n";
            std::cout << "║                    🏆 VICTORY! 🏆                          ║\n";
            std::cout << "╚════════════════════════════════════════════════════════════╝\n" << Colors::RESET;
            
            Colors::typewriter("✅ You defeated the ", 25);
            std::cout << Colors::BRIGHT_YELLOW << enemy->getName() << Colors::RESET;
            Colors::typewriter("!\n", 25);
            
            Colors::delay(200);
            std::cout << Colors::BRIGHT_CYAN << "✨ You gained " << Colors::YELLOW << enemy->getExperienceReward() << Colors::BRIGHT_CYAN << " experience!\n";
            Colors::delay(200);
            std::cout << "💰 You found " << Colors::YELLOW << enemy->getGoldReward() << Colors::BRIGHT_CYAN << " gold!\n" << Colors::RESET;
            
            player->gainExperience(enemy->getExperienceReward());
            player->addGold(enemy->getGoldReward());
            
            return true;
        }
        
        if (player->getHealth() <= 0) {
            Colors::delay(300);
            std::cout << "\n" << Colors::BRIGHT_RED;
            std::cout << "╔════════════════════════════════════════════════════════════╗\n";
            std::cout << "║                    💀 DEFEAT 💀                            ║\n";
            std::cout << "╚════════════════════════════════════════════════════════════╝\n" << Colors::RESET;
            Colors::typewriter("❌ You have been defeated...\n", 40);
            return false;
        }
    }
    
    return false;
}

void Battle::playerAction() {
    bool actionTaken = false;
    while (!actionTaken) {
        // Combat Menu
        const int width = 50;
        std::string border;
        for (int i = 0; i < width - 2; ++i) border += "─";
        
        std::cout << "\n" << Colors::BRIGHT_GREEN;
        std::cout << "┌" << border << "┐\n";
        
        std::string title = " YOUR TURN ";
        int pad = (width - 2 - title.length()) / 2;
        std::cout << "│" << std::string(pad, ' ') << Colors::BOLD << title << Colors::RESET << Colors::BRIGHT_GREEN << std::string(width - 2 - pad - title.length(), ' ') << "│\n";
        std::cout << "├" << border << "┤\n";
        
        std::cout << "│  " << Colors::CYAN << "1. " << Colors::Emoji::ATTACK << " " << std::left << std::setw(width - 9) << "Attack" << Colors::BRIGHT_GREEN << "│\n";
        std::cout << "│  " << Colors::CYAN << "2. " << Colors::Emoji::SCROLL << " " << std::left << std::setw(width - 9) << "Skills" << Colors::BRIGHT_GREEN << "│\n";
        std::cout << "│  " << Colors::CYAN << "3. " << Colors::Emoji::DEFEND << " " << std::left << std::setw(width - 9) << "Defend" << Colors::BRIGHT_GREEN << "│\n";
        std::cout << "│  " << Colors::CYAN << "4. " << Colors::Emoji::POTION << " " << std::left << std::setw(width - 9) << "Use Item" << Colors::BRIGHT_GREEN << "│\n";
        
        std::cout << "└" << border << "┘\n" << Colors::RESET;
        std::cout << Colors::BRIGHT_YELLOW << "🎮 Choice: " << Colors::RESET;
        
        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            choice = 0;
        }
        
        switch(choice) {
            case 1: { // Attack
                int damage = player->attack();
                // Attack animation
                Colors::animateAttack(player->getName(), enemy->getName(), damage);
                enemy->takeDamage(damage);
                actionTaken = true;
                break;
            }
            case 2: { // Skills
                const auto& skills = player->getSkills();
                if (skills.empty()) {
                    std::cout << Colors::GRAY << "You have no skills learned!\n" << Colors::RESET;
                    break;
                }
                
                std::cout << "\n" << Colors::BRIGHT_MAGENTA << "✨ SKILLS ✨\n" << Colors::RESET;
                for (size_t i = 0; i < skills.size(); ++i) {
                     std::cout << Colors::CYAN << (i + 1) << ". " << Colors::WHITE << skills[i].name 
                               << Colors::BLUE << " (" << skills[i].manaCost << " MP)" 
                               << Colors::GRAY << " - " << skills[i].description << "\n";
                }
                std::cout << Colors::GRAY << "0. Cancel\n" << Colors::RESET;
                std::cout << Colors::BRIGHT_YELLOW << "Select skill: " << Colors::RESET;
                
                int skillChoice;
                if (!(std::cin >> skillChoice)) {
                     std::cin.clear(); std::cin.ignore(10000, '\n'); 
                     skillChoice = -1;
                }
                
                if (skillChoice == 0) break; // Back to main menu
                
                auto result = player->castSkill(skillChoice - 1);
                if (result.first == -1) {
                    std::cout << Colors::BRIGHT_RED << "❌ Not enough Mana!\n" << Colors::RESET;
                } else if (result.first == -2) {
                     std::cout << Colors::BRIGHT_RED << "❌ Invalid skill selection.\n" << Colors::RESET;
                } else {
                    // Success
                    if (result.second == "heal") {
                         std::cout << Colors::BRIGHT_GREEN << "✨ You cast " << skills[skillChoice-1].name 
                                   << " and healed " << Colors::GREEN << result.first << Colors::BRIGHT_GREEN << " HP!\n" << Colors::RESET;
                    } else {
                         std::cout << Colors::BRIGHT_MAGENTA << "⚡ You cast " << skills[skillChoice-1].name 
                                   << " dealing " << Colors::BRIGHT_RED << result.first << Colors::BRIGHT_MAGENTA << " damage!\n" << Colors::RESET;
                         enemy->takeDamage(result.first);
                    }
                    actionTaken = true;
                }
                break;
            }
            case 3: { // Defend
                std::cout << Colors::BRIGHT_BLUE << Colors::Emoji::DEFEND << " You take a defensive stance!\n" << Colors::RESET;
                // Defense logic placeholder
                actionTaken = true;
                break;
            }
            case 4: { // Item
                player->displayInventory();
                std::cout << Colors::BRIGHT_YELLOW << "Enter item name to use (or 'cancel'): " << Colors::RESET;
                std::string itemName;
                std::cin.ignore(); 
                std::getline(std::cin, itemName);
                if (itemName != "cancel") {
                    player->useItem(itemName);
                    actionTaken = true;
                }
                break;
            }
            default:
                std::cout << Colors::BRIGHT_RED << "❌ Invalid choice. Try again.\n" << Colors::RESET;
        }
    }
}

void Battle::enemyAction() {
    std::cout << "\n" << Colors::BRIGHT_RED << "── 👹 Enemy Turn ──\n" << Colors::RESET;
    Colors::delay(400);
    int damage = enemy->attack();
    Colors::animateAttack(enemy->getName(), player->getName(), damage);
    player->takeDamage(damage);
}

void Battle::displayBattleStatus() const {
    std::cout << "\n" << Colors::BRIGHT_RED;
    std::cout << "╔══════════════════════════════════════════════════╗\n";
    std::cout << "║               ⚔️  BATTLE ARENA  ⚔️                ║\n";
    std::cout << "╠══════════════════════════════════════════════════╣\n" << Colors::RESET;
    
    // Player Stats
    std::cout << Colors::BRIGHT_RED << "║ " << Colors::BRIGHT_GREEN << "🧑‍🎤 " << std::left << std::setw(44) << player->getName() << Colors::BRIGHT_RED << "║\n";
    
    // Build health bar string
    std::string hpBar = Colors::healthBar(player->getHealth(), player->getMaxHealth(), 20);
    std::string hpText = std::to_string(player->getHealth()) + "/" + std::to_string(player->getMaxHealth());
    std::cout << Colors::BRIGHT_RED << "║ " << Colors::RED << "❤️  HP: " << hpBar 
              << Colors::WHITE << " " << std::left << std::setw(8) << hpText << Colors::BRIGHT_RED << "  ║\n";
    
    std::string mpBar = Colors::manaBar(player->getMana(), player->getMaxMana(), 20);
    std::string mpText = std::to_string(player->getMana()) + "/" + std::to_string(player->getMaxMana());
    std::cout << Colors::BRIGHT_RED << "║ " << Colors::BLUE << "💙 MP: " << mpBar 
              << Colors::WHITE << " " << std::left << std::setw(8) << mpText << Colors::BRIGHT_RED << "  ║\n";
              
    std::cout << Colors::BRIGHT_RED << "╠──────────────────────────────────────────────────╣\n";
    std::cout << Colors::BRIGHT_RED << "║                     ⚡ VS ⚡                      ║\n";
    std::cout << Colors::BRIGHT_RED << "╠──────────────────────────────────────────────────╣\n";
    
    // Enemy Stats
    std::cout << Colors::BRIGHT_RED << "║ " << Colors::BRIGHT_MAGENTA << "👹 " << std::left << std::setw(44) << enemy->getName() << Colors::BRIGHT_RED << "║\n";
    
    std::string ehpBar = Colors::healthBar(enemy->getHealth(), enemy->getMaxHealth(), 20);
    std::string ehpText = std::to_string(enemy->getHealth()) + "/" + std::to_string(enemy->getMaxHealth());
    std::cout << Colors::BRIGHT_RED << "║ " << Colors::RED << "❤️  HP: " << ehpBar 
              << Colors::WHITE << " " << std::left << std::setw(8) << ehpText << Colors::BRIGHT_RED << "  ║\n";
              
    std::cout << Colors::BRIGHT_RED << "╚══════════════════════════════════════════════════╝\n" << Colors::RESET;
}

