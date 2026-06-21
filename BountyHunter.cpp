#include "BountyHunter.h"
#include "Monster.h" 
#include "Skill.h"
#include <iostream>
#include <cstdlib>
#include <vector>

namespace RPG_Colaborate {

    BountyHunter::BountyHunter(string theName, int theMaxHp, int theMaxMp, int theAttackPower, int theDefense, int theGold)
        : Player(theName, theMaxHp, theMaxMp, theAttackPower, theDefense), gold(theGold), bulletType(0), goldenMpConsumed(0), goldenBulletTurns(0) {
        
        this->criticalRate = 25;     
        this->criticalEffect = 180;  

        job = "BountyHunter"; // 補上職業名稱

        // 根據金幣數量微調初始屬性
        this->attackPower += (gold / 100) * 2;
        this->maxMp += (gold / 100) * 5;
        this->mp = this->maxMp; 

        // ⚠️ 貼心提醒：這裡使用的 4 參數 Skill 建構子未來可能會引發未定義行為，
        // 建議有空時將其換成完整的 18 參數建構子。
        skillbox[0] = new Skill("Red Bullet", "Buff", 0, 0);     
        skillbox[1] = new Skill("Blue Bullet", "Buff", 0, 0);    
        skillbox[2] = new Skill("Golden Bullet", "Buff", 0, 10); 
    }

    BountyHunter::~BountyHunter() {}

    void BountyHunter::takeDamage(int damage) {
        if (goldenBulletTurns > 0) {
            damage = damage * 0.5; // 減傷 50%
            std::cout << "🛡️ [Golden Armor] Protected by the Golden Bullet buff, damage reduced by 50%!" << std::endl;
        }
        Player::takeDamage(damage); 
    }

    // ✨ 將霰彈槍邏輯整合進普攻，並支援暴擊與災厄之手 Buff
    void BountyHunter::attack(int targetIndex, std::vector<Monster*> monsters, std::vector<Player*> players) {
        if (goldenBulletTurns > 0) {
            goldenBulletTurns--;
        }

        int currentCritRate = criticalRate + getTempCritRateBonus();
        int currentCritEffect = criticalEffect + getTempCritEffectBonus();

        std::cout << name << " fires the shotgun!" << std::endl;
        
        int baseDamage = this->attackPower * 0.4; 
        int bonusDamage = 0;

        if (bulletType == 1) { 
            bonusDamage = this->attackPower * 0.8;
            std::cout << "🔥 Red Bullet detonates! Dealing massive extra damage!" << std::endl;
            bulletType = 0; 
        } else if (bulletType == 2) { 
            bonusDamage = this->attackPower * 0.2;
            this->mp = std::min(this->maxMp, this->mp + 20); 
            std::cout << "💧 Blue Bullet absorbs magic energy! Restored 20 MP." << std::endl;
            bulletType = 0;
        }

        // 計算主目標傷害並判定暴擊
        int finalPrimaryDamage = (baseDamage * 3) + bonusDamage;
        if (rand() % 100 < currentCritRate) {
            finalPrimaryDamage = finalPrimaryDamage * currentCritEffect / 100;
            std::cout << " Critical Hit! ";
        }
        std::cout << "[Primary Target] Hit 3 times!" << std::endl;
        monsters[targetIndex]->takeDamage(finalPrimaryDamage);

        // 霰彈槍散射效果 (隨機流彈)
        if (monsters.size() > 1) {
            std::cout << "[Spread] Stray bullets scatter!" << std::endl;
            
            // 篩選出活著的怪物，避免流彈打到屍體或引發崩潰
            std::vector<Monster*> aliveMonsters;
            for (auto m : monsters) {
                if (m != nullptr && m->isAlive()) {
                    aliveMonsters.push_back(m);
                }
            }

            for (int i = 0; i < 2; ++i) {
                if (aliveMonsters.empty()) break; 

                int randIndex = rand() % aliveMonsters.size();
                int strayDamage = baseDamage;
                
                // 流彈也有機率暴擊
                if (rand() % 100 < currentCritRate) {
                    strayDamage = strayDamage * currentCritEffect / 100;
                }
                
                std::cout << "-> Stray bullet grazes " << aliveMonsters[randIndex]->getName() << "!" << std::endl;
                aliveMonsters[randIndex]->takeDamage(strayDamage);
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

}
