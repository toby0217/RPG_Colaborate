#include "BattleManager.h"
#include "Monster.h"
#include "Player.h"
#include <string>
#include <cstdlib>
#include <iostream>
#include <cmath>
using std::cout;
using std::endl;

namespace RPG_Colaborate
{
    //預設建構子，內容隨便填的，建議不用
    Monster::Monster(): 
    name("monster"), maxHp(100), hp(100), attackPower(10), rewardGold(10), evadeRate(0), defense(0), rank(NORMAL) {}

    //建構子
    Monster::Monster(const string& theName,int theMaxHp,int theAttackPower,int theRewardGold,int theEvadeRate,int theDefense, MonsterRank theRank):
    name(theName), maxHp(theMaxHp), hp(theMaxHp), attackPower(theAttackPower), rewardGold(theRewardGold),
    evadeRate(theEvadeRate), defense(theDefense), rank(theRank) {}

    //getters
    string Monster::getName() const { return name; }
    int Monster::getHp() const { return hp; }
    int Monster::getMaxHp() const { return maxHp; }
    int Monster::getAttackPower() const
    {
        if (getEffectTurns(WEAKNESS) > 0) {
            static_cast<int>(attackPower * 0.8);
        }
        return attackPower;
    }
    int Monster::getRewardGold() const { return rewardGold; }
    int Monster::getEvadeRate() const { return evadeRate; }
    int Monster::getDefense() const { return defense; }
    MonsterRank Monster::getRank() const { return rank; }

    //setters
    void Monster::setName(const string& newName) { name = newName; }
    void Monster::setHp(int newHp) { hp = newHp; }
    void Monster::setMaxHp(int newMaxHp) { 
        if (newMaxHp >= 0) {
            maxHp = newMaxHp;
        }
    }
    void Monster::setRewardGold(int newRewardGold) {
        if (newRewardGold >= 0) {
            rewardGold = newRewardGold;
        }
    }
    void Monster::setAttackPower(int newAttackPower) {
        if (newAttackPower >= 0) {
            attackPower = newAttackPower;
        }
    }
    void Monster::setEvadeRate(int newEvadeRate) { evadeRate = newEvadeRate; }
    void Monster::setDefense(int newDefense) { defense = newDefense; }
    // 1. 設定怪物等級
    void Monster::setRank(const MonsterRank& theRank) {
        rank = theRank;
    }
    //function
    void Monster::attack(int targetIndex, vector<Player*>& players, vector<Monster*>& monsters)
    {
        //已調整:直接依照攻擊力造成傷害
        cout<<name<<" launch an attack!"<<endl;
        //這裡會輸出訊息

        players[targetIndex]->takeDamage(getAttackPower(), monsters);
    }

    int Monster::calculateFinalDamage(int rawDamage)
    {
        if(defense>0){
            int finalDefense = defense;
            if (getEffectTurns(LOWERDEFENSE) > 0) {
                finalDefense = round(0.7 * defense);
            }
            rawDamage = round(rawDamage * (1.0 - ( 1.0 * finalDefense / (finalDefense + 1000) )));
        }

        return rawDamage;
    }

    //這裡我對應player.cpp寫的，保持邏輯一致
    //已調整
    void Monster::takeDamage(int damage){
        if(evadeRate > 0){
            if(rand()%100 < evadeRate){
                cout<<name<<" evade attack!"<<endl;
                return;
                //結束function
            }
        }
        damage = calculateFinalDamage(damage);
        hp -= damage;
        if (hp < 0) {
            hp = 0; // Prevent HP from dropping below zero
        }
        cout << name << " takes " << damage << " points of damage! " 
                << "(Current HP: " << hp << "/" << maxHp << ")" << endl;
                
        if (!isAlive()) {
            cout << name << " has been defeated..." << endl;
        }
    }

    bool Monster::isAlive()const{
        return hp>0;
    }

    void Monster::showInfo()const{
        cout<<"Monster name:"<<name<<endl;
        cout<<"Monster hp:"<<hp<<endl;
        cout<<"Monster attack:"<<attackPower<<endl;
        cout<<"Monster reward gold:"<<rewardGold<<endl;
    }

        // 2. 承受狀態異常（例如：被施加 BURN 3 回合）
    void Monster::takeEffect(const EffectType& effectType, int effectTurns) {
        // 假設你在 Monster.h 裡的 map 變數叫做 effects (即 map<EffectType, int> effects;)
        // 這裡直接將該狀態的回合數更新或寫入
        StatusEffectList[effectType] = effectTurns; 
    }

    // 3. 取得某個狀態剩餘的回合數
    int Monster::getEffectTurns(const EffectType& effectType) const
    {
        // 使用 find 尋找關鍵字
        auto it = StatusEffectList.find(effectType);
        
        // 如果找到了，回傳它紀錄的剩餘回合數
        if (it != StatusEffectList.end()) {
            return it->second;
        }
        
        // 找不到代表沒有這個 Buff/Debuff，回傳 0
        return 0; 
    }

    // 4. 取得所有 Buff（正面狀態）的字串
    string Monster::getBuffs() {
        // 從對話截圖看，逸明明天會用 enum 的數字來切分正負面狀態
        // 為了讓今天的編譯能順利通過，我們先回傳乾淨的預設值交差
        return ""; 
    }

    // 5. 取得所有 Debuff（負面狀態）的字串
    string Monster::getDebuffs() {
        // 同上，先回傳預設值，明天逸明的戰場邏輯寫好後他自己會來豐富這段
        return ""; 
    }

    void Monster::updateStatusEffects() 
    {
        // 使用迭代器安全遍歷怪物的狀態 Map
        for (auto it = StatusEffectList.begin(); it != StatusEffectList.end(); ) {
            // 防呆：如果回合數本來就是 0 或是負數，直接剔除
            if (it->second <= 0) {
                it = StatusEffectList.erase(it);
                continue;
            }

            // 🎯 【A. 處理怪物的持續傷害 (DOT) 結算】
            // 必須確保怪物在扣血前還活著，避免對屍體鞭屍
            if (this->isAlive()) {
                if (it->first == POISON) {
                    int poisonDamage = 0;
                    int poisonDamageRate = 20; // 基礎中毒傷害
                    if (rank == ELITE && rank == BOSS) {
                        poisonDamage = round(0.05 * (maxHp-hp));
                    }
                    else {
                        poisonDamage = round(0.2 * (maxHp-hp));
                    }
                    
                    cout << "🤢 怪物 [" << name << "] 毒發攻心！\n";
                    this->takeDamage(poisonDamage);
                }
            }

            // 🎯 【B. 狀態剩餘回合數減 1】
            it->second--;

            // 🎯 【C. 檢查減完後是否過期】
            if (it->second == 0) {
                // 完美調用剛剛做好的共享 getEffectName
                cout << "💨 怪物 [" << name << "] 的 " << getEffectName(it->first) << " 狀態解除了。\n";
                
                it = StatusEffectList.erase(it); // 安全刪除，並讓迭代器自動指向下一個元素
            } else {
                ++it; // 沒過期，正常指針往下走
            }
        }
    }
}