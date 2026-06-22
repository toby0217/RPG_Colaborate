#include "TheDeceiver.h"
#include "Monster.h"
#include "Player.h"
#include <iostream>
#include <vector>
#include <ctime>
#include <algorithm> // for std::shuffle
#include <random>    // 🎯 現代 C++ 必備的隨機標頭檔


using std::vector;
using std::cout;
using std::endl;

namespace RPG_Colaborate{
    TheDeceiver::TheDeceiver():
    Monster("The Deceiver",100,10,10,0,0,MonsterRank::BOSS){}

    TheDeceiver::TheDeceiver(const string& name,int hp,int attackPower,int rewardGold):
    Monster(name,hp,attackPower,rewardGold,20,0,BOSS){}
    //不用填閃避、護甲和等級的建構子，也許有用，沒用可以直接刪了

    TheDeceiver::TheDeceiver(const string& name,int hp,int attackPower,int rewardGold,int evadeRate,int defense, MonsterRank rank):
    Monster(name,hp,attackPower,rewardGold,evadeRate,defense,rank){}
    //完整建構子

    void TheDeceiver::entryAction(vector<Player*>& players, vector<Monster*>& monsters){
        cout << "\n🎭 The Deceiver: \"This is no place for the likes of you.\"\n";
        cout<<name<<"entry and trigger a AOE attack!"<<endl;
        for(Player* player:players){
            player->takeDamage(0.5 * getAttackPower(), monsters);
        }

        mechanicSequence = {1, 2, 3};
        // 🎯 替代 random_shuffle 的現代標準寫法
        std::random_device rd;  // 取得硬體隨機數種子
        std::mt19937 g(rd());   // 以種子初始化 Mersenne Twister 偽隨機數生成器
        std::shuffle(mechanicSequence.begin(), mechanicSequence.end(), g); // 進行洗牌
        
        mechanicIndex = 0;
        turnCounter = 0;
        currentStance = 0;
        hitCountInCounterStance = 0;
    }

    void TheDeceiver::attack(int targetIdx, vector<Player*>& players, vector<Monster*>& monsters) {
        turnCounter++;

        // 1. 結算機制
        if (currentStance == 2) {
            cout << "\n🎭 The Deceiver unleashes a devastating Counter-Attack based on hits taken (" << hitCountInCounterStance << " hits)!\n";
            
            // 傷害計算
            int counterDmg = getAttackPower() * (hitCountInCounterStance * 0.15);
            
            for (auto* p : players) {
                if (p->isAlive()) {
                    p->takeDamage(counterDmg, monsters);
                }
            }
            
            hitCountInCounterStance = 0;
            currentStance = 0; // 解除反擊狀態
            return; // 巨額反擊取代了普通攻擊，直接結束回合
        }

        if (currentStance == 3) {
            hp = std::min(getMaxHp(), hp + static_cast<int>(getMaxHp() * 0.2));
            cout << "💚 [Hidden Effect] The Deceiver restores 20% HP at the end of the round!\n";
        }

        // 2. 每兩回合觸發一次「隱藏機制」
        if (turnCounter % 2 == 0) {
            cout << "\n🎭 The Deceiver: \"Interesting... go ahead and guess.\"\n";
            cout << "🌀 An enigmatic aura surrounds The Deceiver... an unknown effect takes hold!\n";
            
            // 按照打亂好的順序依序賦予狀態
            currentStance = mechanicSequence[mechanicIndex];
            mechanicIndex = (mechanicIndex + 1) % 3; // 循環使用
            
            // 如果這次抽到的是反擊，重置受擊次數
            if (currentStance == 2) {
                hitCountInCounterStance = 0; 
            }
            
            return; // 當發動這個機制時，不進行普通攻擊
        }

        // 3. 一般回合的普通攻擊 (如果沒觸發隱藏機制，也沒在反擊)
        currentStance = 0;
        Monster::attack(targetIdx, players, monsters);
    }

    void TheDeceiver::takeDamage(int rawDamage) {
        // 【效果一：減傷 80%】
        if (currentStance == 1) {
            cout << "🛡️ [Hidden Effect] The Deceiver's illusion reduces the damage by 80%!\n";
            rawDamage = static_cast<int>(rawDamage * 0.2); // 只承受 20% 傷害
        }

        // 【效果二：紀錄受擊次數】
        if (currentStance == 2) {
            hitCountInCounterStance++;
            cout << "⚠️ [Hidden Effect] The Deceiver absorbs the kinetic energy... (Hits tracked: " << hitCountInCounterStance << ")\n";
        }

        // 執行完特殊判定後，呼叫父類別原本的扣血邏輯
        Monster::takeDamage(rawDamage); 
    }
}