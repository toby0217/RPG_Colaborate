#include "TheDeceiver.h"
#include "Monster.h"
#include "Player.h"
#include<iostream>
namespace RPG_Colaborate{
    TheDeceiver::TheDeceiver():
    Monster("The Deceiver",100,10,10,0,0,MonsterRank::BOSS){}

    TheDeceiver::TheDeceiver(const string& name,int hp,int attackPower,int rewardGold):
    Monster(name,hp,attackPower,rewardGold,20,0,MonsterRank::BOSS){}
    //不用填閃避、護甲和等級的建構子，也許有用，沒用可以直接刪了

    TheDeceiver::TheDeceiver(const string& name,int hp,int attackPower,int rewardGold,int evadeRate,int defense, MonsterRank rank):
    Monster(name,hp,attackPower,rewardGold,evadeRate,defense,rank){}
    //完整建構子

    void TheDeceiver::entryAOEAttack(vector<Player*>& players){
        for(Player* player:players){
            std::cout<<Monster::getName()<<"entry and trigger a AOE attack."<<std::endl;
            if(player){
                player->takeDamage(Monster::getAttackPower());
            }
        }
    }
}