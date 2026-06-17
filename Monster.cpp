#include "Monster.h"
#include "Player.h"
#include <string>
#include <cstdlib>
#include <iostream>
namespace RPG_Colaborate
{
    //預設建構子，內容隨便填的，建議不用
    Monster::Monster(): 
    name("monster"), hp(100), attackPower(10), rewardGold(10) {}

    //建構子
    Monster::Monster(const std::string& name,int hp,int attackPower,int rewardGold):
    name(name),hp(hp),attackPower(attackPower),rewardGold(rewardGold){}

    //getter
    std::string Monster::getName()const{
        return name;
    }
    int Monster::getHp()const{
        return hp;
    }
    int Monster::getAttackPower()const{
        return attackPower;
    }
    int Monster::getRewardGold()const{
        return rewardGold;
    }

    //setter
    void Monster::setName(const std::string& newName){
        name = newName;
    }
    void Monster::setHp(int newHp){
        hp = newHp;
    }
    void Monster::setRewardGold(int newRewardGold){
        rewardGold = newRewardGold;
    }
    void Monster::setAttackPower(int newAttackPower){
        attackPower = newAttackPower;
    }

    //function
    //攻擊我先用作業的方法
    void Monster::attack(Player& player)const{
        int damage;
        if(attackPower>0){
            damage = rand()%attackPower + 1;
            //傷害1~attack power隨機
        }
        else{
            damage = 0;
            //防止attackPower <= 0時出問題
        }


        std::cout<<name<<" attack "<<player.getName()<<" deal "<<damage<<" damage."<<std::endl;
        //這裡會輸出訊息

        player.takeDamage(damage);
    }

    //這裡我對應player.cpp寫的，保持邏輯一致
    void Monster::takeDamage(int damage){
        hp -= damage;

        std::cout<<name<<" take "<<damage<<" point of damage!"<<std::endl
                 <<"(Current hp:"<<hp<<")"<<std::endl;
        //這裡也會輸出訊息

        if(hp<=0){
            std::cout << name << " has been defeated..." << std::endl;
        }
    }

    bool Monster::isAlive()const{
        return hp>0;
    }

    void Monster::showInfo()const{
        std::cout<<"Monster name:"<<name<<std::endl;
        std::cout<<"Monster hp:"<<hp<<std::endl;
        std::cout<<"Monster attack:"<<attackPower<<std::endl;
        std::cout<<"Monster reward gold:"<<rewardGold<<std::endl;
    }

}