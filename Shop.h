#ifndef SHOP_H
#define SHOP_H

#include "Player.h"
#include <vector>

class Shop {
private:
    std::vector<Item> items;
    std::string shopName;

public:
    Shop(const std::string& name);
    
    void initializeItems();
    void displayShop(Player* player) const;
    bool buyItem(Player* player, const std::string& itemName);
    void addItem(const Item& item);
};

#endif

