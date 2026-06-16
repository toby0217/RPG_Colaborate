#ifndef ITEM_H
#define ITEM_H

#include <string>

namespace RPG_Colaborate
{
    class Item
    {
    private:
        std::string name;
        std::string type;
        int effectValue;
        int quantity;

    public:
        // 建構子
        Item(std::string name, std::string type, int effectValue, int quantity);
        ~Item();

        // Getters
        std::string getName() const;
        std::string getType() const;
        int getEffectValue() const;
        int getQuantity() const;

        // Setters
        void setName(const std::string& newName);
        void setType(const std::string& newType);
        void setEffectValue(int newValue);
        void setQuantity(int newQuantity);

        // 使用是大概這樣.....吧
        void use();
        void showInfo() const;
        bool isAvailable() const;
    };
}

#endif
