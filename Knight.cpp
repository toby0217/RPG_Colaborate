#include "Knight.h"
#include <iostream>
#include <vector>
#include <cmath>
using std::vector;
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
        skillbox[1] = new Skill("Knight Aura", OWN, NONEH, TAUNT, 1,
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
        skillbox[1] = new Skill("Knight Aura", OWN, NONEH, TAUNT, 1,
            NONE, STATIC, NONE, NONE, NONE, NONE, NONE, 0, 0, 0, 50, 0, 5);
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

    

    // ✨ 新增：實作騎士的專屬普攻（支援暴擊與災厄之手加成）
    void Knight::attack(int targetIndex, vector<Monster*>& monsters, vector<Player*>& players) {
        int currentCritRate = calculateFinalCritRate(criticalRate);
        int currentCritEffect = calculateFinalCritEffect(criticalEffect);

        int finalDamage = getAttackPower();
        if (getEffectTurns(LAST_GASP) > 0) {
            finalDamage *= 4;
            takeEffect(LAST_GASP, 0); // ⚡ The moment the attack is unleashed, the status is immediately cleared!
            cout << "🩸 The Last Gasp effect has been released with the attack, status removed.\n";
        }
        if (rand() % 100 < currentCritRate) {
            finalDamage = static_cast<int>(finalDamage * 0.01* currentCritEffect);
            cout << " Critical Hit! ";
        }
        cout << name << " performs a basic attack!" << endl;
        monsters[targetIndex]->takeDamage(finalDamage);
    }
    
    void Knight::takeDamage(int damage, vector<Monster*>& monsters) {
        int finalDamage = calculateFinalDamage(damage);
        if (getEffectTurns(GOLDEN_BELL) > 0) {
            cout << "🛡️ Golden Bell activated! Immune to all damage!\n";
            damage = 0;
        }
        // Knight's Passive: Damage reduction triggers when HP is below 50%
        if (hp < (maxHp / 2)) {
            cout << "🛡️ [Knight Passive] HP below 50%! Guard activated." << endl;
            // Reduce incoming damage by 30% (Takes only 70% of the damage)
            finalDamage = round(0.7 * damage);
        }
        
        if (getEffectTurns(PERSEVERANCE) > 0 && (hp - finalDamage) <= 0) {
            hp = 1; // 🎯 強制鎖血在 1 點！
            
            // 自己攔截 UI 輸出，這樣就不會印出 0 血或死亡宣告了
            cout << name << " takes " << finalDamage << " points of damage! (🛡️ Perseverance Mitigated!) " 
                << "(Current HP: " << hp << "/" << maxHp << ")" << endl;
        }
        else {
            Player::takeDamage(damage, monsters);
        }
    }

    bool Knight::useSkill(int skillInput, int targetIndex, vector<Player*>& players, vector<Monster*>& monsters)
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
            cout << "🛡️ [Knight]: \"Break their ranks! Shield Charge!\"" << endl;
        } else if (skillNumber == 1) {
            cout << "🛡️ [Knight]: \"Come at me! I am your impenetrable wall!\"" << endl;
        } else if (skillNumber == 2) {
            cout << "🛡️ [Knight]: \"My will is forged in iron... I shall not fall!\"" << endl;
        }

        // 呼叫父類的共通邏輯 (扣魔、扣血、呼叫 Skill::use)
        return Player::useSkill(skillInput, targetIndex, players, monsters);
    }
}
