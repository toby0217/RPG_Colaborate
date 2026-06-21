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
        double passiveHealRatio;

    public:
        Berserker();
        Berserker(string theName, int theMaxHp, int theMaxMp, int theAttackPower, int theDefense);
        ~Berserker();

        int getCriticalRate() const;
        int getCriticalEffect() const;
        void setCriticalRate(int newRate);
        void setCriticalEffect(int newEffect);

        // ✨ 新增普攻覆寫，讓狂戰士吃得到暴擊加成
        void attack(int targetIndex, vector<Monster*> monsters, vector<Player*> players) override;

        // 覆寫受傷：只保留被動回血邏輯
        void takeDamage(int damage) override;

        // 技能施放
        bool useSkill(int skillNumber, int targetIndex, vector<Player*> players, vector<Monster*> monsters) override;

        // 供戰場檢測到「持有反擊狀態且受擊」時，向下呼叫的反擊函式
        void triggerCounterAttack(vector<Monster*> monsters);
    };
}

#endif
