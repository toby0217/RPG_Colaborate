#include "FrostSlime.h"
#include "Monster.h"
#include"Player.h"

#include <iostream>
#include <vector>
using std::vector;
using std::cout;
using std::endl;

namespace RPG_Colaborate{
    FrostSlime::FrostSlime():
    Monster("Frost Slime",100,10,10,0,0,MonsterRank::NORMAL){}

    FrostSlime::FrostSlime(const string& name,int hp,int attackPower,int rewardGold):
    Monster(name,hp,attackPower,rewardGold,20,0,MonsterRank::NORMAL){}
    //不用填閃避、護甲和等級的建構子，也許有用，沒用可以直接刪了

    FrostSlime::FrostSlime(const string& name,int hp,int attackPower,int rewardGold,int evadeRate,int defense, MonsterRank rank):
    Monster(name,hp,attackPower,rewardGold,evadeRate,defense,rank){}
    //完整建構子

    void FrostSlime::attack(int targetIndex, vector<Player*>& players, vector<Monster*>& monsters){
        cout << "❄️ Frost Slime launches an Ice Crystal attack!\n";
        players[targetIndex]->takeDamage(getAttackPower(), monsters);
        if (rand() % 2 == 1) {
            players[targetIndex]->takeEffect(FREEZE,1);
        }
    }
}