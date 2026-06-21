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
    name("monster"), hp(100), attackPower(10), rewardGold(10), evadeRate(0), defense(0), rank(NORMAL) {}

    //建構子
    Monster::Monster(const string& name,int hp,int attackPower,int rewardGold,int evadeRate,int defense, MonsterRank rank):
    name(name), hp(hp), attackPower(attackPower), rewardGold(rewardGold), evadeRate(evadeRate), defense(defense), rank(rank){}

    //getters
    string Monster::getName() const { return name; }
    int Monster::getHp() const { return hp; }
    int Monster::getMaxHp() const { return maxHp; }
    int Monster::getAttackPower() const { return attackPower; }
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
    void Monster::attack(Player& player)
    {
        //已調整:直接依照攻擊力造成傷害
        cout<<name<<" launch an attack!"<<endl;
        //這裡會輸出訊息

        player.takeDamage(attackPower);
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
        if(defense>0){
            damage = round(damage * (1 - ( defense / (defense + 100) )));
        }
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

       
   
    // 新增這三個狀態管理函式by 王懷賢這個是那個閃電道具的
     // 2. 承受狀態異常（例如：被施加 BURN 3 回合）
    void Monster::takeEffect(EffectType effectType, int turns) {
        // 假設你在 Monster.h 裡的 map 變數叫做 effects (即 map<EffectType, int> effects;)
        // 這裡直接將該狀態的回合數更新或寫入
        StatusEffectList[effectType] = turns;
        if (effectType == SHOCKED) {
        
            cout << "⚡ " << name << " 陷入了觸電狀態，全身麻痺！\n";
        }
    }
    //底下是我原本寫的但我看已經有雛型就先弄成筆記
    /*void Monster::decrementStatusEffects() {
        for (auto& effect : StatusEffectList) {
            if (effect.second > 0) {
                effect.second--;
            }
        }
    }

    bool Monster::isShocked() {
        return StatusEffectList[SHOCKED] > 0;
    }*/

    // 3. 取得某個狀態剩餘的回合數
    int Monster::getEffects(EffectType effectType) {
        // 檢查 map 裡面有沒有這個狀態，有的話回傳剩餘回合，沒有就回傳 0
        if (StatusEffectList.find(effectType) != StatusEffectList.end()) {
            return StatusEffectList[effectType];
        }
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

}
