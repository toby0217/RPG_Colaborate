#ifndef TheDeceiver_H
#define TheDeceiver_H
#include "BattleManager.h"
#include "Monster.h"
#include<vector>

namespace RPG_Colaborate {
    class TheDeceiver : public Monster {
    public:
        TheDeceiver();
        TheDeceiver(const string& name,int hp,int attackPower,int rewardGold); 
        //不用填閃避和護甲和等級的建構子，也許有用，沒用可以直接刪了

        TheDeceiver(const string& name,int hp,int attackPower,int rewardGold,int evadeRate,int defense, MonsterRank rank);
        //完整建構子
        void entryAOEAttack(vector<Player*>& players);
    };
}

#endif