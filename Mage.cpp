#include "Mage.h"
#include <iostream>
#include <cmath>
#include <cstdlib>

using std::cout;
using std::endl;

namespace RPG_Colaborate {
    // Constructors
    Mage::Mage() : Player() {
        job = "Mage";
        skillbox[0] = new Skill("烈焰迸發", SPREAD, NONEH, BURN, 3,
            DAMAGE, NONE, NONE, DEBUFF, NONE, NONE, SPECIAL, attackPower, 1.2, 0, 30, 0, 4);
        skillbox[1] = new Skill("冰霜新星", SPREAD, NONEH, FREEZE, 3,
            DAMAGE, NONE, NONE, DEBUFF, NONE, NONE, SPECIAL, attackPower, 1.2, 0, 30, 0, 4);
        skillbox[2] = new Skill("極黑之隕", AOE, NONEH, NONEE, 0,
            DAMAGE, NONE, NONE, NONE, NONE, NONE, SPECIAL, attackPower, 2.5, 0, 70, 0, 6);
    }

    Mage::Mage(string theName, int theMaxHp, int theMaxMp, int theAttackPower, int theDefense)
    : Player(theName, theMaxHp, theMaxMp, theAttackPower, theDefense) {
        job = "Mage";
        skillbox[0] = new Skill("烈焰迸發", SPREAD, NONEH, BURN, 2,
            DAMAGE, NONE, NONE, DEBUFF, NONE, NONE, NONE, attackPower, 1.2, 0, 30, 0, 4);
        skillbox[1] = new Skill("冰霜新星", SPREAD, NONEH, FREEZE, 2,
            DAMAGE, NONE, NONE, DEBUFF, NONE, NONE, NONE, attackPower, 1.2, 0, 30, 0, 4);
        skillbox[2] = new Skill("極黑之隕", AOE, NONEH, NONEE, 0,
            DAMAGE, NONE, NONE, NONE, NONE, NONE, NONE, attackPower, 2.5, 0, 70, 0, 6);
    }

    Mage::~Mage() {
        for (int i = 0; i < 3; i++) {
            if (skillbox[i] != nullptr) {
                delete skillbox[i];
                skillbox[i] = nullptr;
            }
        }
    }

    // 覆寫普攻：加入擴散傷害邏輯
    void Mage::attack(int targetIndex, vector<Monster*>& monsters, vector<Player*>& players) {
        Player::attack(targetIndex, monsters, players);

        // 搜尋左側存活目標
        int leftTargetIndex = targetIndex - 1;
        while (leftTargetIndex >= 0 && (monsters[leftTargetIndex] == nullptr || !monsters[leftTargetIndex]->isAlive())) {
            leftTargetIndex--;
        }
        if (leftTargetIndex >= 0) {
            cout << "Magic splashes onto " << monsters[leftTargetIndex]->getName() << "!" << endl;
            monsters[leftTargetIndex]->takeDamage(round(0.5 * attackPower));
        }

        // 搜尋右側存活目標
        int rightTargetIndex = targetIndex + 1;
        while (rightTargetIndex < monsters.size() && (monsters[rightTargetIndex] == nullptr || !monsters[rightTargetIndex]->isAlive())) {
            rightTargetIndex++;
        }
        if (rightTargetIndex < monsters.size()) {
            cout << "Magic splashes onto " << monsters[rightTargetIndex]->getName() << "!" << endl;
            monsters[rightTargetIndex]->takeDamage(round(0.5 * attackPower));
        }
    }

    // 覆寫技能：處理技能條件與額外機率
    bool Mage::useSkill(int skillNumber, int targetIndex, vector<Player*>& players, vector<Monster*>& monsters) {
        if (skillNumber < 0 || skillNumber >= 3 || skillbox[skillNumber] == nullptr) return false;

        int mpRequired = skillbox[skillNumber]->getMpCost();
        if (mp < mpRequired) {
            cout << name << " does not have enough MP!" << endl;
            return false;
        }

        // 提前播報技能台詞
        if (skillNumber == 0) {
            cout << "🔥 [Mage]: \"Incinerate! 烈焰迸發!\"" << endl;
        } else if (skillNumber == 1) {
            cout << "❄️ [Mage]: \"Shatter! 冰霜新星!\"" << endl;
        } else if (skillNumber == 2) {
            cout << "🌌 [Mage]: \"Descend into absolute darkness... 極黑之隕!\"" << endl;
        }

        return Player::useSkill(skillNumber, targetIndex, players, monsters);
    }
    
    // 特殊判定: 元素引爆檢測
    void Mage::triggerClassSpecial(Skill& theSkill, int targetIndex, vector<Monster*>& monsters, vector<Player*>& players) {
        if (&theSkill == skillbox[1]) {
            // SPREAD 範圍包含目標與左右各一
            int leftIndex = targetIndex - 1;
            while (leftIndex >= 0 && (monsters[leftIndex] == nullptr || !monsters[leftIndex]->isAlive())) leftIndex--;
                
            int rightIndex = targetIndex + 1;
            while (rightIndex < monsters.size() && (monsters[rightIndex] == nullptr || !monsters[rightIndex]->isAlive())) rightIndex++;

            int targetsToRoll[3] = { targetIndex, leftIndex, rightIndex };

            for (int i = 0; i < 3; i++) {
                int idx = targetsToRoll[i];
                if (idx >= 0 && idx < monsters.size() && monsters[idx] != nullptr && monsters[idx]->isAlive()) {
                    if (rand() % 100 < 25) { // 25% 機率觸發無法行動 (STUN)
                        cout << " 🧊 " << monsters[idx]->getName() << " is frozen solid and cannot act!" << endl;
                        // 這裡假設 STUN 也是 EffectType 的一環，並強制覆寫 1 回合
                        monsters[idx]->takeEffect(FREEZE, 1);
                    }
                }
            }
        }
        for (auto m : monsters) {
            // 假設怪物的狀態陣列允許直接存取 (與你 Ranger 中的設計一致)
            if (m != nullptr && m->isAlive()) {
                if (&theSkill == skillbox[2]) {
                    if (m->getEffects(BURN) >= 0 || m->getEffects(COLD) >= 0) {
                        cout << "💥 [Elemental Detonation] Thermal shock occurs on " << m->getName() << "!" << endl;
                        
                        // 造成額外傷害 (依你平衡需求可調整倍率，此處預設為攻擊力 1.5 倍)
                        int bonusDamage = round(attackPower * 1.5);
                        m->takeDamage(bonusDamage);

                        // 清空燃燒與冰霜狀態
                        m->takeEffect(BURN, 0);
                        m->takeEffect(COLD, 0);
                    }
                    return;
                }
                if (m->getEffects(BURN) >= 0 && m->getEffects(COLD) >= 0) {
                    cout << "💥 [Elemental Detonation] Thermal shock occurs on " << m->getName() << "!" << endl;
                    
                    // 造成額外傷害 (依你平衡需求可調整倍率，此處預設為攻擊力 1.5 倍)
                    int bonusDamage = round(attackPower * 1.5);
                    m->takeDamage(bonusDamage);

                    // 清空燃燒與冰霜狀態
                    m->takeEffect(BURN, 0);
                    m->takeEffect(COLD, 0);
                }
            }
        }
    }
}