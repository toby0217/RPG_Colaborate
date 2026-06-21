#include "Necromancer.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <vector>

using std::vector;
using std::cout;
using std::endl;

namespace RPG_Colaborate {
    // Constructors
    Necromancer::Necromancer() : Player(), undeadCount(0)
    {
        job = "Necromancer";
        // 技能1: 亡靈召喚 (無傷害，純召喚，自身狀態)
        skillbox[0] = new Skill("Raise Specter", OWN, NONEH, NONEE, 0,
            NONE, NONE, NONE, NONE, NONE, NONE, SPECIAL, attackPower, 0, 0, 30, 0, 3);
        // 技能2: 靈魂割者 (單體傷害，降攻，具備斬殺機制)
        skillbox[1] = new Skill("Soul Reaper", SINGLE, NONEH, WEAKEN, 2,
            DAMAGE, NONE, NONE, DEBUFF, NONE, NONE, SPECIAL, attackPower, 1.2, 0, 30, 0, 4);
        // 技能3: 亡者晚宴 (賦予自身強化狀態)
        skillbox[2] = new Skill("Macabre Feast", OWN, NONEH, FEAST, 3,
            NONE, STATIC, NONE, NONE, NONE, NONE, SPECIAL, attackPower, 0, 0, 60, 0, 5);
    }

    Necromancer::Necromancer(string theName, int theMaxHp, int theMaxMp, int theAttackPower, int theDefense)
    : Player(theName, theMaxHp, theMaxMp, theAttackPower, theDefense), undeadCount(0)
    {
        job = "Necromancer";
        skillbox[0] = new Skill("亡靈召喚", OWN, NONEH, NONEE, 0,
            NONE, NONE, NONE, NONE, NONE, NONE, SPECIAL, attackPower, 0, 0, 30, 0, 3);
        skillbox[1] = new Skill("靈魂割者", SINGLE, NONEH, WEAKEN, 2,
            DAMAGE, NONE, NONE, DEBUFF, NONE, NONE, SPECIAL, attackPower, 1.2, 0, 30, 0, 4);
        skillbox[2] = new Skill("亡者晚宴", OWN, NONEH, FEAST, 3,
            NONE, STATIC, NONE, NONE, NONE, NONE, NONE, attackPower, 0, 0, 60, 0, 5);
    }

    // 亡靈追擊副程式
    void Necromancer::triggerUndead(vector<Monster*>& monsters) {
        if (undeadCount <= 0) return;

        // 判斷是否有「亡者晚宴」強化狀態 (假設原本傷害為攻擊力20%，強化後變40%)
        double multiplier = (StatusEffectList[FEAST] >= 0) ? 0.4 : 0.2;
        int undeadDamage = round(attackPower * multiplier);

        cout << "💀 The spirits awaken! " << undeadCount << " Undead(s) launch their pursuit!" << endl;

        // 每個亡靈進行一次隨機攻擊
        for (int i = 0; i < undeadCount; i++) {
            vector<Monster*> aliveMonsters;
            for (auto m : monsters) {
                if (m != nullptr && m->isAlive()) aliveMonsters.push_back(m);
            }
            if (aliveMonsters.empty()) break; // 沒怪就停止

            Monster* target = aliveMonsters[rand() % aliveMonsters.size()];
            cout << "  -> A spirit strikes " << target->getName() << " used " << undeadDamage << " units of strength!" << endl;
            target->takeDamage(undeadDamage);
        }
    }

    // 覆寫普攻
    void Necromancer::attack(int targetIndex, vector<Monster*>& monsters, vector<Player*>& players) {
        Player::attack(targetIndex, monsters, players); // 執行基礎普攻

        // 被動: 50% 機率召喚亡靈
        if (rand() % 100 < 50) {
            undeadCount++;
            cout << "🧟 [Passive] A new Undead is raised from the shadows! (Current: " << undeadCount << ")" << endl;
        }

        // 普攻後觸發亡靈追擊
        triggerUndead(monsters);
    }

    // 覆寫使用技能 (為了在技能施放後觸發亡靈追擊)
    bool Necromancer::useSkill(int skillNumber, int targetIndex, vector<Player*>& players, vector<Monster*>& monsters) {
        if (skillNumber < 0 || skillNumber >= 3 || skillbox[skillNumber] == nullptr) return false;

        int mpRequired = skillbox[skillNumber]->getMpCost();
        if (mp < mpRequired) {
            cout << name << " does not have enough MP!" << endl;
            return false;
        }

        // 提前播報技能台詞
        if (skillNumber == 0) {
            cout << "💀 [Necromancer]: \"Come out and play, little ones. Let's show them a good time, shall we?\"" << endl;
        } else if (skillNumber == 1) {
            cout << "✂️ [Necromancer]: \"Oh, what a lovely soul you have... Mind if I borrow it for a bit?\"" << endl;
        } else if (skillNumber == 2) {
            cout << "🍷 [Necromancer]: \"Eat up, my darling spirits. Look how entertaining our friends are being!\"" << endl;
        }
        
        return Player::useSkill(skillNumber, targetIndex, players, monsters);
    }

    // 特殊判定
    void Necromancer::triggerClassSpecial(Skill& theSkill, int targetIndex, vector<Monster*>& monsters, vector<Player*>& players) {
        if (&theSkill == skillbox[0]) {
            // 技能1: 召喚兩個亡靈
            undeadCount += 2;
            cout << "🧟 Summoned 2 Undeads! (Current: " << undeadCount << ")" << endl;
        }
        else if (&theSkill == skillbox[1]) {
            // 技能2: 斬殺判定與召喚
            Monster* target = monsters[targetIndex];
            if (target != nullptr && target->isAlive()) {
                bool isElite = (target->getRank() == ELITE || target->getRank() == BOSS);
                double hpThreshold = isElite ? 0.05 : 0.20;
                double currentHpRate = 1.0 * target->getHp() / target->getMaxHp();

                if (currentHpRate <= hpThreshold) {
                    cout << "⚔️ [Execution] " << target->getName() << "'s soul is utterly severed!" << endl;
                    target->takeDamage(target->getHp()); // 滿血斬殺
                    undeadCount += 3;
                    cout << "🧟 Harvested 3 Undeads from the execution! (Current: " << undeadCount << ")" << endl;
                } else {
                    undeadCount += 1;
                    cout << "🧟 Harvested 1 Undead from the target! (Current: " << undeadCount << ")" << endl;
                }
            }
        }
        triggerUndead(monsters);
    }
}