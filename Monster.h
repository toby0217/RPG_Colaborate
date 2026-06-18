#ifndef MONSTER_H
#define MONSTER_H

#include<string>
using std::string;

namespace RPG_Colaborate
{
    class Player;
    
    class Monster
    {
    private:
        string name;
        int hp;
        int maxHp;
        int attackPower;
        int rewardGold;
        int evadeRate;
        int defense;

    public:
        //建構子
        Monster();
        Monster(const string& name,int hp,int attackPower,int rewardGold,int evadeRate,int defense);


        //getters
        string getName()const;
        int getHp()const;
        int getAttackPower()const;
        int getRewardGold()const;
        int getEvadeRate()const;
        int getDefense()const;


        //setters
        void setName(const string& newName);
        void setHp(int newHp);
        void setAttackPower(int newAttackPower);
        void setRewardGold(int newRewardGold);
        void setEvadeRate(int newEvadeRate);
        void setDefense(int newDefense);


        //function
        void attack(Player& player)const;
        void takeDamage(int damage);
        bool isAlive()const;
        void showInfo()const;
    };
}

#endif