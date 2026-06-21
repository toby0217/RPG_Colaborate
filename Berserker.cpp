#include "Berserker.h"
#include <iostream>
#include <cmath>

using std::cout;
using std::endl;

namespace RPG_Colaborate {
    Berserker::Berserker()
    : Player(), criticalRate(15), criticalEffect(200), passiveHealRatio(0.2)
    {
        job = "Berserker";
        skillbox[0] = new Skill("Carnage Strike", SPREAD, NONEH, NONEE, 0,
            DAMAGE, NONE, NONE, NONE, NONE, NONE, NONE, attackPower, 1.8, 0, 0, 15, 3); 
        skillbox[1] = new Skill("Behemoth Counter", OWN, NONEH, COUNTERATTACK, 1,
            NONE, STATIC, NONE, NONE, NONE, NONE, NONE, 0, 0, 0, 40, 0, 4);
        skillbox[2] = new Skill("Feral Execution", AOE, NONEH, NONEE, 0,
            NONE, NONE, NONE, NONE, NONE, NONE, SPECIAL, attackPower, 2.0, 0, 70, 0, 7);
    }

    Berserker::Berserker(string theName, int theMaxHp, int theMaxMp, int theAttackPower, int theDefense)
    : Player(theName, theMaxHp, theMaxMp, theAttackPower, theDefense), 
      criticalRate(15), criticalEffect(200), passiveHealRatio(0.2)
    {
        job = "Berserker";
        skillbox[0] = new Skill("Carnage Strike", SPREAD, NONEH, NONEE, 0,
            DAMAGE, NONE, NONE, NONE, NONE, NONE, NONE, attackPower, 1.8, 0, 0, 15, 4); 
        skillbox[1] = new Skill("Behemoth Counter", OWN, NONEH, COUNTERATTACK, 1,
            NONE, STATIC, NONE, NONE, NONE, NONE, NONE, 0, 0, 0, 40, 0, 4);
        skillbox[2] = new Skill("Feral Execution", AOE, NONEH, NONEE, 0,
            NONE, NONE, NONE, NONE, NONE, NONE, SPECIAL, attackPower, 2.0, 0, 70, 0, 7);
    }

    Berserker::~Berserker() {
        for (int i = 0; i < 3; i++) {
            if (skillbox[i] != nullptr) {
                delete skillbox[i];
                skillbox[i] = nullptr;
            }
        }
    }

    int Berserker::getCriticalRate() const { return criticalRate; }
    int Berserker::getCriticalEffect() const { return criticalEffect; }
    void Berserker::setCriticalRate(int newRate) { criticalRate = newRate; }
    void Berserker::setCriticalEffect(int newEffect) { criticalEffect = newEffect; }

    void Berserker::takeDamage(int damage, vector<Monster*>& monsters) {
        int oldHp = hp;
        Player::takeDamage(damage, monsters);
        int actualDamage = oldHp - hp;

        // 被動技能：根據實際承受傷害的比例回血
        if (isAlive() && actualDamage > 0) {
            int healAmount = round(actualDamage * passiveHealRatio);
            cout << "🩸 [Berserker Passive] Bloodlust triggered! Recovering " << healAmount << " HP." << endl;
            hp += healAmount;
            if (hp > maxHp) hp = maxHp;

            if (getEffectTurns(COUNTERATTACK) > 0) {
                triggerCounterAttack(monsters);
            }
        }
    }

    void Berserker::triggerCounterAttack(vector<Monster*> monsters) {
        cout << "🪓 [Berserker]: \"You dare strike ME?! I'll feed your flesh to the crows!\"" << endl;
        cout << "🪓 [Behemoth Counter] " << name << " triggers a massive global counterattack!" << endl;
        
        int counterDamage = round(getAttackPower() * 1.2);
        for (auto enemy : monsters) {
            if (enemy != nullptr && enemy->isAlive()) {
                cout << " Countering " << enemy->getName() << "!" << endl;
                enemy->takeDamage(counterDamage);
            }
        }

        int skillHeal = round(maxHp * 0.25);
        cout << "🩸 [Counter Behemoth] Recovers " << skillHeal << " HP from the onslaught!" << endl;
        hp += skillHeal;
        if (hp > maxHp) hp = maxHp;

        // 反擊結算後直接拔除狀態，避免重複觸發
        StatusEffectList[COUNTERATTACK] = 0;
    }

    bool Berserker::useSkill(int skillInput, int targetIndex, vector<Player*>& players, vector<Monster*>& monsters)
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
            cout << "🩸 [Berserker]: \"MORE BLOOD! Rip! Tear! DESTROY!!!\"" << endl;
        } else if (skillNumber == 1) {
            cout << "🪓 [Berserker]: \"You dare strike ME?! I'll feed your flesh to the crows!\"" << endl;
        } else if (skillNumber == 2) {
            cout << "👹 [Berserker]: \"GET OUT OF MY WAY! DIE! DIE! DIEEEEE!!!\"" << endl;
        }

        return Player::useSkill(skillInput, targetIndex, players, monsters);
    }

    void Berserker::triggerClassSpecial(Skill& theSkill, int targetIndex, vector<Monster*>& monsters, vector<Player*>& players) {
        if (&theSkill == skillbox[2]) {
            // SPECIAL 邏輯：動態計算血量比例與傷害
            double hpRatio = (double)hp / maxHp;
            double damageMultiplier = 2.0;

            if (hpRatio <= 0.2) {
                damageMultiplier += 1.5;
                cout << "🔥 [Max Power] HP below 20%! Ultimate execution activated!" << endl;
            } else {
                damageMultiplier += (1.0 - hpRatio) * (1.5 / 0.8);
            }

            int finalAoeDamage = round(getAttackPower() * damageMultiplier);
            cout << "Final damage multiplier: " << round(damageMultiplier * 100) << "%" << endl;

            for (auto enemy : monsters) {
                if (enemy != nullptr && enemy->isAlive()) {
                    cout << "Executing " << enemy->getName() << "!" << endl;
                    enemy->takeDamage(finalAoeDamage);
                }
            }
        }
    }
}