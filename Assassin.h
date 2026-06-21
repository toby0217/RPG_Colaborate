#ifndef ASSASSIN_H
#define ASSASSIN_H

#include "Player.h"
#include "Skill.h"
#include "Monster.h"
#include <vector>
using std::vector;

namespace RPG_Colaborate {
    class Assassin : public Player {
    private:
        int criticalRate;
        int criticalEffect;
        int turnCounter; // Tracks rounds for the passive skill
    public:
        Assassin();
        Assassin(string theName, int theMaxHp, int theMaxMp, int theAttackPower, int theDefense);
        ~Assassin();

        int getCriticalRate() const;
        int getCriticalEffect() const;
        int getTurnCounter() const;

        void setCriticalRate(int newRate);
        void setCriticalEffect(int newEffect);

        void executeTurnActions(vector<Monster*>& monsterList);

        //  新增覆寫普攻，讓刺客也能觸發暴擊與災厄之手 Buff
        void attack(int targetIndex, vector<Monster*> monsters, vector<Player*> players) override;
        bool useSkill(int skillNumber, int targetIndex, vector<Player*> players, vector<Monster*> monsters) override;
    };
}

#endif
