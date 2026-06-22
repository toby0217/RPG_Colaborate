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
        int ownedGolds;
        map<int, Item> items;
        Skill* skillbox[3];
        map<EffectType, int> StatusEffectList;

        // ✨ 新增道具狀態變數
        int tempCritRateBonus = 0;
        int tempCritEffectBonus = 0;

    public:
        Player();
        Player(string theName, int theMaxHp, int theMaxMp, int theAttackPower, int theDefense);
        virtual ~Player();

        // Getters
        string getName() const;
        string getJob() const;
        int getHp() const;
        int getMaxHp() const;
        virtual int getAttackPower() const;
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

        virtual void addBountyGold(int gold);
        bool consumeMp(int amount);
        int calculateFinalDamage(int rawDamage);

        // Core combat and interaction actions
        virtual void attack(int targetIndex, vector<Monster*>& monsters, vector<Player*>& players);
        virtual void takeDamage(int damage, vector<Monster*>& monsters);
        bool useItem(int itemCode, vector<Player*>& players, vector<Monster*>& monsters);
        virtual bool useSkill(int skillInput, int targetIndex, vector<Player*>& players, vector<Monster*>& monsters);
        bool isAlive();

        // 在 public: 底下找地方補上這兩行
        void heal(int amount);
        void restoreMp(int amount);


        void takeEffect(const EffectType& effectType, int effectTurns);
        int getEffectTurns(const EffectType& effectType) const;
        void updateStatusEffects();
        void reduceCooldowns();
        
        void reviveWithHp(int reviveHp);
        virtual void triggerClassSpecial(Skill& theSkill, int targetIndex, vector<Monster*>& monsters, vector<Player*>& players);

        int calculateFinalCritRate(int baseRate);
        int calculateFinalCritEffect(double baseEffect);
    };
}

#endif
