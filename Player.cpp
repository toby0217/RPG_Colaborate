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
    : name("Player"), job("Unknown"), maxHp(1000), hp(maxHp), maxMp(100), mp(maxMp),
    attackPower(100), defense(0), ownedGolds(0) {}
    Player::Player(string theName, int theMaxHp, int theMaxMp, int theAttackPower, int theDefense) 
    : name(theName), job("Unknown"), maxHp(theMaxHp), hp(theMaxHp), maxMp(theMaxMp), mp(theMaxMp),
    attackPower(theAttackPower), defense(theDefense), ownedGolds(0) {}

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
    int Player::getAttackPower() const
    {
        double strengthRate = 1;
        // 🎯 核心攔截：如果身上有牧師的增傷 Buff
        if (getEffectTurns(STRENGTH) > 0) {
            strengthRate += 0.3;
        }
        if (getEffectTurns(FLOOR_STRENGTH) > 0) {
            strengthRate += 0.5;
        }

        return static_cast<int>(attackPower * strengthRate);
    }
    int Player::getMp() const { return mp; }
    int Player::getMaxMp() const { return maxMp; }
    int Player::getDefense() const { return defense; }
    int Player::getOwnedGolds() const { return ownedGolds; }

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
    void Player::setOwnedGolds(int newOwnedGolds) { ownedGolds == newOwnedGolds; }

    void Player::addBountyGold(int gold) {
        ownedGolds += gold;
    }

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

    int Player::calculateFinalCritRate(int baseRate)
    {
        int finalRate = baseRate;
        if (getEffectTurns(FLOOR_CALAMITY) > 0) {
            finalRate += 50;
        }
        if (getEffectTurns(CONTSHOOT) > 0) {
            finalRate += 30;
        }
        return finalRate;
    }

    int Player::calculateFinalCritEffect(double baseEffect)
    {
        int finalEffect = baseEffect;
        if (getEffectTurns(FLOOR_CALAMITY) > 0) {
            finalEffect += 200;
        }
        return finalEffect;
    }

    // 1. Basic Attack(已調整)
    // 發動普攻，然後怪物受到傷害
    void Player::attack(int targetIndex, vector<Monster*>& monsters, vector<Player*>& players) {
        int damage = getAttackPower();
        if (getEffectTurns(LAST_GASP) > 0) {
            damage *= 4;
            takeEffect(LAST_GASP, 0); // ⚡ The moment the attack is unleashed, the status is immediately cleared!
            cout << "🩸 The Last Gasp effect has been released with the attack, status removed.\n";
        }
        cout << name << " performs a basic attack!" << endl;
        monsters[targetIndex]->takeDamage(damage);
    }

    // 根據防禦計算受傷
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
        // 🛡️ 金鐘罩優先級最高，直接攔截
        if (getEffectTurns(GOLDEN_BELL) > 0) {
            cout << "🛡️ Golden Bell activated! Immune to all damage!\n";
            damage = 0;
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

    void Player::showInventory() const {
        cout << "\n--- [" << name << "'s Inventory] ---\n";
        
        if (items.empty()) {
            cout << "  (Your backpack is completely empty...)\n";
            return;
        }

        // 格式化輸出表頭
        cout << "Slot | Item Name            | Qty  | Restriction\n";
        cout << "-----+----------------------+------+----------------\n";

        // 遍歷 map 裡的每一個格子
        for (const auto& pair : items) {
            int slot = pair.first;
            const Item& item = pair.second;

            // 使用 printf 或是 cout 進行對齊，這裡示範簡單的 printf 讓排版漂亮
            // %-4d (左對齊4格), %-20s (左對齊20格), %-4d (左對齊4格)
            printf("[%2d] | %-20s | %3d  | ", slot, item.getName().c_str(), item.getQuantity());
            
            if (item.getUsableInBattle()) {
                cout << "[Battle Only]\n";
            } else {
                cout << "[Pre-Battle Only]\n";
            }
        }
        cout << "------------------------------------------------\n";
    }

    void Player::obtainItem(const Item& baseItem) {
        // 假設你的背包上限是 20 格 (可以自由修改這個上限)
        const int MAX_INVENTORY_SLOTS = 20; 
        
        int firstEmptySlot = -1;
        bool foundDuplicate = false;

        // 🔍 第一步：遍歷所有可能的格子，尋找「有沒有重名」
        for (int slot = 0; slot < MAX_INVENTORY_SLOTS; slot++) {
            auto it = items.find(slot);
            
            if (it != items.end()) {
                // 如果這格有道具，檢查名字是否相同
                if (it->second.getName() == baseItem.getName()) {
                    // 🎯 碰到了重名！立刻啟動 quantity++ 並結束
                    int currentQty = it->second.getQuantity();
                    it->second.setQuantity(currentQty + 1);
                    
                    cout << "🎒 [Inventory] [" << name << "] stacked [" << baseItem.getName() 
                        << "] into Slot " << slot << ". (Total: " << it->second.getQuantity() << ")\n";
                    foundDuplicate = true;
                    break;
                }
            } else {
                // 如果這格是空的，且我們之前還沒記錄過空位，就記下第一個找到的空位
                if (firstEmptySlot == -1) {
                    firstEmptySlot = slot;
                }
            }
        }

        // 🔍 第二步：如果沒重名，嘗試塞入第一個空位
        if (!foundDuplicate) {
            if (firstEmptySlot != -1) {
                // 🎯 找到空白格子，塞入該道具，並確保數量為 1
                items[firstEmptySlot] = baseItem;
                items[firstEmptySlot].setQuantity(1);
                
                cout << "🎉 [Inventory] [" << name << "] placed [" << baseItem.getName() 
                    << "] into a new Slot " << firstEmptySlot << ".\n";
            } else {
                // 背包滿了的防呆處理
                cout << "❌ [Inventory] [" << name << "]'s inventory is FULL! Cannot pick up [" 
                    << baseItem.getName() << "].\n";
            }
        }
    }

    // 3. Use Item (戰鬥中使用道具，已修改為支援怪物全體並修復數量Bug)
    bool Player::useItem(int itemCode, vector<Player*>& players, vector<Monster*>& monsters) {
        auto it = items.find(itemCode);
        if (it == items.end()) {
            cout << "This item is not in the inventory.\n";
            return false;
        }

        Item& theItem = items[itemCode];

        if (!theItem.isAvailable(true)) {
            if (theItem.getQuantity() == 0) {
                cout << "❌ [" << name << "] is out of stock!\n";
                return false;
            }
            if (!theItem.getUsableInBattle()) {
                cout << " [" << theItem.getName() << "] can only be used after battle (out of combat)!\n";
                return false;
            }
        }

        cout << name << " use item: [" << theItem.getName() << "]!" << endl;
        theItem.use(*this, players, monsters); 
        return true;
    }

    bool Player::useOutOfCombatItem(int itemCode, vector<Player*>& players, vector<Monster*>& monsters)
    {
        auto it = items.find(itemCode);
        if (it == items.end()) {
            cout << "This item is not in the inventory.\n";
            return false;
        }

        Item& theItem = items[itemCode];

        if (!theItem.isAvailable(true)) {
            if (theItem.getQuantity() == 0) {
                cout << "❌ [" << name << "] is out of stock!\n";
                return false;
            }
            if (theItem.getUsableInBattle()) {
                cout << " [" << theItem.getName() << "] can only be used during battle!\n";
                return false;
            }
        }

        cout << name << " use item: [" << theItem.getName() << "]!" << endl;
        theItem.use(*this, players, monsters); 
        return true;
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
        
        if (skillbox[skillNumber]->getCurrentCD() > 0) {
            cout << "The skill is still in CD!" << endl;
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

    void Player::restoreMp(int amount) {
        mp += amount;
        if (mp > maxMp) {
            mp = maxMp; // 補魔不能超過最大魔力值
        }
        cout << "💠 " << name << " restored " << amount << " MP points. Current MP: " << mp << "/" << maxMp << endl;
    }

    void Player::takeEffect(const EffectType& effectType, int effectTurns) {
        StatusEffectList[effectType] = effectTurns;

        cout << "✨ "<< name <<" is now affected by status effect: " << getEffectName(effectType) << "!" << endl;
        // --- Special Status Activation Logic ---
        if (effectType == LAST_GASP && effectTurns > 0) {
            hp = 1; // Force current HP to 1 (or use setHp(1) if encapsulated)
            cout << "🩸 The Last Gasp unleashed! " << name << "'s HP drops to 1, awakening forbidden potential!\n";
        }
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

    void Player::updateStatusEffects(vector<Monster*>& monsters) {
        // 使用迭代器安全遍歷 Map
        for (auto it = StatusEffectList.begin(); it != StatusEffectList.end() && it->first < FLOOR_STRENGTH; ) {
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

            if (it->first == BURN && it->second > 0) {
                cout << "💥 " << name << " is burning! [BURN]" << endl;
                takeDamage(static_cast<int>(maxHp * 0.05), monsters);
            }
            
            // 狀態回合數減 1
            it->second--;

            // 檢查減完後是否過期
            if (it->second == 0) {
                // 假設你有寫一個 getEffectName() 轉換 Enum 到字串，方便輸出
                cout << "✨ [" << name << "]'s " << getEffectName(it->first) << " effect has ended.\n";
                it = StatusEffectList.erase(it); // 刪除並自動指向下一個元素
            } else {
                ++it; // 沒過期，正常指針往下走
            }
        }
    }

    void Player::reduceCooldowns()
    {
        for (int i = 0; i < 3; i++) {
            skillbox[i]->reduceCooldown();
        }
    }

    // 1. 獲取三個技能的當前冷卻時間 (供 UI 渲染使用)
    int Player::getSkillCooldown(int skillIdx) const {
        // skillIdx 傳入 0, 1, 2 分別對應技能 1, 2, 3
        if (skillIdx >= 0 && skillIdx < 3 && skillbox[skillIdx] != nullptr) {
            return skillbox[skillIdx]->getCurrentCD();
        }
        return 0;
    }

    void Player::reviveWithHp(int reviveHp) {
        // 安全檢查：通常只有死掉的人可以被復活
        if (this->hp <= 0) {
            this->hp = reviveHp;
            
            // 確保復活後的血量不會超過最大血量上限
            if (this->hp > this->maxHp) {
                this->hp = this->maxHp;
            }
            
            cout << name << " come back from Underworld! Revived and healed " << this->hp << " HP points" << endl;
        } else {
            cout << name << " still alive, no need to revive!" << endl;
        }
    }

    // 實作空殼的特殊觸發，讓子類別去覆寫
    void Player::triggerClassSpecial(Skill& theSkill, int targetIndex, vector<Monster*>& monsters, vector<Player*>& players) {
        // Default player does nothing special
    }
}
    
