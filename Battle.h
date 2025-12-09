#ifndef BATTLE_H
#define BATTLE_H

#include "Player.h"
#include "Enemy.h"

class Battle {
private:
    Player* player;
    Enemy* enemy;
    bool playerTurn;
    
    void playerAction();
    void enemyAction();
    void displayBattleStatus() const;

public:
    Battle(Player* p, Enemy* e);
    
    // Returns true if player wins, false if player loses
    bool start();
};

#endif

