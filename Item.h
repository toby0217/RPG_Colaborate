#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <vector>
using std::string;
using std::vector;

namespace RPG_Colaborate
{
    class Player;
    class Monster;

    class Item
    {
    private:
        string name;
        string type;
        int effectValue;
        int quantity;
        bool usableInBattle; // 新增：是否可在戰鬥中使用

    public:
        // 建構子 (預設可用於戰鬥)
        Item();
        Item(string name, string type, int effectValue, int quantity, bool usableInBattle = true);
        ~Item();

        // Getters
        string getName() const;
        string getType() const;
        int getEffectValue() const;
        int getQuantity() const;
        bool getUsableInBattle() const; // 新增的 Getter

        // Setters
        void setName(const string& newName);
        void setType(const string& newType);
        void setEffectValue(int newValue);
        void setQuantity(int newQuantity);

        // 使用道具 (參數改為 target，方便對隊友使用) 加入 monsters 指標（預設為 nullptr，讓脫戰使用時不會報錯
        void use(Player& target,  vector<Monster*>* monsters = nullptr);
        void showInfo() const;
        bool isAvailable() const;
    };
}

#endif
