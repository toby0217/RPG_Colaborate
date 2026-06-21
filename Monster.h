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
        map<EffectType, int> StatusEffectList;

    public:
        //建構子
        Monster();
        Monster(const string& name,int hp,int attackPower,int rewardGold,int evadeRate,int defense);


        //getters
        string getName()const;
        int getHp()const;
        int getMaxHp()const;
        int getAttackPower()const;
        int getRewardGold()const;
        int getEvadeRate()const;
        int getDefense()const;


        //setters
        void setName(const string& newName);
        void setHp(int newHp);
        void setMaxHp(int newMaxHp);
        void setAttackPower(int newAttackPower);
        void setRewardGold(int newRewardGold);
        void setEvadeRate(int newEvadeRate);
        void setDefense(int newDefense);


        //function
        void attack(Player& player)const;
        void takeDamage(int damage);
        bool isAlive()const;
        void showInfo()const;

        string getBuffs();
        string getDebuffs();

        //  修改：補上 int turns 參數，並新增檢查與扣減狀態的函式
        void takeEffect(EffectType effectType, int turns);
        void decrementStatusEffects();
        bool isShocked();
    };
}

#endif
