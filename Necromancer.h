#ifndef NECROMANCER_H
#define NECROMANCER_H

#include "Player.h"
#include "Skill.h"
#include "Monster.h"
#include <vector>
#include <string>

using std::vector;
using std::string;

namespace RPG_Colaborate {
    // 前置宣告，避免循環 include
    class Skill;
    class Monster;

    class Necromancer : public Player
    {
    private:
        int undeadCount; // 當前擁有的亡靈數量
        // 內部追擊副程式：處理普攻與技能施放後的亡靈追擊邏輯
        void triggerUndead(vector<Monster*>& monsters);
    public:
        // 建構子與解構子
        Necromancer();
        Necromancer(string theName, int theMaxHp, int theMaxMp, int theAttackPower, int theDefense);
        virtual ~Necromancer() = default;

        // 覆寫父類別 Player 的虛擬函式
        virtual void attack(int targetIndex, std::vector<Monster*>& monsters, std::vector<Player*>& players) override;
        virtual bool useSkill(int skillInput, int targetIndex, std::vector<Player*>& players, std::vector<Monster*>& monsters) override;
        virtual void triggerClassSpecial(Skill& theSkill, int targetIndex, std::vector<Monster*>& monsters, std::vector<Player*>& players) override;

        // 亡靈數量 Getter / Setter (利於系統外部讀取或面板顯示)
        int getUndeadCount() const { return undeadCount; }
        void setUndeadCount(int count) { undeadCount = count; }
    };
}

#endif // NECROMANCER_H