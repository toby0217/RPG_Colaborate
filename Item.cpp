#include "Item.h"
#include <iostream>

namespace RPG_Colaborate
{
    // 建構子 (Constructor)
    // 初始化清單 (Initializer list) 
    Item::Item(std::string name, std::string type, int effectValue, int quantity)
        : name(name), type(type), effectValue(effectValue), quantity(quantity) {}

    // 解構子 (Destructor)
    Item::~Item() {}

    // --- Getters ---
    std::string Item::getName() const { return name; }//道具名稱
    std::string Item::getType() const { return type; }//道具類型(player.cpp裡預設類型有heal 那是不是可以再分出healmp 跟healhp)
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

    
    
    // 使用道具：數量減一
    void Item::use() {
        if (quantity > 0) {
            quantity--;
            std::cout << "[" << name << "] used! Remaining quantity: " << quantity << std::endl;
        } else {
            std::cout << "Cannot use [" << name << "]. Out of stock!" << std::endl;
        }
    }

    // 顯示道具資訊
    void Item::showInfo() const {
        std::cout << "--- Item Info ---" << std::endl;
        std::cout << "Name: " << name << std::endl;
        std::cout << "Type: " << type << std::endl;
        std::cout << "Effect Value: " << effectValue << std::endl;
        std::cout << "Quantity: " << quantity << std::endl;
        std::cout << "-----------------" << std::endl;
    }

    // 檢查道具是否可用（數量大於 0）
    bool Item::isAvailable() const {
        return quantity > 0;
    }
}
