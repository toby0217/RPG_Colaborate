#include "BlazeShaman.h"
#include "Player.h" 
#include "BattleManager.h" // 為了使用 EffectType::BURN
#include <iostream>

namespace RPG_Colaborate {
    BlazeShaman::BlazeShaman()
        : Monster("Blaze Shaman", 100, 14, 35, 0, 3, NORMAL) {
    }

    BlazeShaman::BlazeShaman(const string& name,int hp,int attackPower,int rewardGold,int evadeRate,int defense,MonsterRank rank):
    Monster(name,hp,attackPower,rewardGold,evadeRate,defense,rank){}
    //完整建構子

    void BlazeShaman::aoeBurnSkill(std::vector<Player*>& players) {
        std::cout << "🔥 " << getName() << " cast 【Blaze Storm】!" << std::endl;
        
        for (Player* player : players) {
            if (player && player->getHp() > 0) {
                // 💥 這裡明天可以對接逸明的玩家受傷與狀態邏輯：
                // player->takeDamage(15); 
                // player->addEffect(EffectType::BURN, 3); 
                std::cout << "💥 " << player->getName() << " is burning! [BURN]" << std::endl;
            }
        }
    }
}