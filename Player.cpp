#include "Player.h"
#include <iostream>

namespace RPG_Colaborate
{
    // Constructor
    Player::Player(const std::string& playerName, int startingHp, int startingAttack)
        : name(playerName), hp(startingHp), maxHp(startingHp), attackPower(startingAttack) {}

    // Destructor
    Player::~Player() {}

    // Getters and Setters
    std::string Player::getName() const { return name; }
    int Player::getHp() const { return hp; }
    int Player::getMaxHp() const { return maxHp; }
    int Player::getAttackPower() const { return attackPower; }
    
    void Player::setHp(int newHp) {
        hp = newHp;
        if (hp < 0) hp = 0; // Prevent negative HP
    }
    void Player::setAttackPower(int newAttack) { attackPower = newAttack; }

    // TODO: Implement attack, takeDamage, useItem, useSkill
}
    
