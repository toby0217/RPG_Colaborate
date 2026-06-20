#ifndef RANGER_H
#define RANGER_H

#include "Player.h"
#include "Skill.h"
#include "Monster.h"
#include <vector>

namespace RPG_Colaborate {
    class Ranger : public Player {
    private:
        int criticalRate;
        int criticalEffect;
        int multiShotTurns; // 連續射擊剩餘回合數 (Buff 狀態)

    public:
        Ranger();
        Ranger(string theName, int theMaxHp, int theMaxMp, int theAttackPower, int theDefense);

        int getCriticalRate() const;
        int getCriticalEffect() const;
        int getMultiShotTurns() const;

        void setCriticalRate(int newRate);
        void setCriticalEffect(int newEffect);

        // 處理回合結束的狀態結算 (僅處理 Buff)
        void updateBuffStatus();

        // 覆寫普攻：處理對 Boss 增傷 50% 以及 連續射擊 (三連射與暴擊)
        void attack(int targetIndex, vector<Monster*> monsters, vector<Player*> players) override;

        // 覆寫使用技能：攔截並實作遊俠特有技能邏輯
        bool useSkill(int skillNumber, int targetIndex, vector<Player*> players, vector<Monster*> monsters) override;
    };
}

#endif