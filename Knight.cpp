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
        *skillbox[0] = Skill("Shield Charge", "Damage", attackPower, 30);
        *skillbox[1] = Skill("Knight Aura", "CC", 0, 50);
        *skillbox[2] = Skill("The Indomitable Will", "Buff", 0, 90);
    }
    Knight::Knight(string theName, int theMaxHp, int theMaxMp, int theAttackPower, int theDefense)
    : Player(theName, theMaxHp, theMaxMp, theAttackPower, theDefense), 
      criticalRate(15), criticalEffect(200)
    {
        job = "Knight";
        job = "Knight";
        *skillbox[0] = Skill("Shield Charge", "Damage", attackPower, 30);
        *skillbox[1] = Skill("Knight Aura", "CC", 0, 50);
        *skillbox[2] = Skill("The Indomitable Will", "Buff", 0, 90);
    }

    // getters
    int Knight::getCriticalRate() const { return criticalRate; }
    int Knight::getCriticalEffect() const { return criticalEffect; }

    // setters
    void Knight::setCriticalRate(int newRate) { criticalRate = newRate; }
    void Knight::setCriticalEffect(int newEffect) { criticalEffect = newEffect;}

    void Knight::takeDamage(int damage) {
        // Knight's Passive: Damage reduction triggers when HP is below 50%
        if (hp < (maxHp / 2)) {
            cout << "🛡️ [Knight Passive] HP below 50%! Guard activated." << endl;
            // Reduce incoming damage by 30% (Takes only 70% of the damage)
            Player::takeDamage(round(0.7 * damage));
        }
    }
}