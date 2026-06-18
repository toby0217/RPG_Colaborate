#include "Item.h"
#include "Player.h"
#include <iostream>
using std::cout;
using std::endl;

namespace RPG_Colaborate
{
    // 建構子 (Constructor)
    // 初始化清單 (Initializer list) 
    // 在 Item 類別中新增這個
    Item::Item() : name("Empty"), type("None"), effectValue(0), quantity(0) {}
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

    void Item::use(Player& user) {
        //若無道具，則無法使用
        if (quantity <= 0) {
            cout << "X " << name << " Insufficient amount, cannot use!\n";
            return;
        }

        cout << user.getName() << " is using item: " << name << endl;
        quantity--; // 扣除數量

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
        cout << "-----------------" << endl;
    }

    // 檢查道具是否可用 (目前條件:數量大於0)
    bool Item::isAvailable() const {
        return quantity > 0;
    }
}
