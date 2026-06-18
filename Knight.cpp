#include "Knight.h"
#include <iostream>
#include <cmath>

namespace RPG_Colaborate {
    void Knight::takeDamage(int incomingDamage) {
        int finalDamage = incomingDamage;

        // Knight's Passive: Damage reduction triggers when HP is below 50%
        if (hp < (maxHp / 2)) {
            std::cout << "🛡️ [Knight Passive] HP below 50%! Guard activated." << std::endl;
            // Reduce incoming damage by 30% (Takes only 70% of the damage)
            finalDamage = std::round(incomingDamage * 0.7);
        }

        // Apply Knight's base defense reduction (Optional, depending on your system)
        finalDamage -= (defense / 2);
        if (finalDamage <= 0 && incomingDamage > 0) {
            finalDamage = 1; // Minimum 1 damage guarantee
        }

        hp -= finalDamage;
        if (hp < 0) hp = 0;

        std::cout << "💥 " << name << " took " << finalDamage << " damage. Current HP: " << hp << "/" << maxHp << std::endl;
    }
}