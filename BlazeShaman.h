#ifndef BLAZESHAMAN_H
#define BLAZESHAMAN_H

#include "Monster.h"
#include <vector>
using std::vector;

namespace RPG_Colaborate {
    class Player;

    class BlazeShaman : public Monster {
    public:
        BlazeShaman();
        BlazeShaman(const string& name,int hp,int attackPower,int rewardGold,int evadeRate,int defense,MonsterRank rank);
        //完整建構子
        
        // 專屬技能：對全體玩家造成傷害並附加 BURN 狀態
        void attack(int targetIndex, vector<Player*>& players, vector<Monster*>& monsters)override;
    };
}

#endif // BLAZESHAMAN_H