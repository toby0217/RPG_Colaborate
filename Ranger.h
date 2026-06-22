#ifndef RANGER_H
#define RANGER_H

#include "Player.h"
#include "Skill.h"
#include "Monster.h"
#include <vector>

using std::vector;

namespace RPG_Colaborate {
    class BattleManager;
    class Skill;
    class Monster;
    
    class Ranger : public Player {
    private:
        int criticalRate;
        int criticalEffect;
        int multiShotTurns; // 連續射擊剩餘回合數 (Buff 狀態)
        int spacegoatHp;
    public:
        Ranger();
        Ranger(string theName, int theMaxHp, int theMaxMp, int theAttackPower, int theDefense);

        int getCriticalRate() const;
        int getCriticalEffect() const;
        int getMultiShotTurns() const;

        void setCriticalRate(int newRate);
        void setCriticalEffect(int newEffect);

        // overrides
        void attack(int targetIndex, vector<Monster*>& monsters, vector<Player*>& players) override;
        void takeDamage(int damage, vector<Monster*>& monsters) override;
        bool useSkill(int skillInput, int targetIndex, vector<Player*>& players, vector<Monster*>& monsters) override;
        void triggerClassSpecial(Skill& theSkill, int targetIndex, vector<Monster*>& monsters, vector<Player*>& players) override;
    };
}

#endif