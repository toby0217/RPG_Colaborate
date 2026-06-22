#include "Assassin.h"
#include <iostream>
#include <vector>
#include <cstdlib>
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
            DAMAGE, NONE, NONE, DEBUFF, NONE, NONE, SPECIAL, attackPower, 1.2, 0, 30, 0, 3);
        skillbox[1] = new Skill("Shadow Vanish", OWN, NONEH, HIDE, 1,
            NONE, STATIC, NONE, NONE, NONE, NONE, SPECIAL, attackPower, 1.6, 0, 40, 0, 3);
        skillbox[2] = new Skill("Nightmare Reap", SINGLE, NONEH, NONEE, 0,
            DAMAGE, NONE, NONE, DEBUFF, NONE, NONE, SPECIAL, attackPower, 2.5, 0, 60, 0, 6);
    }
    Assassin::Assassin(string theName, int theMaxHp, int theMaxMp, int theAttackPower, int theDefense)
    : Player(theName, theMaxHp, theMaxMp, theAttackPower, theDefense),
      criticalRate(15), criticalEffect(200), turnCounter(0)
    {
        job = "Assassin";
        skillbox[0] = new Skill("Cruel Kunai", SINGLE, NONEH, POISON, 2,
            DAMAGE, NONE, NONE, DEBUFF, NONE, NONE, SPECIAL, attackPower, 1.2, 0, 30, 0, 3);
        skillbox[1] = new Skill("Shadow Vanish", OWN, NONEH, HIDE, 1,
            NONE, STATIC, NONE, NONE, NONE, NONE, SPECIAL, attackPower, 1.6, 0, 40, 0, 3);
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


    //  新增：實作刺客的專屬普攻（支援暴擊與災厄之手加成）
    void Assassin::attack(int targetIndex, vector<Monster*>& monsters, vector<Player*>& players) {
        int currentCritRate = calculateFinalCritRate(criticalRate);
        int currentCritEffect = calculateFinalCritEffect(criticalEffect);
        
        int finalDamage = getAttackPower();
        if (getEffectTurns(LAST_GASP) > 0) {
            finalDamage *= 4;
            takeEffect(LAST_GASP, 0); // ⚡ The moment the attack is unleashed, the status is immediately cleared!
            cout << "🩸 The Last Gasp effect has been released with the attack, status removed.\n";
        }
        if (rand() % 100 < currentCritRate) {
            finalDamage = static_cast<int>(finalDamage * 0.01* currentCritEffect);
            cout << " Critical Hit! ";
        }
        cout << name << " performs a basic attack!" << endl;
        monsters[targetIndex]->takeDamage(getAttackPower());
        executeTurnActions(monsters);
    }

    // 被動突襲
    void Assassin::executeTurnActions(vector<Monster*>& monsters) {
        turnCounter++;

        int damage = getAttackPower();

        if (getEffectTurns(LAST_GASP) > 0) {
            damage *= 4;
            takeEffect(LAST_GASP, 0); // ⚡ The moment the attack is unleashed, the status is immediately cleared!
            cout << "🩸 The Last Gasp effect has been released with the attack, status removed.\n";
        }
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
                target->takeDamage(damage);
                targetsAttacked++;
            }
        }
    }

    bool Assassin::useSkill(int skillInput, int targetIndex, vector<Player*>& players, vector<Monster*>& monsters)
    {
        int skillNumber = skillInput - 1;
        if (skillNumber < 0 || skillNumber >= 3 || skillbox[skillNumber] == nullptr) {
            cout << "The skill does not exist." << endl;
            return false;
        }

        if (skillbox[skillNumber]->getCurrentCD() > 0) {
            cout << "The skill is still in CD!" << endl;
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
        }
        if (getEffectTurns(FREEACTION) == 0) {
            executeTurnActions(monsters);
        }
    }
}
