#include "Player.h"
#include "Colors.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <string>
#include <iomanip>

Player::Player(const std::string& playerName, PlayerClass pClass) 
    : name(playerName), playerClass(pClass), level(1), experience(0), 
      experienceToNext(100), gold(50), x(0), y(0), currentRegion("Verdant Woods") {
    initializeStats();
    initializeSkills();
}

void Player::initializeSkills() {
    skills.clear();
    switch(playerClass) {
        case PlayerClass::WARRIOR:
            skills.emplace_back("Power Strike", 10, 10, 1.5f, "physical", "A heavy blow dealing extra damage.");
            skills.emplace_back("Execute", 20, 25, 2.0f, "physical", "A devastating finishing move.");
            break;
        case PlayerClass::MAGE:
            skills.emplace_back("Fireball", 15, 20, 1.5f, "magic", "Launches a ball of fire.");
            skills.emplace_back("Ice Shard", 10, 15, 1.2f, "magic", "Pierces enemy with ice.");
            skills.emplace_back("Heal", 25, 30, 0.5f, "heal", "Restores health points.");
            break;
        case PlayerClass::ARCHER:
            skills.emplace_back("Precise Shot", 12, 15, 1.5f, "physical", "A carefully aimed shot.");
            skills.emplace_back("Double Tap", 18, 10, 1.8f, "physical", "Two quick shots in succession.");
            break;
    }
}

std::pair<int, std::string> Player::castSkill(int index) {
    if (index < 0 || index >= static_cast<int>(skills.size())) {
        return {-2, "error"};
    }
    
    const Skill& skill = skills[index];
    
    if (mana < skill.manaCost) {
        return {-1, "error"};
    }
    
    mana -= skill.manaCost;
    
    // Calculate effectiveness
    // Using strength as the primary scaler for now for all classes
    // In a deeper system, Mage would use Int/Magic
    int value = skill.power + (static_cast<float>(strength) * skill.scaling);
    
    if (skill.type == "heal") {
        heal(value);
    }
    
    return {value, skill.type};
}

void Player::initializeStats() {
    switch(playerClass) {
        case PlayerClass::WARRIOR:
            maxHealth = 120;
            health = maxHealth;
            maxMana = 30;
            mana = maxMana;
            strength = 15;
            defense = 12;
            agility = 8;
            break;
        case PlayerClass::MAGE:
            maxHealth = 80;
            health = maxHealth;
            maxMana = 100;
            mana = maxMana;
            strength = 8;
            defense = 6;
            agility = 10;
            break;
        case PlayerClass::ARCHER:
            maxHealth = 100;
            health = maxHealth;
            maxMana = 50;
            mana = maxMana;
            strength = 12;
            defense = 8;
            agility = 15;
            break;
    }
}

int Player::attack() const {
    int baseDamage = strength;
    // Add some randomness
    int damage = baseDamage + (rand() % 5);
    return damage;
}

int Player::defend() const {
    return defense + (rand() % 3);
}

void Player::takeDamage(int damage) {
    int actualDamage = std::max(1, damage - defend());
    health = std::max(0, health - actualDamage);
}

void Player::heal(int amount) {
    health = std::min(maxHealth, health + amount);
}

void Player::restoreMana(int amount) {
    mana = std::min(maxMana, mana + amount);
}

void Player::gainExperience(int exp) {
    experience += exp;
    while (experience >= experienceToNext) {
        experience -= experienceToNext;
        levelUp();
    }
}

void Player::levelUp() {
    level++;
    experienceToNext = level * 100;
    
    // Increase stats based on class
    switch(playerClass) {
        case PlayerClass::WARRIOR:
            maxHealth += 20;
            strength += 3;
            defense += 2;
            agility += 1;
            break;
        case PlayerClass::MAGE:
            maxHealth += 10;
            maxMana += 15;
            strength += 1;
            defense += 1;
            agility += 2;
            break;
        case PlayerClass::ARCHER:
            maxHealth += 15;
            maxMana += 8;
            strength += 2;
            defense += 1;
            agility += 3;
            break;
    }
    
    // Restore health and mana on level up
    health = maxHealth;
    mana = maxMana;
    
    std::cout << "\n" << Colors::BRIGHT_YELLOW << "🎆 *** LEVEL UP! *** 🎆\n";
    std::cout << "You are now level " << level << "!\n" << Colors::RESET;
    displayStats();
}

void Player::addItem(const Item& item) {
    inventory.push_back(item);
}

bool Player::removeItem(const std::string& itemName) {
    auto it = std::find_if(inventory.begin(), inventory.end(),
        [&itemName](const Item& item) { return item.name == itemName; });
    
    if (it != inventory.end()) {
        inventory.erase(it);
        return true;
    }
    return false;
}

void Player::useItem(const std::string& itemName) {
    auto it = std::find_if(inventory.begin(), inventory.end(),
        [&itemName](const Item& item) { return item.name == itemName; });
    
    if (it != inventory.end()) {
        if (it->type == "potion") {
            if (it->name.find("Health") != std::string::npos) {
                heal(it->value);
                std::cout << "You restored " << it->value << " health!\n";
            } else if (it->name.find("Mana") != std::string::npos) {
                restoreMana(it->value);
                std::cout << "You restored " << it->value << " mana!\n";
            }
            removeItem(itemName);
        } else {
            std::cout << "You can't use that item here.\n";
        }
    } else {
        std::cout << "Item not found in inventory.\n";
    }
}

void Player::displayInventory() const {
    std::cout << "\n" << Colors::BRIGHT_CYAN;
    std::cout << "╔══════════════════════════════════════════════════╗\n";
    std::cout << "║           🎒  INVENTORY  🎒                      ║\n";
    std::cout << "╠══════════════════════════════════════════════════╣\n" << Colors::RESET;
    
    if (inventory.empty()) {
        std::cout << Colors::BRIGHT_CYAN << "║  " << Colors::GRAY << "📭 (empty - no items)" 
                  << std::string(25, ' ') << Colors::BRIGHT_CYAN << " ║\n";
    } else {
        for (const auto& item : inventory) {
            std::cout << Colors::BRIGHT_CYAN << "║  ";
            
            // Icon based on item type
            if (item.type == "potion") {
                std::cout << "🧪 ";
            } else if (item.type == "weapon") {
                std::cout << "⚔️  ";
            } else if (item.type == "armor") {
                std::cout << "🛡️  ";
            } else {
                std::cout << "📦 ";
            }
            
            std::cout << Colors::WHITE << std::left << std::setw(22) << item.name;
            
            if (item.type == "potion") {
                std::string effect = "(+" + std::to_string(item.value) + " HP/MP)";
                std::cout << Colors::GREEN << std::setw(15) << effect;
            } else if (item.type == "weapon") {
                std::string effect = "(+" + std::to_string(item.value) + " ATK)";
                std::cout << Colors::RED << std::setw(15) << effect;
            } else if (item.type == "armor") {
                std::string effect = "(+" + std::to_string(item.value) + " DEF)";
                std::cout << Colors::BLUE << std::setw(15) << effect;
            } else {
                std::cout << std::setw(15) << item.type;
            }
            
            std::cout << Colors::BRIGHT_CYAN << " ║\n" << Colors::RESET;
        }
    }
    
    std::cout << Colors::BRIGHT_CYAN;
    std::cout << "╚══════════════════════════════════════════════════╝\n" << Colors::RESET;
}

bool Player::spendGold(int amount) {
    if (gold >= amount) {
        gold -= amount;
        return true;
    }
    return false;
}

void Player::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not save game to " << filename << "\n";
        return;
    }
    
    file << name << "\n";
    file << static_cast<int>(playerClass) << "\n";
    file << level << " " << experience << " " << experienceToNext << "\n";
    file << health << " " << maxHealth << "\n";
    file << mana << " " << maxMana << "\n";
    file << strength << " " << defense << " " << agility << "\n";
    file << gold << "\n";
    file << x << " " << y << "\n";
    file << currentRegion << "\n";
    file << inventory.size() << "\n";
    for (const auto& item : inventory) {
        file << item.name << "\n";  // Item name on its own line (may contain spaces)
        file << item.type << " " << item.value << " " << item.price << "\n";
    }
    
    file.close();
}

bool Player::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    // Read name (may contain spaces)
    std::getline(file, name);
    
    // Read class
    int classInt;
    file >> classInt;
    playerClass = static_cast<PlayerClass>(classInt);
    initializeSkills(); // Re-initialize skills based on loaded class
    file.ignore(); // Skip newline
    
    // Read stats
    file >> level >> experience >> experienceToNext;
    file >> health >> maxHealth;
    file >> mana >> maxMana;
    file >> strength >> defense >> agility;
    file >> gold;
    file >> x >> y;
    file.ignore(); // Skip newline before region name
    
    // Read region (may contain spaces)
    std::getline(file, currentRegion);
    
    // Read inventory
    inventory.clear();
    int invSize;
    file >> invSize;
    file.ignore(); // Skip newline
    for (int i = 0; i < invSize; i++) {
        std::string itemName, itemType;
        int itemValue, itemPrice;
        // Read item name (may contain spaces)
        std::getline(file, itemName);
        // Read rest of item data
        file >> itemType >> itemValue >> itemPrice;
        file.ignore(); // Skip newline
        inventory.push_back(Item(itemName, itemType, itemValue, itemPrice));
    }
    
    file.close();
    return true;
}

void Player::displayStats() const {
    std::cout << "\n" << Colors::BRIGHT_CYAN;
    std::cout << "╔══════════════════════════════════════════════════╗\n";
    std::cout << "║          📜 CHARACTER SHEET 📜                   ║\n";
    std::cout << "╠══════════════════════════════════════════════════╣\n" << Colors::RESET;
    
    // Name and Class with class icons
    std::cout << Colors::BRIGHT_CYAN << "║ " << Colors::BRIGHT_GREEN << "👤 Name : " << Colors::WHITE 
              << std::left << std::setw(37) << name << Colors::BRIGHT_CYAN << " ║\n";
    
    std::string className = getClassName();
    std::string classIcon;
    if (className == "Warrior") classIcon = "⚔️  Warrior";
    else if (className == "Mage") classIcon = "🔮 Mage";
    else classIcon = "🏹 Archer";
    
    std::cout << Colors::BRIGHT_CYAN << "║ " << Colors::BRIGHT_GREEN << "   Class: " << Colors::WHITE 
              << std::left << std::setw(37) << classIcon << Colors::BRIGHT_CYAN << " ║\n";
    std::cout << Colors::BRIGHT_CYAN << "║ " << Colors::BRIGHT_GREEN << "⭐ Level: " << Colors::BRIGHT_YELLOW 
              << std::left << std::setw(37) << level << Colors::BRIGHT_CYAN << " ║\n";
    
    std::cout << Colors::BRIGHT_CYAN << "╠══════════════════════════════════════════════════╣\n" << Colors::RESET;
    
    // Health Bar
    std::string hpBar = Colors::healthBar(health, maxHealth, 25);
    std::string hpText = std::to_string(health) + "/" + std::to_string(maxHealth);
    std::cout << Colors::BRIGHT_CYAN << "║ " << Colors::BRIGHT_RED << "❤️  HP: " << hpBar 
              << Colors::WHITE << " " << std::left << std::setw(7) << hpText << Colors::BRIGHT_CYAN << " ║\n";
              
    // Mana Bar
    std::string mpBar = Colors::manaBar(mana, maxMana, 25);
    std::string mpText = std::to_string(mana) + "/" + std::to_string(maxMana);
    std::cout << Colors::BRIGHT_CYAN << "║ " << Colors::BRIGHT_BLUE << "💙 MP: " << mpBar 
              << Colors::WHITE << " " << std::left << std::setw(7) << mpText << Colors::BRIGHT_CYAN << " ║\n";
    
    // XP
    std::string xpText = std::to_string(experience) + " / " + std::to_string(experienceToNext);
    std::cout << Colors::BRIGHT_CYAN << "║ " << Colors::BRIGHT_CYAN << "✨ XP: " << Colors::YELLOW 
              << std::left << std::setw(40) << xpText << Colors::BRIGHT_CYAN << " ║\n";
              
    std::cout << Colors::BRIGHT_CYAN << "╠══════════════════════════════════════════════════╣\n" << Colors::RESET;
    
    // Attributes
    std::cout << Colors::BRIGHT_CYAN << "║ " << Colors::BRIGHT_YELLOW << "⚡ ATTRIBUTES:                                    " << Colors::BRIGHT_CYAN << "║\n";
    std::cout << Colors::BRIGHT_CYAN << "║   " << Colors::RED << "💪 STR: " << Colors::WHITE << std::left << std::setw(4) << strength 
              << Colors::BLUE << "🛡️  DEF: " << Colors::WHITE << std::left << std::setw(4) << defense 
              << Colors::GREEN << "🏃 AGI: " << Colors::WHITE << std::left << std::setw(4) << agility 
              << "           " << Colors::BRIGHT_CYAN << "║\n";
              
    std::cout << Colors::BRIGHT_CYAN << "╠══════════════════════════════════════════════════╣\n" << Colors::RESET;
    
    // Gold and Location
    std::cout << Colors::BRIGHT_CYAN << "║ " << Colors::YELLOW << "💰 Gold: " << Colors::WHITE << std::left << std::setw(8) << gold 
              << Colors::CYAN << "📍 Location: " << Colors::WHITE << std::left << std::setw(17) << currentRegion.substr(0, 17) 
              << Colors::BRIGHT_CYAN << " ║\n";
              
    std::cout << Colors::BRIGHT_CYAN << "╚══════════════════════════════════════════════════╝\n" << Colors::RESET;
}

std::string Player::getClassName() const {
    switch(playerClass) {
        case PlayerClass::WARRIOR: return "Warrior";
        case PlayerClass::MAGE: return "Mage";
        case PlayerClass::ARCHER: return "Archer";
        default: return "Unknown";
    }
}

