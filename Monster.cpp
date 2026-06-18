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
    int Monster::getAttackPower() const { return attackPower; }
    int Monster::getRewardGold() const { return rewardGold; }
    int Monster::getEvadeRate() const { return evadeRate; }
    int Monster::getDefense() const { return defense; }

    //setters
    void Monster::setName(const string& newName) { name = newName; }
    void Monster::setHp(int newHp) {
        hp = newHp;

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

}