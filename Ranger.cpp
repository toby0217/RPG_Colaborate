#include "Ranger.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <vector>

using std::vector;
using std::cout;
using std::endl;

namespace RPG_Colaborate {
    Ranger::Ranger()
    : Player(), criticalRate(15), criticalEffect(200), multiShotTurns(0), spacegoatHp(0)
    {
        job = "Ranger";
        skillbox[0] = new Skill("Cascading Shots", OWN, NONEH, CONTSHOOT, 2,
            NONE, STATIC, NONE, NONE, NONE, NONE, SPECIAL, attackPower, 0, 0, 30, 0, 4);
        skillbox[1] = new Skill("Mirage Artifice", OWN, NONEH, SPACEGOAT, 3,
            NONE, STATIC, NONE, NONE, NONE, NONE, SPECIAL, attackPower, 0, 0, 40, 0, 3);
        skillbox[2] = new Skill("Endless Volley", SINGLE, NONEH, NONEE, 0,
            NONE, NONE, NONE, NONE, NONE, NONE, SPECIAL, attackPower, 0.6, 0, 70, 0, 6);
    }

    Ranger::Ranger(string theName, int theMaxHp, int theMaxMp, int theAttackPower, int theDefense)
    : Player(theName, theMaxHp, theMaxMp, theAttackPower, theDefense), 
      criticalRate(15), criticalEffect(200), multiShotTurns(0), spacegoatHp(0)
    {
        job = "Ranger";
        skillbox[0] = new Skill("Cascading Shots", OWN, NONEH, CONTSHOOT, 2,
            NONE, STATIC, NONE, NONE, NONE, NONE, SPECIAL, attackPower, 0, 0, 30, 0, 4);
        skillbox[1] = new Skill("Mirage Artifice", OWN, NONEH, SPACEGOAT, 3,
            NONE, STATIC, NONE, NONE, NONE, NONE, SPECIAL, attackPower, 0, 0, 40, 0, 3);
        skillbox[2] = new Skill("Endless Volley", SINGLE, NONEH, NONEE, 0,
            NONE, NONE, NONE, NONE, NONE, NONE, SPECIAL, attackPower, 0.6, 0, 70, 0, 6);
    }

    int Ranger::getCriticalRate() const { return criticalRate; }
    int Ranger::getCriticalEffect() const { return criticalEffect; }
    int Ranger::getMultiShotTurns() const { return multiShotTurns; }
    void Ranger::setCriticalRate(int newRate) { criticalRate = newRate; }
    void Ranger::setCriticalEffect(int newEffect) { criticalEffect = newEffect; }

    void Ranger::attack(int targetIndex, vector<Monster*>& monsters, vector<Player*>& players) {
        bool isElite = (monsters[targetIndex]->getRank() == ELITE || monsters[targetIndex]->getRank() == BOSS);
        double multiplier = isElite ? 1.5 : 1.0;

        int currentCritRate = criticalRate + ((StatusEffectList[CONTSHOOT] >= 0) ? 30 : 0);

        if (isElite) {
            cout << " [Ranger Passive] Locked onto Boss! Damage increased by 50%!" << endl;
        }

        if (StatusEffectList[CONTSHOOT] >= 0) {
            for (int i = 1; i <= 3; i++) {
                int finalDamage = round(0.6 * getAttackPower() * multiplier);
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
            int finalDamage = getAttackPower();
            if (rand() % 100 < currentCritRate) {
                finalDamage = finalDamage * criticalEffect / 100;
                cout << " Critical Hit! ";
            }
            cout << name << " shoots an arrow at " << monsters[targetIndex]->getName() << "!" << endl;
            monsters[targetIndex]->takeDamage(finalDamage);
        }
    }

    void Ranger::takeDamage(int damage, vector<Monster*>& monsters)
    {
        if (getEffectTurns(SPACEGOAT) > 0) {
            spacegoatHp -= damage;
            cout << "Spacegoat takes " << damage << " points of damage! " 
                << "(Current HP: " << spacegoatHp << ")" << endl;
            // 生命為0，直接破壞假人，遊俠不會受到傷害
            if (spacegoatHp <= 0) {
                spacegoatHp = 0;
                StatusEffectList[SPACEGOAT] = 0;
                cout << "🪆 The Decoy Dummy completely crumbles into pieces! The Ranger is now exposed!" << endl;
            }
        }
        else {
            Player::takeDamage(damage, monsters);
        }
    }

    bool Ranger::useSkill(int skillInput, int targetIndex, vector<Player*>& players, vector<Monster*>& monsters)
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
            cout << "🏹 [Ranger]: \"Can you dodge this? Let's see your speed!\"" << endl;
        } else if (skillNumber == 1) {
            cout << "🎯 [Ranger]: \"Fooled you! Look over here, dummy!\"" << endl;
        } else if (skillNumber == 2) {
            cout << "🌧️ [Ranger]: \"Watch closely! This is full-coverage shooting with zero blind spots!\"" << endl;
        }

        return Player::useSkill(skillInput, targetIndex, players, monsters);
    }

    void Ranger::triggerClassSpecial(Skill& theSkill, int targetIndex, vector<Monster*>& monsters, vector<Player*>& players)
    {
        if (&theSkill == skillbox[0]) { 
            takeEffect(FREEACTION, 1);
        }
        else if (&theSkill == skillbox[1]) {
            spacegoatHp = 0.5 * maxHp;
        }
        else if (&theSkill == skillbox[2]) { 
            int currentCritRate = criticalRate + ((multiShotTurns > 0) ? 30 : 0); // 大招享受連射暴擊加成
            int bouncesLeft = 10;

            // 第一發優先命中主目標
            if (monsters[targetIndex] != nullptr && monsters[targetIndex]->isAlive()) {
                int arrowDamage = getAttackPower();
                if (rand() % 100 < currentCritRate) {
                    arrowDamage = arrowDamage * criticalEffect / 100;
                    cout << " Critical Hit! ";
                }
                cout << "Arrow hits " << monsters[targetIndex]->getName() << "!" << endl;
                monsters[targetIndex]->takeDamage(arrowDamage);
                bouncesLeft--;
            }

            // 剩餘發數在全體存活敵人中隨機彈射
            vector<Monster*> aliveMonsters;
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
                int arrowDamage = getAttackPower();
                if (rand() % 100 < currentCritRate) {
                    arrowDamage = arrowDamage * criticalEffect / 100;
                    cout << " Critical Hit! ";
                }
                cout << "Arrow hits " << randomTarget->getName() << "!" << endl;
                randomTarget->takeDamage(arrowDamage);
            }
        }
    }
}