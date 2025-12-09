#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "Map.h"
#include "Battle.h"
#include "Shop.h"
#include "Enemy.h"
#include <map>
#include <string>

class Game {
private:
    Player* player;
    std::map<std::string, Map*> regions;
    std::string currentRegion;
    Shop* shop;
    bool gameRunning;
    
    void initializeRegions();
    void handleMovement(char direction);
    void handleRandomEncounter();
    Enemy* generateRandomEnemy();
    void handleTownInteraction();
    void handleDungeon();
    void handleCastle();
    void displayMainMenu();
    void createNewGame();
    bool loadGame();
    void saveGame();
    void displayHelp();

public:
    Game();
    ~Game();
    
    void run();
};

#endif

