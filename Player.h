#ifndef PLAYER_H
#define PLAYER_H

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
        std::string job;
        int hp;
        int maxHp;
        int attackPower;
        int mp;
        int maxMp;
        int defense;
        map<int, Item> items;
        Skill* skillbox[3];
    public:
        Player::Player(std::string n, std::string j, int h, int m, int atk, int def);
        ~Player();

        // Getters
        string getName() const;
        int getHp() const;
        int getMaxHp() const;
        int getAttackPower() const;
        int getMp() const;
        int getMaxMp() const;
        std::string getJob() const;
        // Add this under public: in Player.h
        int getBaseAttack() const ;

        // Setters
        void setName(string mewName);
        void setHp(int newHp);
        void setMaxHp(int newMaxHp);
        void setAttackPower(int newAttack);
        void setMp(int newMp);
        void setMaxMp(int newMaxMp);
        void setDefense(int def);
        void setBaseAttack(int atk);

        bool consumeMp(int amount);

        // Core combat and interaction actions
        virtual void attack(Monster& target);
        virtual void takeDamage(int damage);
        bool useItem(int itemCode);
        bool useSkill(int skillNumber, Monster& target);
        bool isAlive();

        // 在 public: 底下找地方補上這兩行
        void heal(int amount);
        void restoreMp(int amount);
    };
}

#endif