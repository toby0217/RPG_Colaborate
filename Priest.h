#ifndef PRIEST_H
#define PRIEST_H

#include "Player.h"
#include <vector>

namespace RPG_Colaborate {

    class Priest : public Player {
    private:
        int criticalRate;
        int criticalEffect;
        bool canRevive; 

        void triggerPassiveHeal(std::vector<Player*>& teammates);

    public:
        Priest(string theName, int theMaxHp, int theMaxMp, int theAttackPower, int theDefense);
        virtual ~Priest() override;

        virtual void attackWithHeal(std::vector<Monster*>& targets, int primaryIndex, std::vector<Player*>& teammates);
        
        void castRetributionRay(Monster* target, std::vector<Player*>& teammates); 
        void castHealingArt(std::vector<Player*>& teammates);                    
        void castDivineResurrection(Player* deadTeammate);                       
    };

} // namespace RPG_Colaborate

#endif // PRIEST_H