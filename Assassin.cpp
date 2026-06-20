#include "Assassin.h"
#include <iostream>
#include <vector>
#include <algorithm> // For std::sort
using std::sort;
using std::cout;
using std::endl;

namespace RPG_Colaborate {
    // Constructors
    Assassin::Assassin()
    : Player(), criticalRate(15), criticalEffect(200)
    {
        job = "Assassin";
        skillbox[0] = new Skill("Cruel Kunai", SINGLE, NONEH, POISON, 2,
            DAMAGE, NONE, NONE, DEBUFF, NONE, NONE, NONE, attackPower, 1.2, 0, 30, 0, 3);
        skillbox[1] = new Skill("Shadow Vanish", OWN, NONEH, HIDE, 1,
            NONE, STATIC, NONE, NONE, NONE, NONE, NONE, attackPower, 1.6, 0, 40, 0, 3);
        skillbox[2] = new Skill("Nightmare Reap", SINGLE, NONEH, NONEE, 0,
            DAMAGE, NONE, NONE, DEBUFF, NONE, NONE, NONE, attackPower, 2.5, 0, 60, 0, 6);
    }
    Assassin::Assassin(string theName, int theMaxHp, int theMaxMp, int theAttackPower, int theDefense)
    : Player(theName, theMaxHp, theMaxMp, theAttackPower, theDefense), 
      criticalRate(15), criticalEffect(200)
    {
        job = "Assassin";
        skillbox[0] = new Skill("Cruel Kunai", SINGLE, NONEH, POISON, 2,
            DAMAGE, NONE, NONE, DEBUFF, NONE, NONE, NONE, attackPower, 1.2, 0, 30, 0, 3);
        skillbox[1] = new Skill("Shadow Vanish", OWN, NONEH, HIDE, 1,
            NONE, STATIC, NONE, NONE, NONE, NONE, NONE, attackPower, 1.6, 0, 40, 0, 3);
        skillbox[2] = new Skill("Nightmare Reap", SINGLE, NONEH, NONEE, 0,
            DAMAGE, NONE, NONE, DEBUFF, NONE, NONE, NONE, attackPower, 2.5, 0, 60, 0, 6);
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

    bool Assassin::useSkill(int skillNumber, int targetIndex, vector<Player*> players, vector<Monster*> monsters)
    {
        if (skillNumber < 0 || skillNumber >= 3 || skillbox[skillNumber] == nullptr) return false;

        int mpRequired = skillbox[skillNumber]->getMpCost();
        if (mp < mpRequired) {
            cout << name << " does not have enough MP!" << endl;
            return false;
        }

        // 播報台詞
        if (skillNumber == 0) cout << "🗡️ [Assassin]: \"A swift end...\"" << endl;
        else if (skillNumber == 1) cout << "🗡️ [Assassin]: \"Shadows veil me.\"" << endl;
        else if (skillNumber == 2) cout << "🗡️ [Assassin]: \"Sleep now, into the eternal nightmare.\"" << endl;

        // 若為大招，先記錄目標的狀態
        Monster* targetMonster = nullptr;
        if (skillNumber == 2 && targetIndex >= 0 && targetIndex < monsters.size()) {
            targetMonster = monsters[targetIndex];
        }

        // 呼叫父類執行技能
        bool success = Player::useSkill(skillNumber, targetIndex, players, monsters);

        // 大招擊殺判定
        if (success && skillNumber == 2 && targetMonster != nullptr) {
            if (!targetMonster->isAlive()) {
                cout << "💀 [Nightmare Reap] Target executed! Skill cooldown reset and MP refunded. You may act again!" << endl;
                // 回復 MP
                this->mp += skillbox[2]->getMpCost();
                if (this->mp > this->maxMp) this->mp = this->maxMp;
                skillbox[2]->setCD(0);
                
                // 重置 CD 邏輯 (需在 BattleManager 的技能冷卻系統配合，這裡先重置 Skill 本身狀態若有)
                // this->triggerFreeAction(); // 示意：這部分通常由 BattleManager 偵測回傳值或狀態來決定
            }
        }
        return success;
    }
}