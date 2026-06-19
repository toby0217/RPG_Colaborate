#include "BountyHunter.h"
#include "Monster.h" // 引入長輩檔案

#include "Skill.h"
#include <iostream>
#include <cstdlib>

namespace RPG_Colaborate {

    BountyHunter::BountyHunter(string theName, int theMaxHp, int theMaxMp, int theAttackPower, int theDefense, int theGold)
        : Player(theName, theMaxHp, theMaxMp, theAttackPower, theDefense), gold(theGold), bulletType(0), goldenMpConsumed(0), goldenBulletTurns(0) {
        
        this->criticalRate = 25;     
        this->criticalEffect = 180;  

        // 根據金幣數量微調初始屬性
        this->attackPower += (gold / 100) * 2;
        this->maxMp += (gold / 100) * 5;
        this->mp = this->maxMp; 

        skillbox[0] = new Skill("Red Bullet", "Buff", 0, 0);     
        skillbox[1] = new Skill("Blue Bullet", "Buff", 0, 0);    
        skillbox[2] = new Skill("Golden Bullet", "Buff", 0, 10); 
    }

    BountyHunter::~BountyHunter() {}

    void BountyHunter::takeDamage(int damage) {
        if (goldenBulletTurns > 0) {
            damage = damage * 0.5; // 減傷 50%
            std::cout << "[Golden Armor] Protected by the Golden Bullet buff, damage reduced by 50%!" << std::endl;
        }
        Player::takeDamage(damage); 
    }

    void BountyHunter::attackShotgun(Monster* primaryTarget, std::vector<Monster*>& allEnemies) {
        if (goldenBulletTurns > 0) {
            goldenBulletTurns--;
        }

        std::cout << name << " fires the shotgun!" << std::endl;
        
        int baseDamage = this->attackPower * 0.4; 
        int bonusDamage = 0;

        if (bulletType == 1) { 
            bonusDamage = this->attackPower * 0.8;
            std::cout << "Red Bullet detonates! Dealing massive extra damage!" << std::endl;
            bulletType = 0; 
        } else if (bulletType == 2) { 
            bonusDamage = this->attackPower * 0.2;
            this->mp = std::min(this->maxMp, this->mp + 20); 
            std::cout << "Blue Bullet absorbs magic energy! Restored 20 MP." << std::endl;
            bulletType = 0;
        }

        std::cout << "[Primary Target] Hit 3 times!" << std::endl;
        primaryTarget->takeDamage((baseDamage * 3) + bonusDamage);

        // 霰彈槍散射效果
        if (allEnemies.size() > 1) {
            std::cout << "[Spread] Stray bullets scatter!" << std::endl;
            for (int i = 0; i < 2; ++i) {
                int randIndex = rand() % allEnemies.size();
                allEnemies[randIndex]->takeDamage(baseDamage);
                std::cout << "-> Stray bullet grazes " << allEnemies[randIndex]->getName() << std::endl;
            }
        }
    }

    void BountyHunter::loadRedBullet() {
        this->bulletType = 1;
        std::cout << "Quickly loaded Red Bullet (Free Action)! Next attack damage boosted." << std::endl;
    }

    void BountyHunter::loadBlueBullet() {
        this->bulletType = 2;
        std::cout << "Quickly loaded Blue Bullet (Free Action)! Next attack will siphon MP." << std::endl;
    }

    void BountyHunter::castGoldenBullet() {
        std::cout << name << " consumes all MP! Dual guns transform into pure gold!" << std::endl;
        
        this->goldenMpConsumed = this->mp; 
        this->mp = 0;                       
        this->bulletType = 3;               
        this->goldenBulletTurns = 2;        
        
        std::cout << "For the next 2 turns: Gains 50% damage reduction, but other skills are sealed!" << std::endl;
    }

} // namespace RPG_Colaborate