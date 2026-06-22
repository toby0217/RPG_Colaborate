#include "Priest.h"
#include "Skill.h"
#include "Monster.h"
#include <iostream>
#include <vector>

using std::vector;
using std::cout;
using std::endl;

using std::cout;
using std::endl;

namespace RPG_Colaborate {

    Priest::Priest() : Player(), canRevive(true) {
        job = "Priest";
        skillbox[0] = new Skill("Pillar of Retribution", SINGLE, OWNH, NONEE, 0,
            DAMAGE, NONE, NONE, NONE, HEAL, NONE, SPECIAL, attackPower, 2.25, 15, 30, 0, 2);
        skillbox[1] = new Skill("Grace of Healing", TEAM, TEAMH, STRENGTH, 2,
            NONE, NONE, BUFF, NONE, HEAL, NONE, SPECIAL, attackPower, 0, 40, 50, 0, 4);                    
        skillbox[2] = new Skill("Divine Revival", SINGLE, NONEH, NONEE, 0,
            NONE, NONE, NONE, NONE, NONE, REVIVE, NONE, attackPower, 0, 0, 90, 0, 0);
    }

    Priest::Priest(string theName, int theMaxHp, int theMaxMp, int theAttackPower, int theDefense)
        : Player(theName, theMaxHp, theMaxMp, theAttackPower, theDefense), canRevive(true) { 
        job = "Priest";
        skillbox[0] = new Skill("Pillar of Retribution", SINGLE, OWNH, NONEE, 0,
            DAMAGE, NONE, NONE, NONE, HEAL, NONE, SPECIAL, attackPower, 2.25, 15, 30, 0, 2);
        skillbox[1] = new Skill("Grace of Healing", TEAM, TEAMH, STRENGTH, 2,
            NONE, NONE, BUFF, NONE, HEAL, NONE, SPECIAL, attackPower, 0, 40, 50, 0, 4);                    
        skillbox[2] = new Skill("Divine Revival", SINGLE, NONEH, NONEE, 0,
            NONE, NONE, NONE, NONE, NONE, REVIVE, NONE, attackPower, 0, 0, 90, 0, 0);
    }

    Priest::~Priest() {
        for (int i = 0; i < 3; i++) {
            if (skillbox[i] != nullptr) {
                delete skillbox[i];
                skillbox[i] = nullptr;
            }
        }
    }

    void Priest::triggerPassiveHeal(vector<Player*>& teammates) {
        if (teammates.empty()) return;
        
        Player* lowestTarget = nullptr;
        double lowestRatio = 2.0; // 初始設大於 1

        for (Player* p : teammates) {
            if (p != nullptr && p->isAlive()) {
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
            cout << "🕊️ [Priest Passive] Holy light embraces " << lowestTarget->getName() 
                 << ", restoring " << healAmount << " HP (15%)!" << endl;
        }
    }

    void Priest::attack(int targetIndex, vector<Monster*>& monsters, vector<Player*>& players) {
        // 呼叫父類普攻
        Player::attack(targetIndex, monsters, players);
        // 普攻後觸發被動回血
        triggerPassiveHeal(players);
    }

    bool Priest::useSkill(int skillInput, int targetIndex, vector<Player*>& players, vector<Monster*>& monsters)
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
            cout << "☀️ [Priest]: \"Allow me to clear the path! May the light guide our way!\"" << endl;
        } else if (skillNumber == 1) {
            cout << "💚 [Priest]: \"Hold on, I am right here with you. Feel the warmth of restoration!\"" << endl;
        } else if (skillNumber == 2) {
            if (!canRevive) {
                cout << "❌ [Divine Revival] This skill can only be used once per battle!" << endl;
                return false;
            }
            // 只要施放成功，標記為不可用
            canRevive = false;
            cout << "🕊️ [Priest]: \"Death shall not claim you today. Stand up, my friends!\"" << endl;
        }

        return Player::useSkill(skillInput, targetIndex, players, monsters);
    }

    void Priest::triggerClassSpecial(Skill& theSkill, int targetIndex, vector<Monster*>& monsters, vector<Player*>& players)
    {
        if (&theSkill == skillbox[0] || &theSkill == skillbox[1]) {
            triggerPassiveHeal(players);
        }
    }
}