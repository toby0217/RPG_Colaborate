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
    private:
        /* Basic attributes */
        string name;
        int hp;
        int maxHp;
        int attackPower;
        int mp;
        int maxMp;
        map<int, Item> items;
        Skill* skillbox[3];
    public:
        Player(string name, int maxHp, int maxMp, int attackPower);
        ~Player();

        // Getters
        string getName() const;
        int getHp() const;
        int getMaxHp() const;
        int getAttackPower() const;
        int getMp() const;
        int getMaxMp() const;

        // Setters
        void setName(string mewName);
        void setHp(int newHp);
        void setMaxHp(int newMaxHp);
        void setAttackPower(int newAttack);
        void setMp(int newMp);
        void setMaxMp(int newMaxMp);

        bool consumeMp(int amount);

        // Core combat and interaction actions
        void attack(Monster& target);
        void takeDamage(int damage);
        bool useItem(int itemCode);
        bool useSkill(int skillNumber, Monster& target);
        bool isAlive();
    };
}

#endif