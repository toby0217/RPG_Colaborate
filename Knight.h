#ifndef KNIGHT_H
#define KNIGHT_H

#include "Player.h"

namespace RPG_Colaborate {
    class Knight : public Player {
    public:
        // Inherit the constructor from Player
        Knight(std::string n, int h, int m, int atk, int def) 
            : Player(n, "Knight", h, m, atk, def) {}

        // Override takeDamage for flat damage reduction (Knight's passive)
        void takeDamage(int incomingDamage) override;
    };
}

#endif