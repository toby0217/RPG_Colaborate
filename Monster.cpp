#include "Monster.h"
#include "Player.h"
#include <string>
#include <cstdlib>
#include <iostream>
using std::cout;
using std::endl;

namespace RPG_Colaborate
{
    //預設建構子，內容隨便填的，建議不用
    Monster::Monster(): 
    name("monster"), hp(100), attackPower(10), rewardGold(10) {}

    //建構子
    Monster::Monster(const string& name,int hp,int attackPower,int rewardGold):
    name(name),hp(hp),attackPower(attackPower),rewardGold(rewardGold) {}

    //getters
    string Monster::getName() const { return name; }
    int Monster::getHp() const { return hp; }
    int Monster::getAttackPower() const { return attackPower; }
    int Monster::getRewardGold() const { return rewardGold; }

    //setters
    void Monster::setName(const string& newName) { name = newName; }
    void Monster::setHp(int newHp) { hp = newHp; }
    void Monster::setRewardGold(int newRewardGold) { rewardGold = newRewardGold; }
    void Monster::setAttackPower(int newAttackPower) { attackPower = newAttackPower; }

    //function
    //攻擊我先用作業的方法
    //留:待新增怪物再調整
    void Monster::attack(Player& player) const
    {
        int damage;
        if(attackPower>0) {
            damage = rand()%attackPower + 1;
            //傷害1~attack power隨機
        }
        else{
            damage = 0;
            //防止attackPower <= 0時出問題
        }

        cout<<name<<" attack "<<player.getName()<<" deal "<<damage<<" damage."<<endl;
        //這裡會輸出訊息

        player.takeDamage(damage);
    }

    //這裡我對應player.cpp寫的，保持邏輯一致
    //已調整
    void Monster::takeDamage(int damage){
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