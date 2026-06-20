#include "Ranger.h"
#include <iostream>
#include <cmath>
#include <cstdlib>

using std::cout;
using std::endl;

namespace RPG_Colaborate {
    Ranger::Ranger()
    : Player(), criticalRate(15), criticalEffect(200), multiShotTurns(0)
    {
        job = "Ranger";
        skillbox[0] = new Skill("連續射擊", OWN, NONEH, CONTSHOOT, 2,
            NONE, STATIC, NONE, NONE, NONE, NONE, NONE, attackPower, 0, 0, 30, 0, 4);
        skillbox[1] = new Skill("假人操術", OWN, NONEH, SPACEGOAT, 3,
            NONE, STATIC, NONE, NONE, NONE, NONE, NONE, attackPower, 0, 0, 40, 0, 3);
        skillbox[2] = new Skill("萬箭齊發", SINGLE, NONEH, NONEE, 0,
            NONE, NONE, NONE, NONE, NONE, NONE, SPECIAL, attackPower, 0.6, 0, 70, 0, 6);
    }

    Ranger::Ranger(string theName, int theMaxHp, int theMaxMp, int theAttackPower, int theDefense)
    : Player(theName, theMaxHp, theMaxMp, theAttackPower, theDefense), 
      criticalRate(15), criticalEffect(200), multiShotTurns(0)
    {
        job = "Ranger";
        skillbox[0] = new Skill("連續射擊", OWN, NONEH, CONTSHOOT, 2,
            NONE, STATIC, NONE, NONE, NONE, NONE, NONE, attackPower, 0, 0, 30, 0, 4);
        skillbox[1] = new Skill("假人操術", OWN, NONEH, SPACEGOAT, 3,
            NONE, STATIC, NONE, NONE, NONE, NONE, NONE, attackPower, 0, 0, 40, 0, 3);
        skillbox[2] = new Skill("萬箭齊發", SINGLE, NONEH, NONEE, 0,
            DAMAGE, NONE, NONE, NONE, NONE, NONE, NONE, attackPower, 0.6, 0, 70, 0, 6);
    }

    int Ranger::getCriticalRate() const { return criticalRate; }
    int Ranger::getCriticalEffect() const { return criticalEffect; }
    int Ranger::getMultiShotTurns() const { return multiShotTurns; }
    void Ranger::setCriticalRate(int newRate) { criticalRate = newRate; }
    void Ranger::setCriticalEffect(int newEffect) { criticalEffect = newEffect; }

    void Ranger::attack(int targetIndex, vector<Monster*> monsters, vector<Player*> players) {
        bool isBoss = (monsters[targetIndex]->getName().find("Boss") != string::npos);
        double multiplier = isBoss ? 1.5 : 1.0;

        int currentCritRate = criticalRate + ((StatusEffectList[CONTSHOOT] >= 0) ? 30 : 0);

        if (isBoss) {
            cout << " [Ranger Passive] Locked onto Boss! Damage increased by 50%!" << endl;
        }

        if (StatusEffectList[CONTSHOOT] >= 0) {
            for (int i = 1; i <= 3; i++) {
                int finalDamage = round(0.6 * attackPower * multiplier);
                if (rand() % 100 < currentCritRate) {
                    finalDamage = finalDamage * criticalEffect / 100;
                    cout << " Critical Hit! ";
                }
                cout << name << " shoots an arrow at " << monsters[targetIndex]->getName() << "!" << endl;
                monsters[targetIndex]->takeDamage(finalDamage);
                
                if (!monsters[targetIndex]->isAlive()) break; 
            }
        }
        else {
            int finalDamage = attackPower;
            if (rand() % 100 < currentCritRate) {
                finalDamage = finalDamage * criticalEffect / 100;
                cout << " Critical Hit! ";
            }
            cout << name << " shoots an arrow at " << monsters[targetIndex]->getName() << "!" << endl;
            monsters[targetIndex]->takeDamage(finalDamage);
        }
    }

    bool Ranger::useSkill(int skillNumber, int targetIndex, vector<Player*> players, vector<Monster*> monsters) {
        if (skillNumber < 0 || skillNumber >= 3 || skillbox[skillNumber] == nullptr) return false;

        int mpRequired = skillbox[skillNumber]->getMpCost();
        if (mp < mpRequired) {
            cout << name << " does not have enough MP!" << endl;
            return false;
        }

        if (skillNumber == 0) { 
            multiShotTurns = 2; 
            cout << " [Buff] Basic attacks become Triple Shot for 2 turns! (Crit Rate +30%)" << endl;
            // 回傳 false 讓主迴圈不視為行動結束
            return false; 
        } 
        else if (skillNumber == 1) { 
            cout << "🪵 [Hide] Summoned a Decoy to absorb incoming damage!" << endl;
            // 實際減傷或阻擋交由引擎 SPACEGOAT 判定
        } 
        else if (skillNumber == 2) { 
            cout << " [Arrow Rain] Unleashing a barrage of arrows!" << endl;
            int currentCritRate = criticalRate + ((multiShotTurns > 0) ? 30 : 0); // 大招享受連射暴擊加成
            int bouncesLeft = 10;

            // 第一發優先命中主目標
            if (monsters[targetIndex] != nullptr && monsters[targetIndex]->isAlive()) {
                int arrowDamage = attackPower;
                if (rand() % 100 < currentCritRate) {
                    arrowDamage = arrowDamage * criticalEffect / 100;
                    cout << " Critical Hit! ";
                }
                cout << "Arrow hits " << monsters[targetIndex]->getName() << "!" << endl;
                monsters[targetIndex]->takeDamage(arrowDamage);
                bouncesLeft--;
            }

            // 剩餘發數在全體存活敵人中隨機彈射
            std::vector<Monster*> aliveMonsters;
            for (auto m : monsters) {
                if (m != nullptr && m->isAlive()) aliveMonsters.push_back(m);
            }

            for (int i = 0; i < bouncesLeft; ++i) {
                aliveMonsters.clear(); // 重新確認存活狀態
                for (auto m : monsters) {
                    if (m != nullptr && m->isAlive()) aliveMonsters.push_back(m);
                }

                if (aliveMonsters.empty()) {
                    break;
                }

                Monster* randomTarget = aliveMonsters[rand() % aliveMonsters.size()];
                int arrowDamage = attackPower;
                if (rand() % 100 < currentCritRate) {
                    arrowDamage = arrowDamage * criticalEffect / 100;
                    cout << " Critical Hit! ";
                }
                cout << "Arrow hits " << randomTarget->getName() << "!" << endl;
                randomTarget->takeDamage(arrowDamage);
            }
        }
        return Player::useSkill(skillNumber, targetIndex, players, monsters);
    }
}