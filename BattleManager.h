#ifndef BATTLEMANAGER_H
#define BATTLEMANAGER_H

#include <vector>

using std::vector;

namespace RPG_Colaborate
{
    // 前置宣告
    class Player;
    class Skill;
    class Item;
    class Monster;

    // 戰鬥系統相關列舉
    enum TargetType { OWN, SINGLE, SPREAD, AOE, TEAM };
    enum HealTargetType { NONEH, OWNH, LOWERHP, TEAMH };
    enum EffectType { NONEE, LOWERDEFENSE, TAUNT, BURN, COLD, FREEZE, POISON, WEAKEN,
                      PERSEVERANCE, CONTSHOOT, SPACEGOAT, HIDE, FREEACTION, STRENGTH, FEAST, GOLDBULLET, COUNTERATTACK };

    class BattleManager
    {
    private:
        int currentTargetIdx;           // 當前玩家鎖定的怪物目標索引
        vector<Player*> players;        // 玩家小隊容器
        vector<Monster*> monsters;      // 怪物敵方容器

        // 內部戰鬥流向控制副程式
        void renderUI();
        void playerTurn(Player& currentPlayer, int currentPlayerIdx); // 更新：新增玩家索引參數
        void monsterPhase();            // 更新：接管整輪怪物行動的階段控制器
        int getValidMonsterTarget();    // 新增：處理 HIDE(隱身) 狀態的智能索敵核心
        void endOfRoundCleanup();       // 新增：大回合結束後的狀態遞減與毒燒結算
        int isBattleOver();             // 新增：檢查勝負狀態 (0:進行中, 1:玩家勝, 2:怪物勝)

    public:
        // 建構子
        BattleManager(vector<Player*> AllPlayers, vector<Monster*> AllMonsters);
        
        // 外部唯一呼叫介面：啟動戰鬥主迴圈
        void startBattle();
    };
}

#endif // BATTLEMANAGER_H