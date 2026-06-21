#ifndef BATTLEMANAGER_H
#define BATTLEMANAGER_H

#include <vector>
using std::vector;

namespace RPG_Colaborate
{
    class Player;
    class Skill;
    class Item;
    class Monster;

    enum TargetType { OWN, SINGLE, SPREAD, AOE, TEAM };
    enum HealTargetType { NONEH, OWNH, LOWERHP, TEAMH };
    enum EffectType { NONEE, LOWERDEFENSE, TAUNT, BURN, COLD, FREEZE, POISON, WEAKEN,
                      PERSEVERANCE, CONTSHOOT, SPACEGOAT, HIDE, FREEACTION, STRENGTH, FEAST, GOLDBULLET, COUNTERATTACKK , SHOCKED };

    class BattleManager
    {
    private:
        int currentTargetIdx;
        vector<Player*> players;
        vector<Monster*> monsters;
        void renderUI();
        void playerTurn(Player& currentPlayer);
        void monsterTurn(Monster& currentMonster);
    public:
        BattleManager(vector<Player*> AllPlayers, vector<Monster*> AllMonsters);
        void startBattle();
    };
}

#endif
