#include "Shop.h"
#include "Colors.h"
#include <iostream>
#include <algorithm>
#include <iomanip>

Shop::Shop(const std::string& name) : shopName(name) {
    initializeItems();
}

void Shop::initializeItems() {
    items.clear();
    
    // Potions
    items.push_back(Item("Health Potion", "potion", 30, 20));
    items.push_back(Item("Mana Potion", "potion", 25, 15));
    items.push_back(Item("Greater Health Potion", "potion", 60, 40));
    items.push_back(Item("Greater Mana Potion", "potion", 50, 35));
    
    // Weapons (for future expansion)
    items.push_back(Item("Iron Sword", "weapon", 5, 100));
    items.push_back(Item("Steel Sword", "weapon", 8, 200));
    items.push_back(Item("Magic Staff", "weapon", 6, 150));
    items.push_back(Item("Elven Bow", "weapon", 7, 180));
    
    // Armor (for future expansion)
    items.push_back(Item("Leather Armor", "armor", 3, 80));
    items.push_back(Item("Chain Mail", "armor", 5, 150));
    items.push_back(Item("Plate Armor", "armor", 8, 250));
}

void Shop::displayShop(Player* player) const {
    std::cout << "\n" << Colors::BRIGHT_YELLOW;
    std::cout << "╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "║              🛒 " << std::left << std::setw(22) << shopName << " 🛒              ║\n";
    std::cout << "╠════════════════════════════════════════════════════════════╣\n";
    std::cout << "║  " << Colors::WHITE << "💰 Your Gold: " << Colors::BRIGHT_YELLOW << std::setw(6) << player->getGold() << Colors::BRIGHT_YELLOW << "                                      ║\n";
    std::cout << "╠════════════════════════════════════════════════════════════╣\n";
    std::cout << "║  " << Colors::CYAN << std::left << std::setw(24) << "ITEM" 
              << std::setw(10) << "TYPE" 
              << std::setw(8) << "PRICE" 
              << std::setw(12) << "EFFECT" << Colors::BRIGHT_YELLOW << " ║\n";
    std::cout << "╠────────────────────────────────────────────────────────────╣\n" << Colors::RESET;
    
    for (size_t i = 0; i < items.size(); i++) {
        std::cout << Colors::BRIGHT_YELLOW << "║  ";
        
        // Icon based on type
        if (items[i].type == "potion") {
            std::cout << "🧪 ";
        } else if (items[i].type == "weapon") {
            std::cout << "⚔️  ";
        } else if (items[i].type == "armor") {
            std::cout << "🛡️  ";
        } else {
            std::cout << "📦 ";
        }
        
        std::cout << Colors::WHITE << std::left << std::setw(20) << items[i].name;
        
        // Type with color coding
        if (items[i].type == "potion") {
            std::cout << Colors::GREEN << std::setw(10) << "Potion";
        } else if (items[i].type == "weapon") {
            std::cout << Colors::RED << std::setw(10) << "Weapon";
        } else if (items[i].type == "armor") {
            std::cout << Colors::BLUE << std::setw(10) << "Armor";
        } else {
            std::cout << Colors::WHITE << std::setw(10) << items[i].type;
        }
        
        std::cout << Colors::YELLOW << std::setw(8) << items[i].price;
        
        // Effect description
        if (items[i].type == "potion") {
            std::cout << Colors::GREEN << "+" << std::setw(3) << items[i].value << " HP/MP";
        } else {
            std::cout << Colors::CYAN << "+" << std::setw(3) << items[i].value << " stat";
        }
        
        std::cout << Colors::BRIGHT_YELLOW << "  ║\n" << Colors::RESET;
    }
    
    std::cout << Colors::BRIGHT_YELLOW;
    std::cout << "╚════════════════════════════════════════════════════════════╝\n" << Colors::RESET;
}

bool Shop::buyItem(Player* player, const std::string& itemName) {
    auto it = std::find_if(items.begin(), items.end(),
        [&itemName](const Item& item) { return item.name == itemName; });
    
    if (it != items.end()) {
        if (player->spendGold(it->price)) {
            player->addItem(*it);
            std::cout << Colors::BRIGHT_GREEN << "✅ You bought " << it->name << " for " << it->price << " gold!\n" << Colors::RESET;
            return true;
        } else {
            std::cout << Colors::BRIGHT_RED << "❌ You don't have enough gold!\n" << Colors::RESET;
            return false;
        }
    } else {
        std::cout << Colors::BRIGHT_RED << "❌ Item not found in shop.\n" << Colors::RESET;
        return false;
    }
}

void Shop::addItem(const Item& item) {
    items.push_back(item);
}

