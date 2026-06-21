#ifndef MONSTER_H
#define MONSTER_H

#include<string>
#include<vector>
#include <map>
#include "BattleManager.h"
using std::string;
using std::vector;
using std::map;

namespace RPG_Colaborate
{
    class Player;

    enum EffectType; // 假設原本有的狀態類型

    // 1. 新增等級/稀有度列舉
    enum MonsterRank {
        NORMAL,  // 小怪
        ELITE,   // 精英
        BOSS     // Boss
    };
    
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
        MonsterRank rank;

    public:
        //建構子
        Monster();
        Monster(const string& theName,int theMaxHp,int theAttackPower,int theRewardGold,int theEvadeRate,int theDefense, MonsterRank theRank);


        //getters
        string getName()const;
        int getHp()const;
        int getMaxHp()const;
        int getAttackPower()const;
        int getRewardGold()const;
        int getEvadeRate()const;
        int getDefense()const;
        MonsterRank getRank()const;


        //setters
        void setName(const string& newName);
        void setHp(int newHp);
        void setMaxHp(int newMaxHp);
        void setAttackPower(int newAttackPower);
        void setRewardGold(int newRewardGold);
        void setEvadeRate(int newEvadeRate);
        void setDefense(int newDefense);
        void setRank(const MonsterRank& theRank);


        int calculateFinalDamage(int rawDamage);
        //function
        virtual void attack(int targetIndex, vector<Player*>& players, vector<Monster*>& monsters);
        void takeDamage(int damage);
        bool isAlive()const;
        void showInfo()const;

        string getBuffs();
        string getDebuffs();

        void takeEffect(const EffectType& effectType, int effectTurns);
        int getEffectTurns(const EffectType& effectType) const;
        void updateStatusEffects();
    };
}

#endif