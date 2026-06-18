#ifndef ASSASSIN_H
#define ASSASSIN_H

#include "Player.h"
#include "Monster.h"
#include <vector>

namespace RPG_Colaborate {
    class Assassin : public Player {
    private:
        int turnCounter; // Tracks rounds for the passive skill

    public:
        Assassin(std::string n, int h, int m, int atk, int def) 
            : Player(n, "Assassin", h, m, atk, def), turnCounter(0) {}

        // Call this at the end of every turn
        void executeTurnActions(std::vector<Monster*>& monsterList);
    };
}

#endif