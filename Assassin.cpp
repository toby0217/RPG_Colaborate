#include "Assassin.h"
#include <iostream>
#include <vector>
#include <algorithm> // For std::sort

namespace RPG_Colaborate {
    // Constructors
    Assassin::Assassin()
    : Player(), criticalRate(15), criticalEffect(200)
    {
        job = "Assassin";
        *skillbox[0] = Skill("Cruel Kunai", "Damage", attackPower, 30);
        *skillbox[1] = Skill("Shadow Vanish", "Hide", 0, 40);
        *skillbox[2] = Skill("Nightmare Reap", "Damage", 0, 60);
    }
    Assassin::Assassin(string theName, int theMaxHp, int theMaxMp, int theAttackPower, int theDefense)
    : Player(theName, theMaxHp, theMaxMp, theAttackPower, theDefense), 
      criticalRate(15), criticalEffect(200)
    {
        job = "Assassin";
        *skillbox[0] = Skill("Cruel Kunai", "Damage", attackPower, 30);
        *skillbox[1] = Skill("Shadow Vanish", "Hide", 0, 40);
        *skillbox[2] = Skill("Nightmare Reap", "Damage", 0, 60);
    }

    // getters
    int Assassin::getCriticalRate() const { return criticalRate; }
    int Assassin::getCriticalEffect() const { return criticalEffect; }
    int Assassin::getTurnCounter() const { return turnCounter; }

    // setters
    void Assassin::setCriticalRate(int newRate) { criticalRate = newRate; }
    void Assassin::setCriticalEffect(int newEffect) { criticalEffect = newEffect;}

    // 被動突襲
    void Assassin::executeTurnActions(std::vector<Monster*>& monsterList) {
        turnCounter++;

        // Trigger assault every 2 rounds
        if (turnCounter % 2 == 0) {
            // Filter out already defeated monsters
            std::vector<Monster*> aliveMonsters;
            for (auto monster : monsterList) {
                if (monster != nullptr && monster->getHp() > 0) {
                    aliveMonsters.push_back(monster);
                }
            }

            if (aliveMonsters.empty()) return;

            // Sort monsters by HP in ascending order (Lowest HP first)
            std::sort(aliveMonsters.begin(), aliveMonsters.end(), [](Monster* a, Monster* b) {
                return a->getHp() < b->getHp();
            });

            std::cout << "⚡ [Assassin Passive] Round " << turnCounter << ": Launching a surprise assault!" << std::endl;

            // Attack up to 2 monsters with the lowest HP
            int targetsAttacked = 0;
            for (auto target : aliveMonsters) {
                if (targetsAttacked >= 2) break;

                std::cout << "🗡️ Assailing " << target->getName() << " (Lowest HP target)!" << std::endl;
                // Deal damage based on Assassin's attack power
                target->takeDamage(attackPower); 
                targetsAttacked++;
            }
        }
    }
}