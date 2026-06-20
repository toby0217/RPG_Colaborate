#include"VampireBat.h"
#include "Monster.h"

namespace RPG_Colaborate{
    VampireBat::VampireBat():
    Monster("VampireBat",100,10,10,0,0){}

    VampireBat::VampireBat(const string& name,int hp,int attackPower,int rewardGold):
    Monster(name,hp,attackPower,rewardGold,20,0){}
    //不用填閃避和護甲的建構子，也許有用，沒用可以直接刪了

    VampireBat::VampireBat(const string& name,int hp,int attackPower,int rewardGold,int evadeRate,int defense):
    Monster(name,hp,attackPower,rewardGold,evadeRate,defense){}
    //完整建構子

    void VampireBat::attack(Player& player){
        Monster::setHp(Monster::getAttackPower() + Monster::getHp());
        Monster::attack(player);
    }
}