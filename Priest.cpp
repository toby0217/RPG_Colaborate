#include "Priest.h"
#include "Item.h"
#include "Skill.h"
#include "Monster.h"
#include <iostream>

namespace RPG_Colaborate {

    Priest::Priest(string theName, int theMaxHp, int theMaxMp, int theAttackPower, int theDefense)
        : Player(theName, theMaxHp, theMaxMp, theAttackPower, theDefense), canRevive(true) { 
        // 2. 這裡把 5 個參數原封不動往上傳給 Player
        
        this->criticalRate = 10;
        this->criticalEffect = 150;

        // 技能初始化保持不變
        skillbox[0] = new Skill("Retribution Ray", "Damage", theAttackPower, 25); 
        skillbox[1] = new Skill("Healing Art", "Heal", 0, 50);                    
        skillbox[2] = new Skill("Divine Resurrection", "Revive", 0, 90);          
    }

    Priest::~Priest() {}

    void Priest::triggerPassiveHeal(std::vector<Player*>& teammates) {
        if (teammates.empty()) return;
        
        Player* lowestTarget = nullptr;
        double lowestRatio = 1.0;

        for (Player* p : teammates) {
            if (p->isAlive()) {
                double ratio = (double)p->getHp() / p->getMaxHp();
                if (ratio < lowestRatio) {
                    lowestRatio = ratio;
                    lowestTarget = p;
                }
            }
        }

        if (lowestTarget != nullptr) {
            int healAmount = lowestTarget->getMaxHp() * 0.15;
            lowestTarget->heal(healAmount); 
            std::cout << "[Priest Passive] Automatically healed the lowest HP teammate " 
                      << lowestTarget->getName() << " for 15% HP (" << healAmount << " HP)!" << std::endl;
        }
    }

    void Priest::attackWithHeal(std::vector<Monster*>& targets, int primaryIndex, std::vector<Player*>& teammates) {
        if (targets.empty()) return;
        std::cout << name << " swings the staff for a basic attack!" << std::endl;
        targets[primaryIndex]->takeDamage(this->attackPower);
        
        triggerPassiveHeal(teammates);
    }

    void Priest::castRetributionRay(Monster* target, std::vector<Player*>& teammates) {
        std::cout << name << " casts [Retribution Ray]!" << std::endl;
        target->takeDamage(this->attackPower * 1.3);
        
        int selfHeal = this->maxHp * 0.15;
        this->heal(selfHeal);
        std::cout << "Divine light heals the caster for " << selfHeal << " HP." << std::endl;

        triggerPassiveHeal(teammates);
    }

    void Priest::castHealingArt(std::vector<Player*>& teammates) {
        std::cout << name << " chants [Healing Art]! Holy light covers the battlefield!" << std::endl;
        for (Player* p : teammates) {
            if (p->isAlive()) {
                p->heal(p->getMaxHp() * 0.4);
                p->setDamageBuffTurns(2); 
            }
        }
        triggerPassiveHeal(teammates);
    }

    void Priest::castDivineResurrection(Player* deadTeammate) {
        if (!canRevive) {
            std::cout << "This skill can only be used once per battle!" << std::endl;
            return;
        }
        
        if (deadTeammate != nullptr && !deadTeammate->isAlive()) {
            deadTeammate->reviveWithHp(deadTeammate->getMaxHp() * 0.5); 
            canRevive = false; 
            std::cout << "[Divine Miracle] " << name << " successfully revived teammate " 
                      << deadTeammate->getName() << "!" << std::endl;
        } else {
            std::cout << "Target teammate is still alive, no need to revive." << std::endl;
        }
    }

} // namespace RPG_Colaborate