#ifndef MONSTER_H
#define MONSTER_H
#include<string>
namespace RPG_Colaborate
{
    class Player;
    
    class Monster
    {
    private:
        std::string name;
        int hp;
        int attackPower;
        int rewardGold;

    public:
        //建構子
        Monster();
        Monster(const std::string& name,int hp,int attackPower,int rewardGold);


        //getter
        std::string getName()const;
        int getHp()const;
        int getAttackPower()const;
        int getRewardGold()const;


        //setter
        void setName(const std::string& name);
        void setHp(int hp);
        void setAttackPower(int attackPower);
        void setRewardGold(int rewardGold);


        //function
        void attack(Player& player)const;
        void takeDamage(int damage);
        bool isAlive()const;
        void showInfo()const;

    };
}

#endif