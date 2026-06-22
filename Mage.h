#ifndef MAGE_H
#define MAGE_H

#include "Player.h"
#include "Skill.h"
#include "Monster.h"
#include <vector>

namespace RPG_Colaborate {
    class Mage : public Player {
    private:
        // 輔助函式：檢測場上敵人是否同時擁有燃燒與冰霜狀態
        //void checkElementalDetonation(vector<Monster*>& monsters);

    public:
        Mage();
        Mage(string theName, int theMaxHp, int theMaxMp, int theAttackPower, int theDefense);
        ~Mage();

        // 覆寫普攻：實作身旁 50% 擴散傷害
        void attack(int targetIndex, vector<Monster*>& monsters, vector<Player*>& players) override;

        // 覆寫技能：處理 25% 暈眩與技能後的元素引爆
        bool useSkill(int skillInput, int targetIndex, vector<Player*>& players, vector<Monster*>& monsters) override;
        void triggerClassSpecial(Skill& theSkill, int targetIndex, vector<Monster*>& monsters, vector<Player*>& players) override;
    };
}

#endif