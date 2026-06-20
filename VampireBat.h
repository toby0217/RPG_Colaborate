#ifndef VampireBat_H
#define VampireBat_H

#include "Monster.h"

namespace RPG_Colaborate {
    class VampireBat : public Monster {
    public:
        VampireBat();
        VampireBat(const string& name,int hp,int attackPower,int rewardGold); 
        //不用填閃避和護甲的建構子，也許有用，沒用可以直接刪了

        VampireBat(const string& name,int hp,int attackPower,int rewardGold,int evadeRate,int defense);
        //完整建構子
        void attack(Player& player)override;
    };
}

#endif