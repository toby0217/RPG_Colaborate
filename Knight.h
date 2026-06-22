#ifndef KNIGHT_H
#define KNIGHT_H

#include "Player.h"
#include "Skill.h"
#include "Monster.h"
#include <string>
#include <vector>

using std::string;
using std::vector;

namespace RPG_Colaborate {
    class Monster;

    class Knight : public Player
    {
    private:
        int criticalRate;
        int criticalEffect;
    public:
        // Inherit the constructor from Player
        Knight();
        Knight(string theName, int theMaxHp, int theMaxMp, int theAttackPower, int theDefense);
        ~Knight();

        // getters
        int getCriticalRate() const;
        int getCriticalEffect() const;

        // setters
        void setCriticalRate(int newRate);
        void setCriticalEffect(int newEffect);

        //  宣告覆寫的普攻
        void attack(int targetIndex, vector<Monster*>& monsters, vector<Player*>& players) override;
        
        // Override takeDamage for flat damage reduction (Knight's passive)
        void takeDamage(int damage, vector<Monster*>& monsters) override;
        bool useSkill(int skillInput, int targetIndex, vector<Player*>& players, vector<Monster*>& monsters) override;
    };
}

#endif
