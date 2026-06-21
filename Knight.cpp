#include "Knight.h"
#include <iostream>
#include <cmath>
using std::cout;
using std::endl;

namespace RPG_Colaborate {
    // Constructors
    Knight::Knight()
    : Player(), criticalRate(15), criticalEffect(200)
    {
        job = "Knight";
        skillbox[0] = new Skill("Shield Charge", SPREAD, NONEH, LOWERDEFENSE, 2,
            DAMAGE, NONE, NONE, DEBUFF, NONE, NONE, NONE, attackPower, 1.2, 0, 30, 0, 3);
        skillbox[1] = new Skill("Knight Aura", AOE, NONEH, TAUNT, 1,
            NONE, STATIC, NONE, NONE, NONE, NONE, NONE, 0, 0, 0, 50, 0, 5);
        skillbox[2] = new Skill("The Indomitable Will", OWN, NONEH, PERSEVERANCE, 2,
            NONE, STATIC, NONE, NONE, NONE, NONE, NONE, 0, 0, 0, 90, 0, 8);
    }
    Knight::Knight(string theName, int theMaxHp, int theMaxMp, int theAttackPower, int theDefense)
    : Player(theName, theMaxHp, theMaxMp, theAttackPower, theDefense), 
      criticalRate(15), criticalEffect(200)
    {
        job = "Knight";
        skillbox[0] = new Skill("Shield Charge", SPREAD, NONEH, LOWERDEFENSE, 2,
            DAMAGE, NONE, NONE, DEBUFF, NONE, NONE, NONE, attackPower, 1.2, 0, 30, 0, 3);
        skillbox[1] = new Skill("Knight Aura", AOE, NONEH, TAUNT, 1,
            NONE, NONE, NONE, DEBUFF, NONE, NONE, NONE, 0, 0, 0, 50, 0, 5);
        skillbox[2] = new Skill("The Indomitable Will", OWN, NONEH, PERSEVERANCE, 2,
            NONE, STATIC, NONE, NONE, NONE, NONE, NONE, 0, 0, 0, 90, 0, 8);
    }

    Knight::~Knight()
    {
        for (int i = 0; i < 3; i++) {
            if (skillbox[i] != nullptr) {
                delete skillbox[i];
                skillbox[i] = nullptr;
            }
        }
    }

    // getters
    int Knight::getCriticalRate() const { return criticalRate; }
    int Knight::getCriticalEffect() const { return criticalEffect; }

    // setters
    void Knight::setCriticalRate(int newRate) { criticalRate = newRate; }
    void Knight::setCriticalEffect(int newEffect) { criticalEffect = newEffect;}

    void Knight::takeDamage(int damage, vector<Monster*>& monsters) {
        // Knight's Passive: Damage reduction triggers when HP is below 50%
        if (hp < (maxHp / 2)) {
            cout << "🛡️ [Knight Passive] HP below 50%! Guard activated." << endl;
            // Reduce incoming damage by 30% (Takes only 70% of the damage)
            Player::takeDamage(round(0.7 * damage), monsters);
        }
        else {
            // 🛠️ 補上這段，不然騎士滿血時會無敵！
            Player::takeDamage(damage, monsters);
        }
    }

    bool Knight::useSkill(int skillNumber, int targetIndex, vector<Player*>& players, vector<Monster*>& monsters)
    {
        if (skillNumber < 0 || skillNumber >= 3 || skillbox[skillNumber] == nullptr) return false;

        int mpRequired = skillbox[skillNumber]->getMpCost();
        if (mp < mpRequired) {
            cout << name << " does not have enough MP!" << endl;
            return false;
        }

        // 提前播報技能台詞
        if (skillNumber == 0) {
            cout << "🛡️ [Knight]: \"Break their ranks! Shield Charge!\"" << endl;
        } else if (skillNumber == 1) {
            cout << "🛡️ [Knight]: \"Come at me! I am your impenetrable wall!\"" << endl;
        } else if (skillNumber == 2) {
            cout << "🛡️ [Knight]: \"My will is forged in iron... I shall not fall!\"" << endl;
        }

        // 呼叫父類的共通邏輯 (扣魔、扣血、呼叫 Skill::use)
        return Player::useSkill(skillNumber, targetIndex, players, monsters);
    }
}