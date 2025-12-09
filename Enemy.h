#ifndef ENEMY_H
#define ENEMY_H

#include <string>

class Enemy {
private:
    std::string name;
    int level;
    int health;
    int maxHealth;
    int strength;
    int defense;
    int agility;
    int experienceReward;
    int goldReward;
    std::string region;

public:
    Enemy(const std::string& enemyName, int enemyLevel, const std::string& enemyRegion);
    
    // Getters
    std::string getName() const { return name; }
    int getLevel() const { return level; }
    int getHealth() const { return health; }
    int getMaxHealth() const { return maxHealth; }
    int getStrength() const { return strength; }
    int getDefense() const { return defense; }
    int getExperienceReward() const { return experienceReward; }
    int getGoldReward() const { return goldReward; }
    
    // Combat
    int attack() const;
    int defend() const;
    void takeDamage(int damage);
    bool isAlive() const { return health > 0; }
    
    // Display
    void displayStats() const;
};

#endif

