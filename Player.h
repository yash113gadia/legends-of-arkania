#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>
#include <map>

enum class PlayerClass {
    WARRIOR,
    MAGE,
    ARCHER
};

class Item {
public:
    std::string name;
    std::string type; // "weapon", "armor", "potion"
    int value;
    int price;
    
    Item(const std::string& n, const std::string& t, int v, int p) 
        : name(n), type(t), value(v), price(p) {}
};

struct Skill {
    std::string name;
    int manaCost;
    int power;      // Base power
    float scaling;  // Multiplier for primary stat
    std::string type; // "physical", "magic", "heal"
    std::string description;
    
    Skill(std::string n, int mc, int p, float s, std::string t, std::string d)
     : name(n), manaCost(mc), power(p), scaling(s), type(t), description(d) {}
};

class Player {
private:
    std::string name;
    PlayerClass playerClass;
    int level;
    int experience;
    int experienceToNext;
    int health;
    int maxHealth;
    int mana;
    int maxMana;
    int strength;
    int defense;
    int agility;
    int gold;
    int x, y; // Position on map
    std::vector<Item> inventory;
    std::vector<Skill> skills;
    std::string currentRegion;
    
    void initializeStats();
    void initializeSkills();

public:
    Player(const std::string& playerName, PlayerClass pClass);
    
    // Getters
    std::string getName() const { return name; }
    PlayerClass getClass() const { return playerClass; }
    int getLevel() const { return level; }
    int getHealth() const { return health; }
    int getMaxHealth() const { return maxHealth; }
    int getMana() const { return mana; }
    int getMaxMana() const { return maxMana; }
    int getStrength() const { return strength; }
    int getDefense() const { return defense; }
    int getAgility() const { return agility; }
    int getGold() const { return gold; }
    int getX() const { return x; }
    int getY() const { return y; }
    int getExperience() const { return experience; }
    int getExperienceToNext() const { return experienceToNext; }
    std::string getCurrentRegion() const { return currentRegion; }
    const std::vector<Skill>& getSkills() const { return skills; }
    
    // Setters
    void setPosition(int newX, int newY) { x = newX; y = newY; }
    void setRegion(const std::string& region) { currentRegion = region; }
    
    // Combat
    int attack() const;
    int defend() const;
    std::pair<int, std::string> castSkill(int index);
    void takeDamage(int damage);
    void heal(int amount);
    void restoreMana(int amount);
    
    // Leveling
    void gainExperience(int exp);
    void levelUp();
    
    // Inventory
    void addItem(const Item& item);
    bool removeItem(const std::string& itemName);
    void useItem(const std::string& itemName);
    void displayInventory() const;
    std::vector<Item> getInventory() const { return inventory; }
    
    // Economy
    void addGold(int amount) { gold += amount; }
    bool spendGold(int amount);
    
    // Save/Load
    void saveToFile(const std::string& filename) const;
    bool loadFromFile(const std::string& filename);
    
    // Display
    void displayStats() const;
    std::string getClassName() const;
};

#endif

