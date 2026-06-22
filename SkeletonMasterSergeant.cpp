#include "SkeletonMasterSergeant.h"
#include "Monster.h"
#include"Player.h"
#include<vector>
#include<iostream>

using std::vector;
using std::cout;
using std::endl;

namespace RPG_Colaborate{
    SkeletonMasterSergeant::SkeletonMasterSergeant():
    Monster("Skeleton Master Sergeant",100,10,10,0,0,MonsterRank::ELITE){}

    SkeletonMasterSergeant::SkeletonMasterSergeant(const string& name,int hp,int attackPower,int rewardGold):
    Monster(name,hp,attackPower,rewardGold,20,0,MonsterRank::ELITE){}
    //不用填閃避、護甲和等級的建構子，也許有用，沒用可以直接刪了

    SkeletonMasterSergeant::SkeletonMasterSergeant(const string& name,int hp,int attackPower,int rewardGold,int evadeRate,int defense, MonsterRank rank):
    Monster(name,hp,attackPower,rewardGold,evadeRate,defense,rank){}
    //完整建構子

    void SkeletonMasterSergeant::entryAction(vector<Player*>& players, vector<Monster*>& monsters)
    {
        summomSkeleton(monsters);
        summomSkeleton(monsters);
    }


    //我嘗試寫寫看，我不確定對不對
    void SkeletonMasterSergeant::summomSkeleton(vector<Monster*>& monsters){
        //按照從中間開始左右左右的方式檢測有沒有活著的怪，沒有就召一個新的
        if(!monsters[1]||!monsters[1]->isAlive()){

            if(monsters[1]){
                delete monsters[1];
            }//如果有怪的屍體，刪了召新的

            monsters[1] = new Monster("Skeleton", 1665, 1419, 0, 0, 243, NORMAL);//召的怪不給錢
            cout<<name<<" summom skeleton"<<endl;

        }

        else if(!monsters[3]){
            
            if(monsters[3]){
                delete monsters[3];
            }

            monsters[3] = new Monster("Skeleton", 1665, 1419, 0, 0, 243, NORMAL);
            cout<<name<<" summom skeleton"<<endl;

        }

        else if(!monsters[0]){

            if(monsters[0]){
                delete monsters[0];
            }

            monsters[0] = new Monster("Skeleton", 1665, 1419, 0, 0, 243, NORMAL);
            cout<<name<<" summom skeleton"<<endl;

        }

        else if(!monsters[4]){

            if(monsters[4]){
                delete monsters[4];
            }

            monsters[4] = new Monster("Skeleton", 1665, 1419, 0, 0, 243, NORMAL);
            cout<<name<<" summom skeleton"<<endl;

        }
        else{
            cout<<"Battle field full, cann't summon skeleton"<<endl;
        }
    }

    void SkeletonMasterSergeant::attack(int targetIndex, vector<Player*>& players, vector<Monster*>& monsters)
    {
        turnCounter++;
        if (turnCounter % 2 == 0) {
            summomSkeleton(monsters);
        }
    }

    
    }
