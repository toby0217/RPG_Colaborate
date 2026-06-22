#ifndef BOUNTYHUNTER_H
#define BOUNTYHUNTER_H

#include "Player.h"
#include "Skill.h"
#include "Monster.h"
#include <vector>
#include <string>

using std::vector;
using std::string;

namespace RPG_Colaborate {
    // 前置宣告，避免循環 include
    class Skill;
    class Monster;

    class BountyHunter : public Player {
    private:
        int bountyAtkBonus;
        int redBulletCount;       // 儲存的紅色子彈數量
        int blueBulletCount;      // 儲存的藍色子彈數量
        int consumedMpForGold;    // 紀錄施放三技時所消耗的魔力（用來計算金色子彈的追加傷害）
    public:
        // 建構子與解構子
        BountyHunter();
        BountyHunter(string theName, int theMaxHp, int theMaxMp, int theAttackPower, int theDefense);
        virtual ~BountyHunter() = default;

        void addBountyGold(int gold) override;
        int getAttackPower() const override;

        // 覆寫父類別 Player 的虛擬函式
        void attack(int targetIndex, vector<Monster*>& monsters, vector<Player*>& players) override;
        void takeDamage(int damage, vector<Monster*>& monsters) override;
        bool useSkill(int skillInput, int targetIndex, vector<Player*>& players, vector<Monster*>& monsters) override;
        void triggerClassSpecial(Skill& theSkill, int targetIndex, vector<Monster*>& monsters, vector<Player*>& players) override;

        // 子彈相關的 Getter / Setter
        int getRedBulletCount() const { return redBulletCount; }
        void setRedBulletCount(int count) { redBulletCount = count; }

        int getBlueBulletCount() const { return blueBulletCount; }
        void setBlueBulletCount(int count) { blueBulletCount = count; }

        int getConsumedMpForGold() const { return consumedMpForGold; }
        void setConsumedMpForGold(int mpAmount) { consumedMpForGold = mpAmount; }
    };
}

#endif // BOUNTYHUNTER_H