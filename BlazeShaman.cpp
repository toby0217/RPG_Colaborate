#include "BlazeShaman.h"
#include "Player.h" 
#include "BattleManager.h" // 為了使用 EffectType::BURN
#include <iostream>
#include <vector>

using std::cout;
using std::endl;

namespace RPG_Colaborate {
    BlazeShaman::BlazeShaman()
        : Monster("Blaze Shaman", 100, 14, 35, 0, 3, NORMAL) {
    }

    BlazeShaman::BlazeShaman(const string& name,int hp,int attackPower,int rewardGold,int evadeRate,int defense,MonsterRank rank):
    Monster(name,hp,attackPower,rewardGold,evadeRate,defense,rank){}
    //完整建構子

    void BlazeShaman::attack(int targetIndex, vector<Player*>& players, vector<Monster*>& monsters)
    {
        cout << "🔥 " << getName() << " cast 【Blaze Storm】!" << endl;
        for (Player* player : players) {
            if (player->isAlive()) {
                players[targetIndex]->takeDamage(getAttackPower(), monsters);
                player->takeEffect(BURN, 3);
            }
        }
    }
}