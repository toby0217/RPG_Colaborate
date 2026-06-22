#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <vector>
using std::string;
using std::vector;

namespace RPG_Colaborate
{
    class BattleManager;
    class Player;
    class Monster;

    class Item
    {
    private:
        string name;
        string type;
        EffectType statusEffectType;
        int statusEffectTurns;
        int effectValue;
        int quantity;
        bool usableInBattle; // 新增：是否可在戰鬥中使用

    public:
        // 建構子 (預設可用於戰鬥)
        Item();
        Item(string theName, string theType, int theEValue, EffectType theSEType, int theSETurns, int theQuantity, bool theUsableInBattle);
        ~Item();

        // Getters
        string getName() const;
        string getType() const;
        int getEffectValue() const;
        EffectType getStatusEffectType() const;
        int getStatusEffectTurns() const;
        int getQuantity() const;
        bool getUsableInBattle() const;

        vector<Item> getStandardItems();

        // Setters
        void setName(const string& newName);
        void setType(const string& newType);
        void setEffectValue(int newValue);
        void setStatusEffectType(EffectType newType);
        void setStatusEffectTurns(int newTurns);
        void setQuantity(int newQuantity);
        void setUsableInBattle(bool newValue);
        

        // 使用道具 (參數改為 target，方便對隊友使用) 加入 monsters 指標（預設為 nullptr，讓脫戰使用時不會報錯
        void use(Player& user, vector<Player*>& players, vector<Monster*>& monsters);
        void showInfo() const;
        bool isAvailable(bool inBattle) const;
    };
}

#endif
