#ifndef SKILL_H
#define SKILL_H

#include <string>
using std::string;

namespace RPG_Colaborate
{
    class Player;
    class Monster;
    
    class Skill
    {
    private:
        string name;
        string type;
        //技能的傷害應該會跟角色攻擊力有關 那這邊的damage是不是可以改成技能倍率之類的
        int damage;
        int mpCost;

    public:
        // 建構子
        Skill(string theName, string theType, int theDamage, int theMpCost);
        ~Skill();

        // Getters
        string getName() const;
        string getType() const;
        int getDamage() const;
        int getMpCost() const;

        // Setters
        void setName(string newName);
        void setType(string newType);
        void setDamage(int newDamage);
        void setMpCost(int newMpCost);

        // 用技能 嗯大概這樣
        void use(Player& user, Monster& target);
        void showInfo() const;
    };
}

#endif
