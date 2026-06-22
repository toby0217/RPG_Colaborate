#ifndef BATTLEMANAGER_H
#define BATTLEMANAGER_H

#include <vector>
#include <string>

using std::vector;
using std::string;

namespace RPG_Colaborate
{
    // 前置宣告
    class Player;
    class Skill;
    class Item;
    class Monster;
    class VampireBat;
    class BlazeShaman;
    class SkeletonMasterSergeant;
    class FrostSlime;
    class TheDeceiver;

    // 戰鬥系統相關列舉
    enum TargetType { OWN, SINGLE, SPREAD, AOE, TEAM };
    enum HealTargetType { NONEH, OWNH, LOWERHP, TEAMH };
    enum EffectType { NONEE, LOWERDEFENSE, BURN, COLD, FREEZE, POISON, WEAKNESS, ELETRICSHOCK,
                      TAUNT, PERSEVERANCE, CONTSHOOT, SPACEGOAT, HIDE, FREEACTION, STRENGTH, FEAST, GOLDBULLET, COUNTERATTACK, GOLDEN_BELL,
                      FLOOR_STRENGTH, FLOOR_CALAMITY, LAST_GASP };

    enum MonsterType {
        NONEM = 0, // 代表空位 (nullptr)
        SLIME,
        GOBLIN,
        GOBLIN_W,
        ELITE_G,
        SKELETON,
        BAT,
        SHAMAN,
        SERGEANT,
        DK,
        ICE_SLIME,
        DECEIVER
    };
    
    using M = MonsterType;

    const std::vector<std::vector<MonsterType>> STAGE_DATABASE = {
        // 站位：       [1]      [2]         [3]         [4]         [5]
        /* 層數 1 */ { M::NONEM, M::SLIME,   M::GOBLIN,  M::SLIME,   M::NONEM },
        /* 層數 2 */ { M::NONEM, M::GOBLIN,  M::GOBLIN_W,M::GOBLIN,  M::NONEM },
        /* 層數 3 */ { M::NONEM, M::GOBLIN_W,M::ELITE_G, M::GOBLIN_W,M::NONEM },
        /* 層數 4 */ { M::NONEM, M::SKELETON,M::BAT,     M::SKELETON,M::NONEM },
        /* 層數 5 */ { M::SKELETON, M::BAT,  M::SHAMAN,  M::BAT,     M::SKELETON },
        /* 層數 6 */ { M::NONEM, M::NONEM,   M::SERGEANT,M::NONEM,   M::NONEM },
        /* 層數 7 */ { M::NONEM, M::SHAMAN,  M::SHAMAN,  M::SHAMAN,  M::NONEM },
        /* 層數 8 */ { M::NONEM, M::SHAMAN,  M::DK,      M::BAT,     M::NONEM },
        /* 層數 9 */ { M::NONEM, M::SLIME,   M::ICE_SLIME,M::SLIME,  M::NONEM },
        /* 層數 10*/ { M::NONEM, M::NONEM,   M::DECEIVER, M::NONEM,  M::NONEM }
    };
    
    inline string getEffectName(EffectType type) 
    {
        switch (type) {
            case LOWERDEFENSE:  return "LOWERDEFENSE";
            case TAUNT:         return "TAUNT";
            case BURN:          return "BURN";
            case COLD:          return "COLD";
            case FREEZE:        return "FREEZE";
            case POISON:        return "POISON";
            case WEAKNESS:      return "WEAKNESS";
            case PERSEVERANCE:  return "PERSEVERANCE";
            case CONTSHOOT:     return "CONTSHOOT";
            case SPACEGOAT:     return "SPACEGOAT";
            case HIDE:          return "HIDE";
            case FREEACTION:    return "FREEACTION";
            case STRENGTH:      return "STRENGTH";
            case FEAST:         return "FEAST";
            case GOLDBULLET:    return "GOLDBULLET";
            case COUNTERATTACK: return "COUNTERATTACK";
            default:            break;
        }
        return "";
    }

    class BattleManager
    {
    private:
        int currentTargetIdx;           // 當前玩家鎖定的怪物目標索引
        vector<Player*> players;        // 玩家小隊容器
        vector<Monster*> monsters;      // 怪物敵方容器
        vector<bool> playerTurnReached;  // 玩家輪次是否抵達
        vector<bool> monsterTurnReached; // 怪物輪次是否抵達

        // 內部戰鬥流向控制副程式
        void preBattlePhase();          // 🆕 新增：戰前(脫戰)準備階段
        void renderUI(int activePlayerIdx = -1);
        void playerTurn(Player& currentPlayer, int currentPlayerIdx);
        void autoLockFirstTarget();
        void switchTarget();
        void monsterPhase();
        int getValidMonsterTarget();
        void endOfRoundCleanup();
        int isBattleOver();

    public:
        BattleManager(vector<Player*> AllPlayers);
        void startBattle();
    };

    // 🆕 新增：專屬的怪物生成工廠 (搭配你的 STAGE_DATABASE 使用)
    class MonsterFactory {
    public:
        static vector<Monster*> createStageMonsters(int stageLevel);
    };
}

#endif // BATTLEMANAGER_H