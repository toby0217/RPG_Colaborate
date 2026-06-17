#include "Skill.h"
#include <iostream>
#include <string>
namespace RPG_Colaborate
{
    //建構子
    Skill::Skill(std::string name, int damage, int mpCost):
    name(name),damage(damage),mpCost(mpCost){}

    Skill::~Skill(){}

    //getter
    std::string Skill::getName()const{
        return name;
    }
    int Skill::getDamage()const{
        return damage;
    }
    int Skill::getMpCost()const{
        return mpCost;
    }

    //setter
    void Skill::setName(const std::string& newName){
        name = newName;
    }
    void Skill::setDamage(int newDamage){
        damage = newDamage;
    }
    void Skill::setMpCost(int newMpCost){
        mpCost = newMpCost;
    }

    //function
    void Skill::use(){
        std::cout<<"Use "<<name<<" cost "<<mpCost<<" MP"<<std::endl;
    }

    void Skill::showInfo()const{
        std::cout<<"Skill name:"<<name<<std::endl;
        std::cout<<"Mp Cost:"<<mpCost<<std::endl;
        std::cout<<"Skill damage:"<<damage<<std::endl;
    }
}