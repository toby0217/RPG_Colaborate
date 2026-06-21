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
    name("monster"), hp(100), attackPower(10), rewardGold(10), evadeRate(0), defense(0) {}

    //建構子
    Monster::Monster(const string& name,int hp,int attackPower,int rewardGold,int evadeRate,int defense):
    name(name), hp(hp), attackPower(attackPower), rewardGold(rewardGold), evadeRate(evadeRate), defense(defense) {}

    //getters
    string Monster::getName() const { return name; }
    int Monster::getHp() const { return hp; }
    int Monster::getMaxHp() const { return maxHp; }
    int Monster::getAttackPower() const { return attackPower; }
    int Monster::getRewardGold() const { return rewardGold; }
    int Monster::getEvadeRate() const { return evadeRate; }
    int Monster::getDefense() const { return defense; }

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

    //function
    void Monster::attack(Player& player) const
    {
        //已調整:直接依照攻擊力造成傷害
        cout<<name<<" launch an attack!"<<endl;
        //這裡會輸出訊息

        player.takeDamage(attackPower);
    }

    //這裡我對應player.cpp寫的，保持邏輯一致
    //已調整
    void Monster::takeDamage(int damage){
        // 觸電狀態：受到傷害增加 50%
        if(StatusEffectList[SHOCKED] > 0){
            damage = round(damage * 1.5);
            cout << " 觸電脆弱！" << name << " 受到了額外 50% 傷害！\n";
        }

        if(evadeRate > 0){
            if(rand()%100 < evadeRate){
                cout << name << " evade attack!" << endl;
                return;
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
    // 新增這三個狀態管理函式
    void Monster::takeEffect(EffectType effectType, int turns) {
        StatusEffectList[effectType] = turns;
        if (effectType == SHOCKED) {
            cout << "⚡ " << name << " 陷入了觸電狀態，全身麻痺！\n";
        }
    }

    void Monster::decrementStatusEffects() {
        for (auto& effect : StatusEffectList) {
            if (effect.second > 0) {
                effect.second--;
            }
        }
    }

    bool Monster::isShocked() {
        return StatusEffectList[SHOCKED] > 0;
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

}
