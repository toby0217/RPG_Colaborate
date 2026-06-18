#ifndef ITEM_H
#define ITEM_H

#include <string>
using std::string;

namespace RPG_Colaborate
{
    class Player;

    class Item
    {
    private:
        string name;
        string type;
        int effectValue;
        int quantity;

    public:
        // 建構子
        Item();
        Item(string name, string type, int effectValue, int quantity);
        ~Item();

        // Getters
        string getName() const;
        string getType() const;
        int getEffectValue() const;
        int getQuantity() const;

        // Setters
        void setName(const string& newName);
        void setType(const string& newType);
        void setEffectValue(int newValue);
        void setQuantity(int newQuantity);

        // 使用道具之類的
        void use(Player& user);
        void showInfo() const;
        bool isAvailable() const;
    };
}

#endif