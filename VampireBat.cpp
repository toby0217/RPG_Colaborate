#include"VampireBat.h"
#include "Monster.h"
#include"BattleManager.h"

namespace RPG_Colaborate{
    VampireBat::VampireBat():
    Monster("VampireBat",100,10,10,0,0,MonsterRank::NORMAL){}

    VampireBat::VampireBat(const string& name,int hp,int attackPower,int rewardGold):
    Monster(name,hp,attackPower,rewardGold,20,0,MonsterRank::NORMAL){}
    //不用填閃避、護甲和等級的建構子，也許有用，沒用可以直接刪了

    VampireBat::VampireBat(const string& name,int hp,int attackPower,int rewardGold,int evadeRate,int defense,MonsterRank rank):
    Monster(name,hp,attackPower,rewardGold,evadeRate,defense,rank){}
    //完整建構子

    void VampireBat::attack(int targetIndex, vector<Player*>& players, vector<Monster*>& monsters){
        Monster::setHp(Monster::getAttackPower() + Monster::getHp());
        Monster::attack(targetIndex, players, monsters);
    }
}