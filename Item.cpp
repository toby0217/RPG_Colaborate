#include "BattleManager.h"
#include "Item.h"
#include "Player.h"
#include <iostream>
using std::cout;
using std::endl;

namespace RPG_Colaborate
{
    // 建構子
    Item::Item() : name("Empty"), type("None"), effectValue(0), quantity(0), usableInBattle(true) {}
    
    Item::Item(string name, string type, int effectValue, int quantity, bool usableInBattle)
        : name(name), type(type), effectValue(effectValue), quantity(quantity), usableInBattle(usableInBattle) {}

    // 解構子
    Item::~Item() {}

    // --- Getters ---
    string Item::getName() const { return name; }//道具名稱
    string Item::getType() const { return type; }//道具類型(player.cpp裡預設類型有heal 那是不是可以再分出healmp 跟healhp)
    int Item::getEffectValue() const { return effectValue; }//道具效果值(像藥水的治療量之類的)
    int Item::getQuantity() const { return quantity; }//道具數量
    bool Item::getUsableInBattle() const { return usableInBattle; }//戰鬥中是否可用

    // --- Setters ---
    void Item::setName(const std::string& newName) { name = newName; }
    void Item::setType(const std::string& newType) { type = newType; }
    void Item::setEffectValue(int newValue) { effectValue = newValue; }
    void Item::setQuantity(int newQuantity) { 
        quantity = newQuantity; 
        if (quantity < 0) quantity = 0; // 避免數量變成負數
    }
    //這邊底下是新增的部分
    void Item::use(Player& target, vector<Monster*>* monsters) {
        if (quantity <= 0) {
            cout << "X [" << name << "] 數量不足，無法使用！\n";
            return;
        }

        // 蘇生之露特例：對活著的人無效
        if (type == "Revive" && target.isAlive()) {
            cout << target.getName() << " 還活蹦亂跳的，不需要使用蘇生之露！\n";
            return; // 不扣除數量直接中斷
        }

        quantity--; // 扣除數量

        // -- 原本的恢復道具 --
        if (type == "Heal_HP") {
            target.heal(effectValue); 
        } 
        else if (type == "Heal_MP") {
            int newMp = target.getMp() + effectValue;
            if (newMp > target.getMaxMp()) newMp = target.getMaxMp();
            target.setMp(newMp);
            cout  << target.getName() << " 回復了 " << effectValue << " 點 MP！\n";
        } 
        else if (type == "Revive") {
            target.reviveWithHp(effectValue);
        }
        // -- 新增：脫戰 Buff 道具 --
        else if (type == "Strength") {
            target.applyStrengthBuff(effectValue);
        }
        else if (type == "Swift") {
            target.applySwiftBuff();
        }
        else if (type == "Calamity") {
            target.applyCalamityBuff();
        }
        // --  新增：戰鬥中道具 --
        else if (type == "ScorchingSun") {
            if (monsters != nullptr) {
                cout << " 烈日凌空！熾熱的陽光灼燒所有敵人！\n";
                for (auto m : *monsters) {
                    if (m != nullptr && m->isAlive()) {
                        int missingHp = m->getMaxHp() - m->getHp();
                        if (missingHp > 0) {
                            m->takeDamage(missingHp);
                        } else {
                            cout << m->getName() << " 處於滿血狀態，毫髮無傷！\n";
                        }
                    }
                }
            } else {
                cout << "烈日凌空只能在戰鬥中使用！\n";
            }
        }
        else if (type == "GoldenBell") {
            target.applyGoldenBell();
        }
        else if (type == "LastGasp") {
            target.applyLastGasp();
        }
    }
        // 35備註:需要等確認所有Item型別才能繼續寫
        // 根據王懷賢、宋金日的規劃：分為回血與回魔 (image_1.png)
        // Use lowercase or specific strings to check both type and name for safety
    /*if (type == "Potion" || type == "HP" || name == "Potion" || name == "Health Potion" || name == "紅藥水") {
        user.heal(effectValue); 
    } 
    else if (type == "Ether" || type == "MP" || name == "Ether" || name == "Mana Potion" || name == "藍藥水") {
        user.restoreMp(effectValue); 
    } 
    else if (type == "StrPotion" || name == "str藥水" || name == "Str Potion") {
        // Wang Huaixian's Idea
        int currentAtk = user.getBaseAttack();
        user.setBaseAttack(currentAtk + effectValue);
        std::cout << "🔥 Attack power increased by " << effectValue << " points!" << std::endl;
    } 
    else if (type == "SwanSong" || name == "絕唱" || name == "Swan Song") {
        // Sun Yiming's Idea
        int damageToSelf = user.getHp() - 1;
        user.takeDamage(damageToSelf); 
        
        int currentAtk = user.getBaseAttack();
        user.setBaseAttack(currentAtk * 3); 
        
        std::cout << "🩸 [Swan Song] Activated! HP reduced to 1, Attack power boosted to 300%!!!" << std::endl;
    }*/
    }
    
    // 顯示道具資訊
    void Item::showInfo() const {
        cout << "--- Item Info ---" << endl;
        cout << "Name: " << name << endl;
        cout << "Type: " << type << endl;
        cout << "Effect Value: " << effectValue << endl;
        cout << "Quantity: " << quantity << endl;
        cout << "Usable in Battle: " << (usableInBattle ? "Yes" : "No") << endl;
        cout << "-----------------" << endl;
    }

    // 檢查道具是否可用 (目前條件:數量大於0)
    bool Item::isAvailable() const {
        return quantity > 0;
    }
}
