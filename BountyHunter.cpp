#include "BountyHunter.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <vector>

using std::vector;
using std::cout;
using std::endl;

namespace RPG_Colaborate {
    BountyHunter::BountyHunter() : Player(), bountyAtkBonus(0),  redBulletCount(0), blueBulletCount(0), consumedMpForGold(0) {
        job = "Bounty Hunter";
        skillbox[0] = new Skill("Crimson Trigger", OWN, NONEH, NONEE, 0,
            NONE, NONE, NONE, NONE, NONE, NONE, SPECIAL, attackPower, 0, 0, 20, 0, 0);
        skillbox[1] = new Skill("Cobalt Chamber", OWN, NONEH, NONEE, 0,
            NONE, NONE, NONE, NONE, NONE, NONE, SPECIAL, attackPower, 0, 0, 10, 0, 0);
        skillbox[2] = new Skill("Midas Shell", OWN, NONEH, GOLDBULLET, 2,
            NONE, STATIC, NONE, NONE, NONE, NONE, SPECIAL, attackPower, 0, 0, 0, 0, 4);
    }

    BountyHunter::BountyHunter(string theName, int theMaxHp, int theMaxMp, int theAttackPower, int theDefense)
    : Player(theName, theMaxHp, theMaxMp, theAttackPower, theDefense), bountyAtkBonus(0), redBulletCount(0), blueBulletCount(0), consumedMpForGold(0) {
        job = "Bounty Hunter";
        skillbox[0] = new Skill("Crimson Trigger", OWN, NONEH, NONEE, 0,
            NONE, NONE, NONE, NONE, NONE, NONE, SPECIAL, attackPower, 0, 0, 20, 0, 0);
        skillbox[1] = new Skill("Cobalt Chamber", OWN, NONEH, NONEE, 0,
            NONE, NONE, NONE, NONE, NONE, NONE, SPECIAL, attackPower, 0, 0, 10, 0, 0);
        skillbox[2] = new Skill("Midas Shell", OWN, NONEH, GOLDBULLET, 2,
            NONE, STATIC, NONE, NONE, NONE, NONE, SPECIAL, attackPower, 0, 0, 0, 0, 4);
    }

    void BountyHunter::addBountyGold(int gold) {
        ownedGolds += gold;
        if (ownedGolds > 0) {
            bountyAtkBonus = ownedGolds * 10;
            if (ownedGolds >= 300) {
                bountyAtkBonus = 3000;
            }
        }
        else {
            bountyAtkBonus = 0;
        }
    }

    int BountyHunter::getAttackPower() const
    {
        int finalAtk = attackPower + bountyAtkBonus;

        double strengthRate = 1;
        // 🎯 核心攔截：如果身上有牧師的增傷 Buff
        if (getEffectTurns(STRENGTH) > 0) {
            strengthRate += 0.3;
        }
        if (getEffectTurns(FLOOR_STRENGTH) > 0) {
            strengthRate += 0.5;
        }

        return static_cast<int>(finalAtk * strengthRate);
    }

    // 覆寫普攻：霰彈槍 5 段傷害
    void BountyHunter::attack(int targetIndex, vector<Monster*>& monsters, vector<Player*>& players) {
        cout << name << " pulls out the Shotgun and fires a 5-round burst!" << endl;

        int multiplier = 1;
        if (getEffectTurns(LAST_GASP) > 0) {
            multiplier = 4;
            takeEffect(LAST_GASP, 0); // ⚡ The moment the attack is unleashed, the status is immediately cleared!
            cout << "🩸 The Last Gasp effect has been released with the attack, status removed.\n";
        }

        for (int i = 0; i < 5; i++) {
            // 索敵邏輯：前三段打主目標，後兩段打隨機目標
            int currentTargetIdx = targetIndex;
            if (i >= 3) {
                vector<int> aliveIndices;
                for (int j = 0; j < monsters.size(); j++) {
                    if (monsters[j] != nullptr && monsters[j]->isAlive()) {
                        aliveIndices.push_back(j);
                    }
                }
                if (aliveIndices.empty()) break;
                currentTargetIdx = aliveIndices[rand() % aliveIndices.size()];
            }

            Monster* target = monsters[currentTargetIdx];
            if (!target->isAlive()) continue;

            // 子彈種類判定與傷害計算
            int baseShotDamage = round(getAttackPower() * 0.3); // 假設每發子彈基礎為30%攻擊力
            int finalDamage = baseShotDamage;

            if (StatusEffectList[GOLDBULLET] >= 0) {
                cout << "  🟡 [Golden Bullet] ";
                // 金色子彈：根據當初消耗的魔力增加傷害
                finalDamage += round(consumedMpForGold * 3.5);
                heal(round(maxHp * 0.02)); // 回復微量生命值 (例如 2% 最大生命)
            }
            else if (redBulletCount > 0) {
                cout << "  🔴 [Red Bullet] ";
                finalDamage = round(baseShotDamage * 2.5); // 大量額外傷害
                redBulletCount--;
            }
            else if (blueBulletCount > 0) {
                cout << "  🔵 [Blue Bullet] ";
                finalDamage = round(baseShotDamage * 1.5); // 少量額外傷害
                mp += 20; // 回魔
                if (mp > maxMp) mp = maxMp;
                blueBulletCount--;
            }
            else {
                cout << "  ⚪ [Normal Bullet] ";
            }
            finalDamage *= multiplier;

            cout << "hits " << target->getName() << "!" << endl;
            target->takeDamage(finalDamage);
        }
    }

    void BountyHunter::takeDamage(int damage, vector<Monster*>& monsters)
    {
        if (StatusEffectList[GOLDBULLET] >= 0) {
            cout << "✨ [Bounty Hunter]: \"Can't touch the merchandise! Solid gold armor, baby!\"" << endl;
            Player::takeDamage(round(0.5 * damage), monsters);
        }
        else {
            Player::takeDamage(damage, monsters);
        }
    }

    // 覆寫技能：處理金色子彈的「沉默」效果
    bool BountyHunter::useSkill(int skillInput, int targetIndex, vector<Player*>& players, vector<Monster*>& monsters)
    {
        int skillNumber = skillInput - 1;
        if (skillNumber < 0 || skillNumber >= 3 || skillbox[skillNumber] == nullptr) {
            cout << "The skill does not exist." << endl;
            return false;
        }

        if (StatusEffectList[GOLDBULLET] >= 0) {
            cout << "⚠️ " << name << " is currently locking loaded with Golden Bullets! Skills are disabled!" << endl;
            return false;
        }

        int mpRequired = skillbox[skillNumber]->getMpCost();
        if (mp < mpRequired) {
            cout << name << " does not have enough MP!" << endl;
            return false;
        }

        // 提前播報技能台詞
        if (skillNumber == 0) {
            cout << "🔴 [Bounty Hunter]: \"This one's got your name on it! Extra gunpowder, on the house!\"" << endl;
        } else if (skillNumber == 1) {
            cout << "🔵 [Bounty Hunter]: \"Refilling my focus and your wounds! Business is blooming!\"" << endl;
        } else if (skillNumber == 2) {
            cout << "💰 [Bounty Hunter]: \"Oh baby! Time for the jackpot! Smell the gold! HAHAHA!\"" << endl;
        }

        return Player::useSkill(skillInput, targetIndex, players, monsters);
    }

    // 特殊判定
    void BountyHunter::triggerClassSpecial(Skill& theSkill, int targetIndex, vector<Monster*>& monsters, vector<Player*>& players) {
        if (&theSkill == skillbox[0]) {
            redBulletCount++;
            cout << "🔫 Loaded a Red Bullet! Action refunded!" << endl;
            takeEffect(FREEACTION, 1);
        }
        else if (&theSkill == skillbox[1]) {
            blueBulletCount++;
            cout << "🔫 Loaded a Blue Bullet! Action refunded!" << endl;
            takeEffect(FREEACTION, 1);
        }
        else if (&theSkill == skillbox[2]) {
            // 金色子彈：動態消耗全部魔力
            consumedMpForGold = mp; 
            mp = 0;
            cout << "✨ Consumed " << consumedMpForGold << " MP to enter Golden State! Action refunded!" << endl;
            // 獲得免傷的部分，你可以在 Player::takeDamage() 裡面攔截 GOLDEN_STATE 來減傷 50%
            takeEffect(FREEACTION, 1);
        }
    }
}