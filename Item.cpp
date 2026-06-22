#include "BattleManager.h"
#include "Item.h"
#include "Player.h"
#include "Monster.h"
#include <iostream>
#include <vector>

using std::vector;
using std::cout;
using std::endl;

namespace RPG_Colaborate
{
    // 在 Item.cpp 中實作預設道具表
    Item Item::getStandardItems(int stdCode) {
        vector<Item> stdItems = {
            // 建構子參數對應：
            // (name, type, effectValue(比例/傷害), effectType, effectTurns, quantity, usableInBattle)
            
            // --- 恢復類 (effectValue 代表 % 數) ---
            {"Health Potion", "HEAL_HP", 50, NONEE, 0, 1, false},   // 回復 50% 最大 HP
            {"Mana Potion", "HEAL_MP", 50, NONEE, 0, 1, true},   // 回復 50% 最大 MP
            {"Revive Dew", "REVIVE", 100, NONEE, 0, 1, false},    // 復活並回復 100% HP

            // --- 脫戰強化類 (usableInBattle = false) ---
            {"Strength Potion", "BUFF", 0, FLOOR_STRENGTH, 1, 1, false}, 
            {"Swift Potion", "BUFF", 0, FREEACTION, 1, 1, false},     // 直接上1回合 FREEACTION
            {"Hand of Calamity", "BUFF", 0, FLOOR_CALAMITY, 1, 1, false},

            // --- 戰鬥強化類 (usableInBattle = true) ---
            {"Golden Bell", "BUFF", 0, GOLDEN_BELL, 1, 1, true},
            {"The Last Gasp", "BUFF", 0, LAST_GASP, 1, 1, true},

            // --- 攻擊類 (effectValue 代表純傷害) ---
            {"Scorching Sun", "ATTACK", 3500, NONEE, 0, 1, true},           // 純傷害，NONE 0回不播報
            {"Thor's Fury", "ATTACK", 15000, ELETRICSHOCK, 1, 1, true}   // 傷害 + 觸電 1回
        };

        return stdItems[stdCode];
    }
    
    // 建構子 (新增了 effectType 與 effectTurns)
    Item::Item() : name("Empty"), type("None"), effectValue(0), statusEffectType(NONEE),
    statusEffectTurns(0), quantity(0), usableInBattle(true) {}
    
    Item::Item(string theName, string theType, int theEValue, EffectType theSEType, int theSETurns, int theQuantity, bool theUsableInBattle)
    : name(theName), type(theType), effectValue(theEValue), statusEffectType(theSEType),
    statusEffectTurns(theSETurns), quantity(theQuantity), usableInBattle(theUsableInBattle) {}

    Item::~Item() {}

    // --- Getters ---
    string Item::getName() const { return name; }
    string Item::getType() const { return type; }
    int Item::getEffectValue() const { return effectValue; }
    EffectType Item::getStatusEffectType() const { return statusEffectType; }
    int Item::getStatusEffectTurns() const { return statusEffectTurns; }
    int Item::getQuantity() const { return quantity; }
    bool Item::getUsableInBattle() const { return usableInBattle; }

    // --- Setters ---
    void Item::setName(const string& newName) { name = newName; }
    void Item::setType(const string& newType) { type = newType; }
    void Item::setEffectValue(int newValue) { effectValue = newValue; }
    void Item::setStatusEffectType(EffectType newType) { statusEffectType = newType; }
    void Item::setStatusEffectTurns(int newTurns) { statusEffectTurns = newTurns; }
    void Item::setUsableInBattle(bool newValue) { usableInBattle = newValue; }
    void Item::setQuantity(int newQuantity) { 
        quantity = newQuantity; 
        if (quantity < 0) quantity = 0; // Safety guard to prevent negative stock
    }

    // --- 核心：使用道具 (完全對齊 Skill 泛型邏輯) ---
    void Item::use(Player& user, vector<Player*>& players, vector<Monster*>& monsters) {
        if (quantity <= 0) {
            cout << "❌ [" << name << "] is out of stock!\n";
            return;
        }

        // --- 1. 復活 (特例：對活人無效) ---
        if (type == "REVIVE") {
            if (user.isAlive()) {
                cout << "✨ " << user.getName() << " is still alive and does not need Revive Dew!\n";
                return; // 不扣數量直接中斷
            }
            user.reviveWithHp(user.getMaxHp() * (effectValue / 100.0));
            cout << "🌟 " << user.getName() << " has been revived with " << effectValue << "% HP!\n";
        }
        else {
            // 死人不能喝其他藥水
            if (!user.isAlive()) {
                cout << "💀 " << user.getName() << " is defeated and cannot use items!\n";
                return;
            }
        }

        // 成功發動，扣除數量
        quantity--;

        // --- 2. 回血 (百分比) ---
        if (type == "HEAL_HP") {
            int healAmount = user.getMaxHp() * (effectValue / 100.0);
            user.heal(healAmount);
        } 
        // --- 3. 回魔 (百分比) ---
        else if (type == "HEAL_MP") {
            int restoreAmount = user.getMaxMp() * (effectValue / 100.0);
            int newMp = user.getMp() + restoreAmount;
            if (newMp > user.getMaxMp()) newMp = user.getMaxMp();
            user.setMp(newMp);
            cout << "💧 " << user.getName() << " restored " << restoreAmount << " MP!\n";
        }
        // --- 4. 強化 (上狀態) ---
        else if (type == "BUFF") {
            user.takeEffect(statusEffectType, statusEffectTurns);
            cout << "✨ " << user.getName() << " used " << name << "!\n";
        }
        // --- 5. 攻擊 (AOE 傷害 + 狀態) ---
        else if (type == "ATTACK") {
            if (monsters.empty()) {
                cout << "⚠️ No enemies available to attack!\n";
                quantity++;
                return;
            }
            cout << "⚔️ " << user.getName() << " unleashed the power of " << name << "!\n";
            for (auto m : monsters) {
                if (m != nullptr && m->isAlive()) {
                    // 造成純傷害
                    m->takeDamage(effectValue);
                    
                    // 施加狀態 (如果是 0 回合的 NONEE，你原本的 takeEffect 內部會自然過濾掉播報)
                    if (m->isAlive()) {
                        m->takeEffect(statusEffectType, statusEffectTurns);
                    }
                }
            }
        }
    }

    // 顯示道具資訊
    void Item::showInfo() const {
        cout << "--- " << name << " ---" << endl;
        cout << "Type: " << type << endl;
        cout << "Effect Value: " << effectValue << (type == "HEAL_HP" || type == "HEAL_MP" ? "%" : " pts") << endl;
        cout << "Quantity: " << quantity << endl;
        cout << "Usable in Battle: " << (usableInBattle ? "Yes" : "No") << endl;
        cout << "-----------------" << endl;
    }

    // --- Check if Item is Available (Includes Battle Context Check) ---
    bool Item::isAvailable(bool inBattle) const {
        // 1. Stock check
        if (quantity <= 0) {
            return false;
        }
        
        // 2. Battle context check
        if (inBattle) {
            // In battle: Item must be explicitly allowed in battle (usableInBattle == true)
            return usableInBattle;
        } else {
            // Out of battle: 
            // - Buff items with usableInBattle = false are allowed (e.g., Strength Potion)
            // - Standard recovery items (usableInBattle = true) are also allowed (e.g., Health Potion)
            // - BUT offensive items (type == "ATTACK") are strictly forbidden out of battle
            if (type == "ATTACK") {
                return false;
            }
            return true;
        }
    }
}