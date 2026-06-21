#ifndef PRIEST_H
#define PRIEST_H

#include "Player.h"
#include <vector>

namespace RPG_Colaborate {

    class Priest : public Player {
    private:
        bool canRevive; 

        // 內部共用邏輯：對最低血量隊友回血 15%
        void triggerPassiveHeal(std::vector<Player*>& teammates);

    public:
        Priest();
        Priest(string theName, int theMaxHp, int theMaxMp, int theAttackPower, int theDefense);
        virtual ~Priest() override;

        // 覆寫以觸發被動補血
        void attack(int targetIndex, vector<Monster*>& monsters, vector<Player*>& players) override;
        bool useSkill(int skillInput, int targetIndex, vector<Player*>& players, vector<Monster*>& monsters) override;
        void triggerClassSpecial(Skill& theSkill, int targetIndex, vector<Monster*>& monsters, vector<Player*>& players) override;
    };
}

#endif