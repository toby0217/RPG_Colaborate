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

    public:
        //建構子
        Monster();
        Monster(const string& name,int hp,int attackPower,int rewardGold);


        //getters
        string getName()const;
        int getHp()const;
        int getAttackPower()const;
        int getRewardGold()const;


        //setters
        void setName(const string& newName);
        void setHp(int newHp);
        void setAttackPower(int newAttackPower);
        void setRewardGold(int newRewardGold);


        //function
        void attack(Player& player)const;
        void takeDamage(int damage);
        bool isAlive()const;
        void showInfo()const;
    };
}

#endif