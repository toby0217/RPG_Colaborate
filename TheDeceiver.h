#ifndef TheDeceiver_H
#define TheDeceiver_H
#include "BattleManager.h"
#include "Monster.h"
#include<vector>

namespace RPG_Colaborate {
    class TheDeceiver : public Monster {
    private:
        std::vector<int> mechanicSequence; // 儲存 1, 2, 3 的隨機順序
        int mechanicIndex = 0;             // 當前執行到陣列的第幾個效果
        int turnCounter = 0;               // 回合計數器 (每兩回合觸發一次)
        
        int currentStance = 0;             // 0: 無, 1: 減傷, 2: 蓄力反擊, 3: 回血
        int hitCountInCounterStance = 0;   // 紀錄在效果二期間受到的攻擊次數
    public:
        TheDeceiver();
        TheDeceiver(const string& name,int hp,int attackPower,int rewardGold);
        //不用填閃避和護甲和等級的建構子，也許有用，沒用可以直接刪了

        TheDeceiver(const string& name,int hp,int attackPower,int rewardGold,int evadeRate,int defense, MonsterRank rank);
        //完整建構子
        void entryAction(vector<Player*>& players, vector<Monster*>& monsters)override;
        // 覆寫以實現減傷與受擊計數
        void attack(int targetIdx, vector<Player*>& players, vector<Monster*>& monsters)override;
        void takeDamage(int damage) override;
    };
}

#endif