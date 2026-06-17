#include "Item.h"
#include <iostream>
using std::cout;
using std::endl;

namespace RPG_Colaborate
{
    // 建構子 (Constructor)
    // 初始化清單 (Initializer list) 
    Item::Item(string name, string type, int effectValue, int quantity)
        : name(name), type(type), effectValue(effectValue), quantity(quantity) {}

    // 解構子 (Destructor)
    Item::~Item() {}

    // --- Getters ---
    string Item::getName() const { return name; }//道具名稱
    string Item::getType() const { return type; }//道具類型(player.cpp裡預設類型有heal 那是不是可以再分出healmp 跟healhp)
    int Item::getEffectValue() const { return effectValue; }//道具效果值(像藥水的治療量之類的)
    int Item::getQuantity() const { return quantity; }//道具數量

    // --- Setters ---
    void Item::setName(const std::string& newName) { name = newName; }
    void Item::setType(const std::string& newType) { type = newType; }
    void Item::setEffectValue(int newValue) { effectValue = newValue; }
    void Item::setQuantity(int newQuantity) { 
        quantity = newQuantity; 
        if (quantity < 0) quantity = 0; // 避免數量變成負數
    }

    
    
    // 使用道具：數量減一(待新增道具再更新)
    void Item::use() {
        if (quantity > 0) {
            quantity--;
            cout << "[" << name << "] used! Remaining quantity: " << quantity << endl;
        } else {
            cout << "Cannot use [" << name << "]. Out of stock!" << endl;
        }

        // 根據道具類型來決定效果 (假設 "Heal" 類型是補血)
        //if (item.getType() == "Heal") {
        //    int healAmount = item.getEffectValue();
        //    hp += healAmount;
            
            // 確保補血不會超過最大血量
        //    if (hp > maxHp) {
        //        hp = maxHp;
        //    }
        //    std::cout << name << " recovers " << healAmount << " HP! "
        //            << "(Current HP: " << hp << "/" << maxHp << ")" << std::endl;
        //}

        // 呼叫道具本身的 use()，讓它自己去把 quantity (數量) 扣掉 1
    }

    // 顯示道具資訊
    void Item::showInfo() const {
        cout << "--- Item Info ---" << endl;
        cout << "Name: " << name << endl;
        cout << "Type: " << type << endl;
        cout << "Effect Value: " << effectValue << endl;
        cout << "Quantity: " << quantity << endl;
        cout << "-----------------" << endl;
    }

    // 檢查道具是否可用 (目前條件:數量大於0)
    bool Item::isAvailable() const {
        return quantity > 0;
    }
}
