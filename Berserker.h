#ifndef BERSERKER_H
#define BERSERKER_H

#include "Player.h"
#include "Skill.h"
#include "Monster.h"
#include <vector>

namespace RPG_Colaborate {
    class Berserker : public Player {
    private:
        int criticalRate;
        int criticalEffect;
        
        // 狂戰士專屬機制變數
        double passiveHealRatio;     // 受到傷害後的回復比例 (例如 0.2 代表回復 20%)
        bool isCounterActive;        // 是否處於反擊狀態
        std::vector<Monster*>* activeEnemyLine; // 用於在受傷時指向當前戰場的敵人陣列

        // 技能冷卻系統
        int currentCD[3];
        int maxCD[3];

    public:
        // Constructors
        Berserker();
        Berserker(string theName, int theMaxHp, int theMaxMp, int theAttackPower, int theDefense);

        // Getters
        int getCriticalRate() const;
        int getCriticalEffect() const;
        bool getIsCounterActive() const;

        // Setters
        void setCriticalRate(int newRate);
        void setCriticalEffect(int newEffect);

        // 覆寫受傷邏輯：實作「受傷固定比例回血」被動與「反擊巨獸」主動
        void takeDamage(int damage) override;

        // 核心技能施放：傳入目標索引與敵方戰線
        bool useSkill(int skillNumber, int targetIndex, std::vector<Monster*>& enemyLine);
        
        // 回合結束時推進 CD 減少
        void updateTurnStatus();
    };
}

#endif
