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
        skillbox[0] = new Skill("嗜血狂擊", SPREAD, NONEH, NONEE, 0,
            DAMAGE, NONE, NONE, NONE, NONE, NONE, NONE, attackPower, 2.5, 0, 0, 0, 4); 
        skillbox[1] = new Skill("反擊巨獸", OWN, NONEH, COUNTERATTACK, 1,
            NONE, STATIC, NONE, NONE, NONE, NONE, NONE, 0, 0, 0, 40, 0, 4);
        skillbox[2] = new Skill("狂獸極刑", AOE, NONEH, NONEE, 0,
            DAMAGE, NONE, NONE, NONE, NONE, NONE, NONE, attackPower, 0, 0, 70, 0, 7);
    }

    Berserker::Berserker(string theName, int theMaxHp, int theMaxMp, int theAttackPower, int theDefense)
    : Player(theName, theMaxHp, theMaxMp, theAttackPower, theDefense), 
      criticalRate(15), criticalEffect(200), passiveHealRatio(0.2)
    {
        job = "Berserker";
        skillbox[0] = new Skill("嗜血狂擊", SPREAD, NONEH, NONEE, 0,
            DAMAGE, NONE, NONE, NONE, NONE, NONE, NONE, attackPower, 2.5, 0, 0, 0, 4); 
        skillbox[1] = new Skill("反擊巨獸", OWN, NONEH, COUNTERATTACK, 1,
            NONE, STATIC, NONE, NONE, NONE, NONE, NONE, 0, 0, 0, 40, 0, 4);
        skillbox[2] = new Skill("狂獸極刑", AOE, NONEH, NONEE, 0,
            DAMAGE, NONE, NONE, NONE, NONE, NONE, NONE, attackPower, 0, 0, 70, 0, 7);
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

    void Berserker::takeDamage(int damage) {
        int oldHp = hp;
        Player::takeDamage(damage);
        int actualDamage = oldHp - hp;

        // 被動技能：根據實際承受傷害的比例回血
        if (isAlive() && actualDamage > 0) {
            int healAmount = round(actualDamage * passiveHealRatio);
            cout << "🩸 [Berserker Passive] Bloodlust triggered! Recovering " << healAmount << " HP." << endl;
            hp += healAmount;
            if (hp > maxHp) hp = maxHp;
        }
    }

    void Berserker::triggerCounterAttack(vector<Monster*> monsters) {
        cout << "🪓 [Counter Behemoth] " << name << " triggers a massive global counterattack!" << endl;
        
        int counterDamage = round(attackPower * 1.2);
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
        StatusEffectList[COUNTERATTACK] = -1;
    }

    bool Berserker::useSkill(int skillNumber, int targetIndex, vector<Player*> players, vector<Monster*> monsters) {
        if (skillNumber < 0 || skillNumber >= 3 || skillbox[skillNumber] == nullptr) return false;

        int mpRequired = skillbox[skillNumber]->getMpCost();
        if (mp < mpRequired) {
            cout << name << " does not have enough MP!" << endl;
            return false;
        }

        if (skillNumber == 0) {
            // 攔截並處理扣血機制
            int hpCost = round(hp * 0.15);
            if (hpCost < 1) hpCost = 1; // 至少扣 1
            if (hp <= hpCost) {
                cout << name << "'s HP is too low to cast [嗜血狂擊]!" << endl;
                return false; // 血量不夠則終止施放
            }
            hp -= hpCost;
            cout << "🩸 " << name << " consumes " << hpCost << " HP to empower the strike! (Remaining HP: " << hp << ")" << endl;
        } 
        else if (skillNumber == 1) {
            cout << "🛡️ [Berserker]: \"Let them come! I'll crush them all!\"" << endl;
        } 
        else if (skillNumber == 2) {
            // SPECIAL 邏輯：動態計算血量比例與傷害
            double hpRatio = (double)hp / maxHp;
            double damageMultiplier = 2.0;

            if (hpRatio <= 0.2) {
                damageMultiplier += 1.5;
                cout << "🔥 [Max Power] HP below 20%! Ultimate execution activated!" << endl;
            } else {
                damageMultiplier += (1.0 - hpRatio) * (1.5 / 0.8);
            }

            int finalAoeDamage = round(attackPower * damageMultiplier);
            cout << " Final damage multiplier: " << round(damageMultiplier * 100) << "%" << endl;

            for (auto enemy : monsters) {
                if (enemy != nullptr && enemy->isAlive()) {
                    cout << " Executing " << enemy->getName() << "!" << endl;
                    enemy->takeDamage(finalAoeDamage);
                }
            }
        }

        // 剩下的統一交給父類處理 (如扣魔、進入 CD、賦予狀態)
        return Player::useSkill(skillNumber, targetIndex, players, monsters);
    }
}