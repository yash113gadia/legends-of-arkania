#include "Enemy.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

Enemy::Enemy(const std::string& enemyName, int enemyLevel, const std::string& enemyRegion)
    : name(enemyName), level(enemyLevel), region(enemyRegion) {
    
    // Base stats scale with level
    maxHealth = 50 + (level * 15);
    health = maxHealth;
    strength = 8 + (level * 2);
    defense = 5 + (level * 1);
    agility = 6 + (level * 1);
    experienceReward = 20 + (level * 10);
    goldReward = 10 + (level * 5);
}

int Enemy::attack() const {
    int baseDamage = strength;
    int damage = baseDamage + (rand() % 5);
    return damage;
}

int Enemy::defend() const {
    return defense + (rand() % 3);
}

void Enemy::takeDamage(int damage) {
    int actualDamage = std::max(1, damage - defend());
    health = std::max(0, health - actualDamage);
}

void Enemy::displayStats() const {
    std::cout << "\n=== ENEMY STATS ===\n";
    std::cout << "Name: " << name << "\n";
    std::cout << "Level: " << level << "\n";
    std::cout << "Health: " << health << "/" << maxHealth << "\n";
    std::cout << "Strength: " << strength << "\n";
    std::cout << "Defense: " << defense << "\n";
}

