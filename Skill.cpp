#include "Skill.h"
#include "Player.h"
#include "Monster.h"
#include <iostream>
#include <string>
using std::cout;
using std::endl;

namespace RPG_Colaborate
{
    //constructors
    Skill::Skill(string name, int damage, int mpCost):
    name(name),damage(damage),mpCost(mpCost) {}

    Skill::~Skill(){}

    //getters
    string Skill::getName() const { return name; }
    int Skill::getDamage() const { return damage; }
    int Skill::getMpCost() const { return mpCost; }

    //setters
    void Skill::setName(const string& newName) { name = newName; }
    void Skill::setDamage(int newDamage) { damage = newDamage; }
    void Skill::setMpCost(int newMpCost) { mpCost = newMpCost; }

    //function
    // 使用技能(已調整)
    // 讓技能主導施法程序
    void Skill::use(Player& user, Monster& target)
    {
        // cout<<"Use \""<<name<<"\" cost "<<mpCost<<" MP"<<endl;
        
        cout << user.getName() << " casts a skill: [" << name << "]!" << endl;
            
        // 先做純傷害技能，之後再嘗試加別的類型
        // 直接使用技能傷害
        target.takeDamage(damage);
    }

    void Skill::showInfo() const
    {
        cout << "Skill name:" << name << endl;
        cout << "Mp Cost:" << mpCost << endl;
        cout << "Skill damage:" << damage << endl;
    }
}