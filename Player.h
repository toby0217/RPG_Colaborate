#ifndef PLAYER_H
#define PLAYER_H

#include "BattleManager.h"
#include <string>
#include <map>
using std::string;
using std::map;

namespace RPG_Colaborate
{
    // Forward declarations to avoid circular dependencies
    class Monster;
    class Item;
    class Skill;

    class Player
    {
    protected:
        /* Basic attributes */
        string name;
        string job;
        int hp;
        int maxHp;
        int attackPower;
        int mp;
        int maxMp;
        int defense;
        map<int, Item> items;
        Skill* skillbox[3];
        map<EffectType, int> StatusEffectList;
    public:
        Player();
        Player(string theName, int theMaxHp, int theMaxMp, int theAttackPower, int theDefense);
        virtual ~Player();

        // Getters
        string getName() const;
        string getJob() const;
        int getHp() const;
        int getMaxHp() const;
        int getAttackPower() const;
        int getMp() const;
        int getMaxMp() const;
        int getDefense() const;

        // Setters
        void setName(string newName);
        void setHp(int newHp);
        void setMaxHp(int newMaxHp);
        void setAttackPower(int newAttack);
        void setMp(int newMp);
        void setMaxMp(int newMaxMp);
        void setDefense(int newDefense);

        bool consumeMp(int amount);

        // Core combat and interaction actions
        virtual void attack(int targetIndex, vector<Monster*> monsters, vector<Player*> players);
        virtual void takeDamage(int damage);
        bool useItem(int itemCode);

        bool useItemOutOfBattle(int itemCode, Player& target);

        virtual bool useSkill(int skillNumber, int targetIndex, vector<Player*> players, vector<Monster*> monsters);
        bool isAlive();

        // 在 public: 底下找地方補上這兩行
        void heal(int amount);
        //void restoreMp(int amount);

        string getBuffs();
        string getDebuffs();

        void takeEffect(EffectType& effectType, int effectTurns);
        void reviveWithHp(int reviveHp);
        virtual void triggerClassSpecial(EffectType& type);
    };
}

#endif
