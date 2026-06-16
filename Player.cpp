#include "Player.h"
#include "Item.h"  
#include "Skill.h"
#include "Monster.h"
#include <iostream>


namespace RPG_Colaborate
{
    // Constructor
    Player::Player(std::string name, int maxHp, int maxMp, int attackPower) 
    : name(name), maxHp(maxHp), hp(maxHp), maxMp(maxMp), mp(maxMp), attackPower(attackPower) {}

    // Destructor
    Player::~Player() {}

    // Getters and Setters
    std::string Player::getName() const { return name; }
    int Player::getHp() const { return hp; }
    int Player::getMaxHp() const { return maxHp; }
    int Player::getAttackPower() const { return attackPower; }

    int Player::getMp() const { return mp; }
    int Player::getMaxMp() const { return maxMp; }

    bool Player::consumeMp(int amount) {
        if (mp >= amount) {
            mp -= amount;
            std::cout << name << " consumed " << amount << " MP! (remaining MP: " << mp << ")\n";
            return true;
        } else {
            std::cout << name << " insufficient MP! Cannot perform skill!\n";
            return false;
        }
    }

        // 1. Basic Attack
    void Player::attack(Monster& target) {
        std::cout << name << " performs a basic attack!" << std::endl;
        
        // TODO: Call target.takeDamage() depending on Monster's implementation
        // Example: target.takeDamage(this->attackPower);
    }

    // 2. Take Damage
    void Player::takeDamage(int damage) {
        hp -= damage;
        if (hp < 0) {
            hp = 0; // Prevent HP from dropping below zero
        }
        std::cout << name << " takes " << damage << " points of damage! " 
                << "(Current HP: " << hp << "/" << maxHp << ")" << std::endl;
                
        if (hp == 0) {
            std::cout << name << " has been defeated..." << std::endl;
        }
    }
    
    void Player::setHp(int newHp) {
        hp = newHp;
        if (hp < 0) hp = 0; // Prevent negative HP
    }
    void Player::setAttackPower(int newAttack) { attackPower = newAttack; }

    // 3. Use Item (使用道具)
    void Player::useItem(Item& item) {
        // 檢查道具是否還有剩
        if (!item.isAvailable()) {
            std::cout << name << " tries to use [" << item.getName() << "], but it's empty!" << std::endl;
            return;
        }

        std::cout << name << " uses an item: [" << item.getName() << "]!" << std::endl;

        // 根據道具類型來決定效果 (假設 "Heal" 類型是補血)
        if (item.getType() == "Heal") {
            int healAmount = item.getEffectValue();
            hp += healAmount;
            
            // 確保補血不會超過最大血量
            if (hp > maxHp) {
                hp = maxHp;
            }
            std::cout << name << " recovers " << healAmount << " HP! "
                    << "(Current HP: " << hp << "/" << maxHp << ")" << std::endl;
        }

        // 呼叫道具本身的 use()，讓它自己去把 quantity (數量) 扣掉 1
        item.use(); 
    }

    // 4. Use Skill (使用技能)
    void Player::useSkill(Skill& skill, Monster& target) {
        int cost = skill.getMpCost();

        // 呼叫我們之前寫好的 consumeMp，如果魔力夠就會自動扣除並回傳 true
        if (this->consumeMp(cost)) {
            std::cout << name << " casts a skill: [" << skill.getName() << "]!" << std::endl;
            
            // 回應隊友的註解：我們可以把 Player 的攻擊力與技能傷害結合
            // 這裡示範「角色攻擊力 + 技能額外傷害」的算法，若隊友改成倍率也可以輕鬆改成相乘
            int totalDamage = this->attackPower + skill.getDamage();
            
            std::cout << "The skill deals a total of " << totalDamage << " damage!" << std::endl;
            
            // 真正讓怪物扣血
            target.takeDamage(totalDamage);

            // 呼叫技能本身的 use()，也許隊友以後會在這裡加上特效文字
            skill.use();
        } 
        // 如果魔力不夠，consumeMp 裡面已經會印出失敗訊息了，所以這裡不用寫 else
    }

    
}
    
