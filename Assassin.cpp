#include "Assassin.h"
#include <iostream>
#include <vector>
#include <algorithm> // For std::sort
using std::vector;
using std::sort;
using std::cout;
using std::endl;

namespace RPG_Colaborate {
    // Constructors
    Assassin::Assassin()
    : Player(), criticalRate(15), criticalEffect(200), turnCounter(0)
    {
        job = "Assassin";
        skillbox[0] = new Skill("Cruel Kunai", SINGLE, NONEH, POISON, 2,
            DAMAGE, NONE, NONE, DEBUFF, NONE, NONE, NONE, attackPower, 1.2, 0, 30, 0, 3);
        skillbox[1] = new Skill("Shadow Vanish", OWN, NONEH, HIDE, 1,
            NONE, STATIC, NONE, NONE, NONE, NONE, NONE, attackPower, 1.6, 0, 40, 0, 3);
        skillbox[2] = new Skill("Nightmare Reap", SINGLE, NONEH, NONEE, 0,
            DAMAGE, NONE, NONE, DEBUFF, NONE, NONE, SPECIAL, attackPower, 2.5, 0, 60, 0, 6);
    }
    Assassin::Assassin(string theName, int theMaxHp, int theMaxMp, int theAttackPower, int theDefense)
    : Player(theName, theMaxHp, theMaxMp, theAttackPower, theDefense), 
      criticalRate(15), criticalEffect(200), turnCounter(0)
    {
        job = "Assassin";
        skillbox[0] = new Skill("Cruel Kunai", SINGLE, NONEH, POISON, 2,
            DAMAGE, NONE, NONE, DEBUFF, NONE, NONE, NONE, attackPower, 1.2, 0, 30, 0, 3);
        skillbox[1] = new Skill("Shadow Vanish", OWN, NONEH, HIDE, 1,
            NONE, STATIC, NONE, NONE, NONE, NONE, NONE, attackPower, 1.6, 0, 40, 0, 3);
        skillbox[2] = new Skill("Nightmare Reap", SINGLE, NONEH, NONEE, 0,
            DAMAGE, NONE, NONE, DEBUFF, NONE, NONE, SPECIAL, attackPower, 2.5, 0, 60, 0, 6);
    }

    Assassin::~Assassin()
    {
        for (int i = 0; i < 3; i++) {
            if (skillbox[i] != nullptr) {
                delete skillbox[i];
                skillbox[i] = nullptr;
            }
        }
    }

    // getters
    int Assassin::getCriticalRate() const { return criticalRate; }
    int Assassin::getCriticalEffect() const { return criticalEffect; }
    int Assassin::getTurnCounter() const { return turnCounter; }

    // setters
    void Assassin::setCriticalRate(int newRate) { criticalRate = newRate; }
    void Assassin::setCriticalEffect(int newEffect) { criticalEffect = newEffect;}

    // 被動突襲
    void Assassin::executeTurnActions(vector<Monster*>& monsters) {
        turnCounter++;

        // Trigger assault every 2 rounds
        if (turnCounter % 2 == 0) {
            // Filter out already defeated monsters
            vector<Monster*> aliveMonsters;
            for (auto monster : monsters) {
                if (monster != nullptr && monster->getHp() > 0) {
                    aliveMonsters.push_back(monster);
                }
            }

            if (aliveMonsters.empty()) return;

            // Sort monsters by HP in ascending order (Lowest HP first)
            sort(aliveMonsters.begin(), aliveMonsters.end(), [](Monster* a, Monster* b) {
                return a->getHp() < b->getHp();
            });

            cout << "⚡ [Assassin Passive] Round " << turnCounter << ": Launching a surprise assault!" << endl;

            // Attack up to 2 monsters with the lowest HP
            int targetsAttacked = 0;
            for (auto target : aliveMonsters) {
                if (targetsAttacked >= 2) break;

                cout << "🗡️ Assailing " << target->getName() << " (Lowest HP target)!" << endl;
                // Deal damage based on Assassin's attack power
                target->takeDamage(getAttackPower());
                targetsAttacked++;
            }
        }
    }

    void Assassin::attack(int targetIndex, vector<Monster*>& monsters, vector<Player*>& players)
    {
        Player::attack(targetIndex, monsters, players);
        executeTurnActions(monsters);
    }

    bool Assassin::useSkill(int skillInput, int targetIndex, vector<Player*>& players, vector<Monster*>& monsters)
    {
        int skillNumber = skillInput - 1;
        if (skillNumber < 0 || skillNumber >= 3 || skillbox[skillNumber] == nullptr) {
            cout << "The skill does not exist." << endl;
            return false;
        }

        int mpRequired = skillbox[skillNumber]->getMpCost();
        if (mp < mpRequired) {
            cout << name << " does not have enough MP!" << endl;
            return false;
        }

        // 提前播報技能台詞
        if (skillNumber == 0) {
            cout << "🗡️ [Assassin]: \"A swift end...\"" << endl;
        } else if (skillNumber == 1) {
            cout << "🗡️ [Assassin]: \"Shadows veil me.\"" << endl;
        } else if (skillNumber == 2) {
            cout << "🗡️ [Assassin]: \"Sleep now, into the eternal nightmare.\"" << endl;
        }

        return Player::useSkill(skillInput, targetIndex, players, monsters);
    }

    void Assassin::triggerClassSpecial(Skill& theSkill, int targetIndex, vector<Monster*>& monsters, vector<Player*>& players)
    {
        if (&theSkill == skillbox[2] && !monsters[targetIndex]->isAlive()) {
            cout << "💀 [Nightmare Reap] Target executed! Skill cooldown reset and MP refunded. You may act again!" << endl;
            // 回復 MP
            mp += skillbox[2]->getMpCost();
            if (mp > maxMp) mp = maxMp;
            skillbox[2]->setCurrentCD(0);

            takeEffect(FREEACTION, 1);
                
            // 重置 CD 邏輯 (需在 BattleManager 的技能冷卻系統配合，這裡先重置 Skill 本身狀態若有)
        }
    }
}