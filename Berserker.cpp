#include "Berserker.h"
#include <iostream>
#include <cmath>

using std::cout;
using std::endl;

namespace RPG_Colaborate {
    // Constructors
    Berserker::Berserker()
    : Player(), criticalRate(10), criticalEffect(200), 
      passiveHealRatio(0.25), isCounterActive(false), activeEnemyLine(nullptr)
    {
        job = "Berserker";
        // 使用 new 配置記憶體，設定技能基本資訊 (數值會在執行時依機制加權)
        skillbox[0] = new Skill("嗜血狂擊", "Damage", attackPower * 2, 0); // 消耗血不耗魔
        skillbox[1] = new Skill("反擊巨獸", "Buff", 0, 40);
        skillbox[2] = new Skill("狂獸極刑", "Damage", attackPower * 3, 60);

        // 初始化 CD
        for(int i = 0; i < 3; ++i) currentCD[i] = 0;
        maxCD[0] = 4; // 嗜血狂擊 CD 4 回合
        maxCD[1] = 4; // 反擊巨獸 CD 4 回合
        maxCD[2] = 7; // 狂獸極刑 CD 7 回合
    }

    Berserker::Berserker(string theName, int theMaxHp, int theMaxMp, int theAttackPower, int theDefense)
    : Player(theName, theMaxHp, theMaxMp, theAttackPower, theDefense), 
      criticalRate(10), criticalEffect(200), 
      passiveHealRatio(0.25), isCounterActive(false), activeEnemyLine(nullptr)
    {
        job = "Berserker";
        skillbox[0] = new Skill("嗜血狂擊", "Damage", attackPower * 2, 0);
        skillbox[1] = new Skill("反擊巨獸", "Buff", 0, 40);
        skillbox[2] = new Skill("狂獸極刑", "Damage", attackPower * 3, 60);

        for(int i = 0; i < 3; ++i) currentCD[i] = 0;
        maxCD[0] = 4;
        maxCD[1] = 4;
        maxCD[2] = 7;
    }

    // Getters & Setters
    int Berserker::getCriticalRate() const { return criticalRate; }
    int Berserker::getCriticalEffect() const { return criticalEffect; }
    bool Berserker::getIsCounterActive() const { return isCounterActive; }
    void Berserker::setCriticalRate(int newRate) { criticalRate = newRate; }
    void Berserker::setCriticalEffect(int newEffect) { criticalEffect = newEffect; }

    // 覆寫受傷邏輯 (核心機制：被動回血 + 受到傷害立即反擊)
    void Berserker::takeDamage(int damage) {
        // 修正原代碼的整數除法漏洞，轉型為 double 計算防禦減傷
        if (defense > 0) {
            damage = round(damage * (1 - ( (double)defense / (defense + 100) )));
        }

        hp -= damage;
        if (hp < 0) hp = 0;

        cout << " " << name << " takes " << damage << " points of damage! " 
             << "(Current HP: " << hp << "/" << maxHp << ")" << endl;

        // --- 被動技能：受到任何傷害後回復該傷害固定比例的生命值 ---
        if (isAlive() && damage > 0) {
            int healAmount = round(damage * passiveHealRatio);
            cout << " [Berserker Passive] Bloodlust triggered! Recovering from pain." << endl;
            heal(healAmount); 
        }

        // --- 主動技能觸發：反擊巨獸 ---
        if (isAlive() && isCounterActive && activeEnemyLine != nullptr) {
            cout << " [Counter Behemoth] Triggers a massive global counterattack!" << endl;
            
            // 1. 對敵方全體進行反擊 (造成 120% 攻擊力的傷害)
            int counterDamage = round(attackPower * 1.2);
            for (auto enemy : *activeEnemyLine) {
                if (enemy != nullptr && enemy->isAlive()) {
                    cout << " Countering " << enemy->getName() << "!" << endl;
                    enemy->takeDamage(counterDamage);
                }
            }

            // 2. 回復 25% 最大生命值
            int skillHeal = round(maxHp * 0.25);
            heal(skillHeal);

            // 3. 狀態消失
            isCounterActive = false;
            activeEnemyLine = nullptr; // 解除指針綁定
            cout << " Counter state has faded." << endl;
        }

        if (!isAlive()) {
            cout << name << " has been defeated..." << endl;
        }
    }

    // 實作狂戰士三大技能
    bool Berserker::useSkill(int skillNumber, int targetIndex, std::vector<Monster*>& enemyLine) {
        if (skillNumber < 1 || skillNumber > 3) {
            cout << "The skill does not exist." << endl;
            return false;
        }

        int skillIndex = skillNumber - 1;
        Skill* usedSkill = skillbox[skillIndex];

        // 1. 檢查 CD
        if (currentCD[skillIndex] > 0) {
            cout << " Skill [" << usedSkill->getName() << "] is on cooldown! (" 
                 << currentCD[skillIndex] << " turns remaining)" << endl;
            return false;
        }

        // 2. 檢查並消耗魔力/生命
        if (skillNumber == 1) {
            // 嗜血狂擊：消耗自身 15% 當前生命值 (至少保留 1 點血，不直接自殺)
            int hpCost = round(hp * 0.15);
            if (hpCost < 1) hpCost = 1;
            
            if (hp <= hpCost) {
                cout << " Too low HP to cast [嗜血狂擊]!" << endl;
                return false;
            }
            hp -= hpCost;
            cout  << name << " consumes " << hpCost << " HP to empower the strike! (Remaining HP: " << hp << "/" << maxHp << ")" << endl;
        } else {
            // 其它技能消耗魔力
            if (!consumeMp(usedSkill->getMpCost())) return false;
        }

        cout << name << " casts a skill: [" << usedSkill->getName() << "]!" << endl;

        // 3. 執行技能效果
        if (skillNumber == 1) { // 嗜血狂擊 (擴散傷害)
            if (targetIndex < 0 || targetIndex >= enemyLine.size() || enemyLine[targetIndex] == nullptr || !enemyLine[targetIndex]->isAlive()) {
                cout << "Invalid or dead target!" << endl;
                return false;
            }

            Monster* mainTarget = enemyLine[targetIndex];
            int mainDamage = round(attackPower * 2.5); // 大量傷害 250%
            int splashDamage = round(mainDamage * 0.5); // 擴散 50%

            cout << "🪓 Cleaving " << mainTarget->getName() << "!" << endl;
            mainTarget->takeDamage(mainDamage);

            // 左側擴散
            if (targetIndex - 1 >= 0 && enemyLine[targetIndex - 1] != nullptr && enemyLine[targetIndex - 1]->isAlive()) {
                cout << " Splash hits left: " << enemyLine[targetIndex - 1]->getName() << endl;
                enemyLine[targetIndex - 1]->takeDamage(splashDamage);
            }
            // 右側擴散
            if (targetIndex + 1 < enemyLine.size() && enemyLine[targetIndex + 1] != nullptr && enemyLine[targetIndex + 1]->isAlive()) {
                cout << " Splash hits right: " << enemyLine[targetIndex + 1]->getName() << endl;
                enemyLine[targetIndex + 1]->takeDamage(splashDamage);
            }
        } 
        else if (skillNumber == 2) { // 反擊巨獸
            isCounterActive = true;
            activeEnemyLine = &enemyLine; // 綁定當前戰場，受傷時才能找到全體敵人
            cout << "🛡️ [Buff] Enters Counter State for 1 turn. Ready to retaliate!" << endl;
        } 
        else if (skillNumber == 3) { // 狂獸極刑
            // 計算生命比例與增傷
            double hpRatio = (double)hp / maxHp;
            double damageMultiplier = 2.0; // 基礎 200% 大量傷害

            // 自身生命比例越低則傷害越高，低於 30% 時達到最大值 (額外增傷 150%)
            if (hpRatio <= 0.3) {
                damageMultiplier += 1.5; 
                cout << " [Max Power] HP below 30%! Ultimate execution activated!" << endl;
            } else {
                // 按比例線性增傷
                damageMultiplier += (1.0 - hpRatio) * 1.5;
            }

            int finalAoeDamage = round(attackPower * damageMultiplier);
            cout << " Final damage multiplier: " << round(damageMultiplier * 100) << "%" << endl;

            // 對敵方全體造成傷害
            for (auto enemy : enemyLine) {
                if (enemy != nullptr && enemy->isAlive()) {
                    cout << " Executing " << enemy->getName() << "!" << endl;
                    enemy->takeDamage(finalAoeDamage);
                }
            }
        }

        // 進入 CD
        currentCD[skillIndex] = maxCD[skillIndex];
        return true;
    }

    // 每回合結束時呼叫，減少 CD
    void Berserker::updateTurnStatus() {
        for (int i = 0; i < 3; ++i) {
            if (currentCD[i] > 0) currentCD[i]--;
        }
        
        // 如果反擊狀態過了一回合都沒被分配受傷，清空狀態
        if (isCounterActive) {
            isCounterActive = false;
            activeEnemyLine = nullptr;
            cout << "[Counter Behemoth] Counter state expired without being hit." << endl;
        }
    }
}
