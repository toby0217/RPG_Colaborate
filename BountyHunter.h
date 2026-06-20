#ifndef BOUNTYHUNTER_H
#define BOUNTYHUNTER_H

#include "Player.h"
#include <vector>

namespace RPG_Colaborate {

    class Monster; // 前置宣告

    class BountyHunter : public Player {
    private:
        int gold;               
        int bulletType;         
        int goldenMpConsumed;   
        int goldenBulletTurns;

        int criticalRate;
        int criticalEffect;

    public:
        // 對齊 Player 的 5 個參數，再加上專屬的 theGold
        BountyHunter(string theName, int theMaxHp, int theMaxMp, int theAttackPower, int theDefense, int theGold);
        virtual ~BountyHunter() override;

        virtual void takeDamage(int damage) override;

        void attackShotgun(Monster* primaryTarget, std::vector<Monster*>& allEnemies);

        void loadRedBullet();   
        void loadBlueBullet();  
        void castGoldenBullet(); 
    };

} // namespace RPG_Colaborate

#endif // BOUNTYHUNTER_H