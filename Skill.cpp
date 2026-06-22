#include "BattleManager.h"
#include "Skill.h"
#include "Player.h"
#include "Monster.h"
#include <iostream>
#include <string>
#include <vector>
using std::string;
using std::vector;
using std::cout;
using std::endl;

namespace RPG_Colaborate
{
    //constructors
    Skill::Skill() {}
    Skill::Skill(string theName, string theType, int theDamage, int theMpCost)
    : name(theName), type(theType), damage(theDamage), mpCost(theMpCost) {}
    Skill::Skill(string theName, TargetType TType, HealTargetType HType, EffectType EType, int ETurns,
    SkillType isDmg, SkillType isSt, SkillType isB, SkillType isDb, SkillType isH, SkillType isRv, SkillType isSp,
    int theDamage, double theMultiplier, int theHealPercent, int theMpCost, int theHpCost, int theCD)
    : name(theName), targetType(TType), healTargetType(HType), effectType(EType), effectTurns(ETurns),
    isDamage(isDmg), isStatic(isSt), isBuff(isB), isDebuff(isDb), isHeal(isH), isRevive(isRv), isSpecial(isSp),
    damage(theDamage), multiplier(theMultiplier), healPercent(theHealPercent), mpCost(theMpCost), hpCost(theHpCost), CD(theCD), currentCD(0) {}

    Skill::~Skill(){}

    // getters
    string Skill::getName() const { return name; }
    string Skill::getType() const { return type; }
    TargetType Skill::getTargetType() const { return targetType; }
    HealTargetType Skill::getHealTargetType() const { return healTargetType; }
    EffectType Skill::getEffectType() const { return effectType; }
    int Skill::getEffectTurns() const { return effectTurns; }
    SkillType Skill::getIsDamage() const { return isDamage; }
    SkillType Skill::getIsStatic() const { return isStatic; }
    SkillType Skill::getIsBuff() const { return isBuff; }
    SkillType Skill::getIsDebuff() const { return isDebuff; }
    SkillType Skill::getIsHeal() const { return isHeal; }
    SkillType Skill::getIsRevive() const { return isRevive; }
    SkillType Skill::getIsSpecial() const { return isSpecial; }
    int Skill::getDamage() const { return damage; }
    double Skill::getMultiplier() const { return multiplier; }
    int Skill::getHealPercent() const { return healPercent; }
    int Skill::getMpCost() const { return mpCost; }
    int Skill::getHpCost() const { return hpCost; }
    int Skill::getCD() const { return CD; }
    int Skill::getCurrentCD() const { return currentCD; }

    // setters
    void Skill::setName(string newName) { name = newName; }
    void Skill::setType(string newType) { type = newType; }
    void Skill::setTargetType(TargetType newTargetType) { targetType = newTargetType; }
    void Skill::setHealTargetType(HealTargetType newHealTargetType) { healTargetType = newHealTargetType; }
    void Skill::setEffectType(EffectType newEffectType) { effectType = newEffectType; }
    void Skill::setEffectTurns(int newEffectTurns) { effectTurns = newEffectTurns; }
    void Skill::setIsDamage(SkillType newIsDamage) { isDamage = newIsDamage; }
    void Skill::setIsStatic(SkillType newIsStatic) { isStatic = newIsStatic; }
    void Skill::setIsBuff(SkillType newIsBuff) { isBuff = newIsBuff; }
    void Skill::setIsDebuff(SkillType newIsDebuff) { isDebuff = newIsDebuff; }
    void Skill::setIsHeal(SkillType newIsHeal) { isHeal = newIsHeal; }
    void Skill::setIsRevive(SkillType newIsRevive) { isRevive = newIsRevive; }
    void Skill::setIsSpecial(SkillType newIsSpecial) { isSpecial = newIsSpecial; }
    void Skill::setDamage(int newDamage) { damage = newDamage; }
    void Skill::setMultiplier(double newMultiplier) { multiplier = newMultiplier; }
    void Skill::setHealPercent(int newHealPercent) { healPercent = newHealPercent; }
    void Skill::setMpCost(int newMpCost) { mpCost = newMpCost; }
    void Skill::setHpCost(int newHpCost) { hpCost = newHpCost; }
    void Skill::setCD(int newCD) { CD = newCD; }
    void Skill::setCurrentCD(int newCurrentCD) { currentCD = newCurrentCD; }

    //function
    // 使用技能(已調整)
    // 讓技能主導施法程序
    void Skill::use(Player& user, int targetIndex, vector<Player*>& players, vector<Monster*>& monsters)
    {
        currentCD = CD;
        damage = multiplier * user.getAttackPower();
        int leftTargetIndex, rightTargetIndex;

        // 技能有傷害:依照類型進行索敵
        if (isDamage == DAMAGE) {
            if (user.getEffectTurns(LAST_GASP) > 0) {
                damage *= 4;
                user.takeEffect(LAST_GASP, 0); // ⚡ The moment the attack is unleashed, the status is immediately cleared!
                cout << "🩸 The Last Gasp effect has been released with the attack, status removed.\n";
            }
            switch (targetType)
            {
            case SINGLE:
                monsters[targetIndex]->takeDamage(damage);
                break;
            case SPREAD:
                monsters[targetIndex]->takeDamage(damage);
                leftTargetIndex = targetIndex - 1;
                while (leftTargetIndex >= 0 &&
                    (monsters[leftTargetIndex] == nullptr || !monsters[leftTargetIndex]->isAlive())) {
                    leftTargetIndex--;
                }
                if (leftTargetIndex >= 0) {
                    monsters[leftTargetIndex]->takeDamage(0.5 * damage);
                }

                rightTargetIndex = targetIndex + 1;
                while (rightTargetIndex < monsters.size() &&
                    (monsters[rightTargetIndex] == nullptr || !monsters[rightTargetIndex]->isAlive())) {
                    rightTargetIndex++;
                }
                if (rightTargetIndex > 0) {
                    monsters[rightTargetIndex]->takeDamage(0.5 * damage);
                }
                break;
            case AOE:
                for (int i = 0; i < monsters.size(); i++) {
                    if (monsters[i] != nullptr && monsters[i]->isAlive()) {
                        monsters[i]->takeDamage(damage);
                    }
                }
                break;
            default:
                break;
            }
        }

        if (isStatic == STATIC) {
            if (targetType == OWN) {
                user.takeEffect(effectType, effectTurns);
            }
        }

        // 技能有對隊友增益
        if (isBuff == BUFF) {
            switch (targetType)
            {
            case OWN:
                user.takeEffect(effectType, effectTurns);
                break;
            case TEAM:
                for (int i = 0; i < players.size(); i++) {
                    if (players[i]->isAlive()) {
                        players[i]->takeEffect(effectType, effectTurns);
                    }
                }
            default:
                break;
            }
        }

        // 技能有減益敵人
        if (isDebuff == DEBUFF) {
            switch (targetType)
            {
            case SINGLE:
                monsters[targetIndex]->takeEffect(effectType, effectTurns);
                break;
            case SPREAD:
                monsters[targetIndex]->takeEffect(effectType, effectTurns);
                leftTargetIndex = targetIndex - 1;
                while (leftTargetIndex >= 0 &&
                    (monsters[leftTargetIndex] == nullptr || !monsters[leftTargetIndex]->isAlive())) {
                    leftTargetIndex--;
                }
                if (leftTargetIndex > 0) {
                    monsters[leftTargetIndex]->takeEffect(effectType, effectTurns);
                }

                rightTargetIndex = targetIndex + 1;
                while (leftTargetIndex < monsters.size() &&
                    (monsters[rightTargetIndex] == nullptr || !monsters[rightTargetIndex]->isAlive())) {
                    rightTargetIndex++;
                }
                if (rightTargetIndex > 0) {
                    monsters[rightTargetIndex]->takeEffect(effectType, effectTurns);
                }
                break;
            case AOE:
                for (int i = 0; i < monsters.size(); i++) {
                    if (monsters[i] != nullptr && monsters[i]->isAlive()) {
                        monsters[i]->takeEffect(effectType, effectTurns);
                    }
                }
                break;
            default:
                break;
            }
        }

        // 技能有治療
        if (isHeal == HEAL) {
            double maxHpRate = 0;
            int lowerHpIndex = 0;
            switch (healTargetType)
            {
            case OWN:
                user.heal(0.01 * healPercent * user.getMaxHp());
                break;
            case LOWERHP:
                for (int i = 0; i < players.size(); i++) {
                    double playerHpRate = 1.0 * players[i]->getHp() / players[i]->getMaxHp();
                    if (playerHpRate > maxHpRate) {
                        maxHpRate = playerHpRate;
                        lowerHpIndex = i;
                    }
                }
                players[lowerHpIndex]->heal(healPercent);
                break;
            case TEAM:
                for (int i = 0; i < players.size(); i++) {
                    if (players[i]->isAlive()) {
                        players[i]->heal(0.01 * healPercent * user.getMaxHp());
                    }
                }
                break;
            default:
                break;
            }
        }

        // 技能有復活
        if (isRevive == REVIVE) {
            int reviveTarget = 0;
            do
            {
                reviveTarget = rand() % players.size();
            } while (players[reviveTarget]->isAlive());
            players[reviveTarget]->setHp(players[reviveTarget]->getMaxHp());
            players[reviveTarget]->setMp(players[reviveTarget]->getMaxMp());
        }

        if (isSpecial == SPECIAL) {
            user.triggerClassSpecial(*this, targetIndex, monsters, players);
        }
    }

    void Skill::reduceCooldown() {
        if(currentCD > 0) currentCD--;
    }

    void Skill::showInfo() const
    {
        cout << "Skill name:" << name << endl;
        cout << "Mp Cost:" << mpCost << endl;
        cout << "Skill damage:" << damage << endl;
    }
}