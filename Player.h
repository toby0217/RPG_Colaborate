#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>

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
        std::string name;
        int hp;
        int maxHp;
        int attackPower;
    public:
        Player(const std::string& playerName, int startingHp, int startingAttack);
        ~Player();

        // Getters and Setters
        std::string getName() const;
        int getHp() const;
        int getMaxHp() const;
        int getAttackPower() const;
        void setHp(int newHp);
        void setAttackPower(int newAttack);

        // Core combat and interaction actions
        void attack(Monster& target);
        void takeDamage(int damage);
        void useItem(Item& item);
        void useSkill(Skill& skill, Monster& target);
    };
}

#endif