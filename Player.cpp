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
    : name(theName), job("Unknown"), maxHp(theMaxHp), hp(theMaxHp), maxMp(theMaxMp), mp(theMaxMp), attackPower(theAttackPower), defense(theDefense) {}

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
    int Player::getAttackPower()
    {
        // 🎯 核心攔截：如果身上有牧師的增傷 Buff
        if (getEffectTurns(STRENGTH) > 0) {
            // 回傳放大後的動態攻擊力
            return static_cast<int>(attackPower * 1.3);
        }

        return attackPower;
    }
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
            mp -= cost;
            cout << name << " consumed " << cost << " MP! (remaining MP: " << mp << ")" << endl;
            return true;
        }
        else {
            cout << name << " insufficient MP! Cannot perform skill!" << endl;
            return false;
        }
    }

    // 1. Basic Attack(已調整)
    // 發動普攻，然後怪物受到傷害
    void Player::attack(int targetIndex, vector<Monster*>& monsters, vector<Player*>& players) {
        cout << name << " performs a basic attack!" << endl;
        monsters[targetIndex]->takeDamage(getAttackPower());
    }

    int Player::calculateFinalDamage(int rawDamage)
    {
        if(defense>0){
            rawDamage = round(rawDamage * (1.0 - ( 1.0 * defense / (defense + 1000) )));
        }

        return rawDamage;
    }

    // 2. Take Damage (受到傷害)(已調整)
    // 減少生命值，若沒有存活則彈出死亡播報
    void Player::takeDamage(int damage, vector<Monster*>& monsters) {
        damage = calculateFinalDamage(damage);
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

    // 3. Use Item (使用道具)(已調整)
    bool Player::useItem(int itemCode) {
        // 檢查道具代碼是否合法
        auto it = items.find(itemCode);
        if (it == items.end()) {
            cout << "No this item." << endl;
            return false;
        }

        Item theItem = items[itemCode];
        // 檢查道具是否能使用，以及數量是否>0
        if (!theItem.isAvailable()) {
            cout << "Failed to use this item." << endl;
            if (theItem.getQuantity() <= 0) {
                cout << "Not enough items in package." << endl;
            }
            return false;
        }

        // 使用道具
        cout << name << " uses an item: [" << theItem.getName() << "]!" << endl;
        theItem.use(*this);
        return true;
    }

    void Player::takeEffect(const EffectType& effectType, int effectTurns) {
        // 記錄狀態與對應的回合數
        StatusEffectList[effectType] = effectTurns;
        if (effectTurns > 0) {
            cout << "✨ " << name << " is now affected by status effect: " << getEffectName(effectType) << "!" << endl;
        }

    }

    

    // 4. Use Skill (使用技能)(已調整)
    // 實作技能使用 (加入耗血邏輯)
    bool Player::useSkill(int skillInput, int targetIndex, vector<Player*>& players, vector<Monster*>& monsters)
    {
        int skillNumber = skillInput - 1;
        if (skillNumber < 0 || skillNumber >= 3 || skillbox[skillNumber] == nullptr) {
            cout << "The skill does not exist." << endl;
            return false;
        }
        
        int mpRequired = skillbox[skillNumber]->getMpCost();
        if (!consumeMp(mpRequired)) {
            cout << name << " does not have enough MP!" << endl;
            return false;
        }

        if (skillbox[skillNumber]->getCurrentCD() > 0) {
            cout << "The skill is still in CD!" << endl;
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

    void Player::restoreMp(int amount) {
        mp += amount;
        if (mp > maxMp) {
            mp = maxMp; // 補魔不能超過最大魔力值
        }
        cout << "💠 " << name << " restored " << amount << " MP points. Current MP: " << mp << "/" << maxMp << endl;
    }

    int Player::getEffectTurns(const EffectType& effectType) const
    {
        // 使用 find 尋找關鍵字
        auto it = StatusEffectList.find(effectType);
        
        // 如果找到了，回傳它紀錄的剩餘回合數
        if (it != StatusEffectList.end()) {
            return it->second;
        }
        
        // 找不到代表沒有這個 Buff/Debuff，回傳 0
        return 0;
    }

    void Player::updateStatusEffects() {
        // 使用迭代器安全遍歷 Map
        for (auto it = StatusEffectList.begin(); it != StatusEffectList.end(); ) {
            // 防呆：如果原本就是 0 或是負數，直接剔除
            if (it->second <= 0) {
                it = StatusEffectList.erase(it);
                continue;
            }

            // 🎯 【這裡處理玩家每回合結束的特殊結算】
            // 例如：如果騎士沒有覆寫，也可以直接在這裡寫 PERSEVERANCE 的回血邏輯
            // 騎士判定自己有沒有堅毅不倒
            if (it->first == PERSEVERANCE && it->second == 1) {
                // 1. 計算已損生命值 (Missing HP)
                int missingHp = maxHp - hp;
                
                // 2. 計算 30% 的恢復量
                int healAmount = static_cast<int>(missingHp * 0.30);
                
                // 3. 實際補血（因為是用已損血量去算，絕不可能超過 MaxHP）
                hp += healAmount;
                
                cout << "💖 [Perseverance - Final Burst] As the protection fades, [" << name 
                    << "] converts pure willpower into vitality, restoring 30% of missing HP (+" << healAmount 
                    << " HP)! (Current HP: " << hp << "/" << maxHp << ")" << endl;
            }
            
            // 狀態回合數減 1
            it->second--;

            // 檢查減完後是否過期
            if (it->second == 0) {
                // 假設你有寫一個 getEffectName() 轉換 Enum 到字串，方便輸出
                cout << "✨ [" << name << "] 的 " << getEffectName(it->first) << " 效果結束了。\n";
                it = StatusEffectList.erase(it); // 刪除並自動指向下一個元素
            } else {
                ++it; // 沒過期，正常指針往下走
            }
        }
    }

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
        }
    }

    // 實作空殼的特殊觸發，讓子類別去覆寫
    void Player::triggerClassSpecial(Skill& theSkill, int targetIndex, vector<Monster*>& monsters, vector<Player*>& players) {
        // Default player does nothing special
    }
}
    
