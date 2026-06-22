#ifndef FrostSlime_H
#define FrostSlime_H
#include "BattleManager.h"
#include "Monster.h"

namespace RPG_Colaborate {
    class FrostSlime : public Monster {
    public:
        FrostSlime();
        FrostSlime(const string& name,int hp,int attackPower,int rewardGold); 
        //不用填閃避和護甲和等級的建構子，也許有用，沒用可以直接刪了

        FrostSlime(const string& name,int hp,int attackPower,int rewardGold,int evadeRate,int defense, MonsterRank rank);
        //完整建構子
        void attack(int targetIndex, vector<Player*>& players, vector<Monster*>& monsters)override;
    };
}

#endif