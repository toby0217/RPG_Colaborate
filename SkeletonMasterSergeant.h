#ifndef SkeletonMasterSergeant_H
#define SkeletonMasterSergeant_H
#include "Monster.h"
#include <vector>
namespace RPG_Colaborate {
    class SkeletonMasterSergeant : public Monster {
    public:
        SkeletonMasterSergeant();
        SkeletonMasterSergeant(const string& name,int hp,int attackPower,int rewardGold); 
        //不用填閃避和護甲和等級的建構子，也許有用，沒用可以直接刪了

        SkeletonMasterSergeant(const string& name,int hp,int attackPower,int rewardGold,int evadeRate,int defense, MonsterRank rank);
        //完整建構子
        void entryAction(vector<Player*>& players, vector<Monster*>& monsters)override;
        void summomSkeleton(std::vector<Monster*>& monsters);
        void attack(int targetIndex, vector<Player*>& players, vector<Monster*>& monsters)override;
    };
}

#endif