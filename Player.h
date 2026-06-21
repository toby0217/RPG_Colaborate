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

        // ✨ 新增道具狀態變數
        int tempAtkBonus = 0;
        bool hasSwiftBuff = false;
        int invincibleTurns = 0;
        bool hasLastGasp = false;
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

        //  修改：加入 monsters 以支援全體攻擊道具
        bool useItem(int itemCode, vector<Monster*>& monsters); 
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

        // 新增道具狀態操作函式
        void applyStrengthBuff(int amount);
        void applySwiftBuff();
        bool consumeSwiftBuff();
        void applyGoldenBell();
        void decrementInvincibleTurns();
        void applyLastGasp();
        bool checkAndConsumeLastGasp();
        void applyCalamityBuff();
        int getTempCritRateBonus() const;
        int getTempCritEffectBonus() const;
        void clearBattleBuffs(); // 戰鬥結束後呼叫，清除跨場 Buff
    };
}

#endif
