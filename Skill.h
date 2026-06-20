#ifndef SKILL_H
#define SKILL_H

#include "BattleManager.h"
#include <string>
#include <vector>

using std::string;
using std::vector;

namespace RPG_Colaborate
{
    class Player;
    class Monster;

    enum SkillType { NONE, DAMAGE, STATIC, BUFF, DEBUFF, HEAL, REVIVE, SPECIAL };
    
    class Skill
    {
    private:
        string name;
        string type;
        TargetType targetType;
        HealTargetType healTargetType;
        EffectType effectType;
        int effectTurns;
        SkillType isDamage;
        SkillType isStatic;
        SkillType isBuff;
        SkillType isDebuff;
        SkillType isHeal;
        SkillType isRevive;
        SkillType isSpecial;
        int damage;
        double multiplier;
        int healPercent;
        int mpCost;
        int hpCost;
        int CD;
    public:
        // 建構子
        Skill();
        Skill(string theName, string theType, int theDamage, int theMpCost);
        Skill(string theName, TargetType TType, HealTargetType HType, EffectType EType, int ETurns,
        SkillType isDmg, SkillType isSt, SkillType isB, SkillType isDb, SkillType isH, SkillType isRv, SkillType isSp,
        int theDamage, double theMultiplier, int theHealPercent, int theMpCost, int theHpCost, int CD);
        ~Skill();

        // Getters
        string getName() const;
        string getType() const;
        TargetType getTargetType() const;
        HealTargetType getHealTargetType() const;
        EffectType getEffectType() const;
        int getEffectTurns() const;
        SkillType getIsDamage() const;
        SkillType getIsStatic() const;
        SkillType getIsBuff() const;
        SkillType getIsDebuff() const;
        SkillType getIsHeal() const;
        SkillType getIsRevive() const;
        SkillType getIsSpecial() const;
        int getDamage() const;
        double getMultiplier() const;
        int getHealPercent() const;
        int getMpCost() const;
        int getHpCost() const;
        int getCD() const;

        // Setters
        void setName(string newName);
        void setType(string newType);
        void setTargetType(TargetType newTargetType);
        void setHealTargetType(HealTargetType newHealTargetType);
        void setEffectType(EffectType newEffectType);
        void setEffectTurns(int newEffectTurns);
        void setIsDamage(SkillType newIsDamage);
        void setIsStatic(SkillType newIsStatic);
        void setIsBuff(SkillType newIsBuff);
        void setIsDebuff(SkillType newIsDebuff);
        void setIsHeal(SkillType newIsHeal);
        void setIsRevive(SkillType newIsRevive);
        void setIsSpecial(SkillType newIsSpecial);
        void setDamage(int newDamage);
        void setMultiplier(double newMultiplier);
        void setHealPercent(int newHealPercent);
        void setMpCost(int newMpCost);
        void setHpCost(int newHpCost);
        void setCD(int newCD);

        // 功能
        void use(Player& user, int targetIndex, vector<Player*>& players, vector<Monster*>& monsters);
        void showInfo() const;
    };
}

#endif