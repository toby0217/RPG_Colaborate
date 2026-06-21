#include "BattleManager.h"
#include "Player.h"
#include "Item.h"  
#include "Skill.h"
#include "Monster.h"
#include <iostream>
#include <cmath>
using std::cout;
using std::endl;

namespace RPG_Colaborate
{
    // Constructors
    Player::Player()
    : name("Player"), job("Unknown"), maxHp(1000), hp(maxHp), maxMp(100), mp(maxMp), attackPower(100) {}
    Player::Player(string theName, int theMaxHp, int theMaxMp, int theAttackPower, int theDefense) 
    : name(theName), job("Unknown"), maxHp(maxHp), hp(maxHp), maxMp(maxMp), mp(maxMp), attackPower(attackPower) {}

    // Destructor
    Player::~Player() {
    
        for (int i = 0; i < 3; ++i) {
            if (skillbox[i] != nullptr) {
                delete skillbox[i]; // 💡 確保每一招 new 出來的技能都有被釋放！
                skillbox[i] = nullptr;
            }
        }
    }

    //getters
    string Player::getName() const { return name; }
    string Player::getJob() const { return job; }
    int Player::getHp() const { return hp; }
    int Player::getMaxHp() const { return maxHp; }
    int Player::getAttackPower() const { return attackPower; }
    int Player::getMp() const { return mp; }
    int Player::getMaxMp() const { return maxMp; }
    int Player::getDefense() const { return defense; }

    //setters
    void Player::setName(string newName) { name = newName; }
    void Player::setHp(int newHp)
    {
        hp = newHp;
        if (hp < 0) hp = 0; // Prevent negative HP
    }
    void Player::setMaxHp(int newMaxHp) { maxHp = newMaxHp; }
    void Player::setAttackPower(int newAttackPower) { attackPower = newAttackPower; }
    void Player::setMp(int newMp) { mp = newMp; }
    void Player::setMaxMp(int newMaxMp) { maxMp = newMaxMp; }
    void Player::setDefense(int newDefense) { defense = newDefense; }

    bool Player::consumeMp(int cost) {
        if (mp >= cost) {
            //mp -= cost;
            //std::cout << name << " consumed " << cost << " MP! (remaining MP: " << mp << ")\n";
            return true;
        }
        else {
            cout << name << " insufficient MP! Cannot perform skill!" << endl;
            return false;
        }
    }

    // 1. Basic Attack(已調整)
    // 發動普攻，然後怪物受到傷害
    void Player::attack(int targetIndex, vector<Monster*> monsters, vector<Player*> players) {
        cout << name << " performs a basic attack!" << endl;
        monsters[targetIndex]->takeDamage(attackPower);
    }

    // 2. Take Damage (受到傷害)(已調整)
    // 減少生命值，若沒有存活則彈出死亡播報
    void Player::takeDamage(int damage) {
        
        // ✨ 金鐘罩無敵判定
        if (invincibleTurns > 0) {
            cout << "🛡️ 金鐘罩發揮作用！" << name << " 免疫了這次傷害！\n";
            return;
        }
        
        if(defense>0){
            damage = round(damage * (1 - ( defense / (defense + 100) )));
        }
        hp -= damage;
        if (hp < 0) {
            hp = 0; // Prevent HP from dropping below zero
        }
        cout << name << " takes " << damage << " points of damage! " 
                << "(Current HP: " << hp << "/" << maxHp << ")" << endl;
                
        if (!isAlive()) {
            cout << name << " has been defeated..." << endl;
        }
    }

    // 3. Use Item (戰鬥中使用道具，已修改為支援怪物全體並修復數量Bug)
    bool Player::useItem(int itemCode, vector<Monster*>& monsters) {
        auto it = items.find(itemCode);
        if (it == items.end()) {
            cout << "背包裡沒有這個道具。\n";
            return false;
        }

        Item& theItem = items[itemCode];

        if (!theItem.getUsableInBattle()) {
            cout << " [" << theItem.getName() << "] 只能在戰鬥結束後（脫戰）使用！\n";
            return false;
        }

        if (!theItem.isAvailable()) {
            cout << "道具數量不足。\n";
            return false;
        }

        cout << name << " 在戰鬥中使用了道具: [" << theItem.getName() << "]!" << endl;
        theItem.use(*this, &monsters); 
        return true;
    }

    // 新增：脫戰專用道具
    bool Player::useItemOutOfBattle(int itemCode, Player& target) {
        auto it = items.find(itemCode);
        if (it == items.end()) {
            cout << "背包裡沒有這個道具。\n";
            return false;
        }

        Item& theItem = items[itemCode];

        if (!theItem.isAvailable()) {
            cout << "道具數量不足。\n";
            return false;
        }

        cout << "🏕️ 脫戰休整：" << name << " 對 " << target.getName() << " 使用了 [" << theItem.getName() << "]！" << endl;
        theItem.use(target, nullptr);
        return true;
    }
    void Player::takeEffect(EffectType& effectType, int effectTurns) {
        // 記錄狀態與對應的回合數
        StatusEffectList[effectType] = effectTurns;
        cout << "✨ " << name << " is now affected by status effect!" << endl;
    }

    // 實作空殼的特殊觸發，讓子類別去覆寫
    void Player::triggerClassSpecial(EffectType& type) {
        // Default player does nothing special
    }

    // 4. Use Skill (使用技能)(已調整)
    // 實作技能使用 (加入耗血邏輯)
    bool Player::useSkill(int skillNumber, int targetIndex, vector<Player*> players, vector<Monster*> monsters)
    {
        if (skillNumber < 0 || skillNumber >= 3 || skillbox[skillNumber] == nullptr) {
            cout << "The skill does not exist." << endl;
            return false;
        }
        
        int mpRequired = skillbox[skillNumber]->getMpCost();
        if (!consumeMp(mpRequired)) {
            cout << name << " does not have enough MP!" << endl;
            return false;
        }

        // 技能施放成功，開始計算 HpCost
        int hpCostPercent = skillbox[skillNumber]->getHpCost();
        if (hpCostPercent > 0) {
            int hpDeduction = (maxHp * hpCostPercent) / 100;

            if (hpDeduction >= hp) {
                hp = 1;// 確保不會因為耗血而死
            }
            else {
                hp -= hpDeduction;
            }
            cout << "🩸 " << name << " sacrifices " << hpDeduction << " HP to cast the skill! (Remaining HP: " << hp << ")" << endl;
        }

        cout << name << " casts a skill: [" << skillbox[skillNumber]->getName() << "]!" << endl;

        // 交給 Skill 物件去執行具體邏輯
        skillbox[skillNumber]->use(*this, targetIndex, players, monsters);
        return true;
    }

    // 5. isAlive (確認存活)(已調整)
    bool Player::isAlive()
    {
        return hp > 0;
    }

    void Player::heal(int amount) {
        hp += amount;
        if (hp > maxHp) {
            hp = maxHp; // 補血不能超過最大生命值
        }
        cout << "💚 " << name << " healed " << amount << " HP points! " << "(Current HP: " << hp << "/" << maxHp << ")" << endl;
    }

    //void Player::restoreMp(int amount) {
    //    mp += amount;
    //    if (mp > maxMp) {
    //        mp = maxMp; // 補魔不能超過最大魔力值
    //    }
    //    std::cout << "💙 " << name << " healed " << amount << " MP points. Current MP: " << mp << "/" << maxMp << "\n";
    //}


    void Player::reviveWithHp(int reviveHp) {
        // 安全檢查：通常只有死掉的人可以被復活
        if (this->hp <= 0) {
            this->hp = reviveHp;
            
            // 確保復活後的血量不會超過最大血量上限
            if (this->hp > this->maxHp) {
                this->hp = this->maxHp;
            }
            
            std::cout << name << " come back from Underworld! Revived and healed " << this->hp << " HP points" << std::endl;
        } else {
            std::cout << name << " still alive, no need to revive!" << std::endl;

            // --- 道具Buff相關實作 ---
    void Player::applyStrengthBuff(int amount) {
        tempAtkBonus += amount;
        attackPower += amount;
        cout << "💪 " << name << " 喝下力量藥水，下場戰鬥攻擊力提升 " << amount << "！\n";
    }

    void Player::applySwiftBuff() {
        hasSwiftBuff = true;
        cout << "⚡ " << name << " 喝下迅捷藥水，下場戰鬥第一回合將行動兩次！\n";
    }

    bool Player::consumeSwiftBuff() {
        if (hasSwiftBuff) {
            hasSwiftBuff = false;
            return true;
        }
        return false;
    }

    void Player::applyGoldenBell() {
        invincibleTurns = 1;
        cout << "🛡️ " << name << " 啟動金鐘罩！直到下次行動前免疫所有傷害！\n";
    }

    void Player::decrementInvincibleTurns() {
        if (invincibleTurns > 0) {
            invincibleTurns--;
            if (invincibleTurns == 0) cout << "🛡️ " << name << " 的金鐘罩效果結束了。\n";
        }
    }

    void Player::applyLastGasp() {
        hp = 1;
        attackPower *= 4; // 原本 100% + 額外 300% = 400%
        hasLastGasp = true;
        cout << "🩸 " << name << " 發動絕唱！生命值降至 1，下一次攻擊/技能傷害暴增 300%！\n";
    }

    bool Player::checkAndConsumeLastGasp() {
        if (hasLastGasp) {
            hasLastGasp = false;
            return true;
        }
        return false;
    }

    void Player::applyCalamityBuff() {
        tempCritRateBonus += 50;
        tempCritEffectBonus += 200;
        cout << "🔥 " << name << " 觸發災厄之手！下場戰鬥暴擊率+50%，暴擊傷害+200%！\n";
    }

    int Player::getTempCritRateBonus() const { return tempCritRateBonus; }
    int Player::getTempCritEffectBonus() const { return tempCritEffectBonus; }

    void Player::clearBattleBuffs() {
        if (tempAtkBonus > 0) {
            attackPower -= tempAtkBonus;
            tempAtkBonus = 0;
        }
        tempCritRateBonus = 0;
        tempCritEffectBonus = 0;
        hasSwiftBuff = false;
        hasLastGasp = false;
        invincibleTurns = 0;
        }
    }
}
    
