#include "BattleManager.h"
#include "Player.h"
#include "Skill.h"
#include "Item.h"
#include "Monster.h"
#include <iostream>

using std::cin;
using std::cout;
using std::vector;

namespace RPG_Colaborate
{
    void BattleManager::renderUI()
    {
        cout << "=================================================================\n";
        for (int i = 0; i < players.size(); i++) {
            // 第一行：基本數值與鎖定箭頭
            cout << "[" << players[i]->getJob() << "] " << players[i]->getName() 
                << " HP:" << players[i]->getHp() << "/" << players[i]->getMaxHp() 
                << " MP:" << players[i]->getMp() << "/" << players[i]->getMaxMp();
            
            cout << "      "; // 中間排版空格
            
            if (i < monsters.size() && monsters[i]->isAlive()) {
                string arrow = (i == currentTargetIdx) ? "==> " : "    ";
                cout << arrow << monsters[i]->getName() 
                    << " HP:" << monsters[i]->getHp() << "/" << monsters[i]->getMaxHp();
            }
            cout << "\n";
            
            // 第二行：狀態顯示區 (正負面狀態分離)
            // 假設寫了 getBuffs() 和 getDebuffs()
            //cout << "Buff:[" << players[i]->getEffects() << "] Debuff:[" << players[i]->getEffects() << "]";
            
            cout << "               "; // 中間排版空格
            
            if (i < monsters.size() && monsters[i]->isAlive()) {
                cout << "Buff:[" << monsters[i]->getBuffs() << "] Debuff:[" << monsters[i]->getDebuffs() << "]";
            }
            cout << "\n-----------------------------------------------------------------\n";
        }
    }

    void BattleManager::playerTurn(Player& currentPlayer) {
        bool actionCompleted = false;
    
        do 
        {
            renderUI();

            char choice;
            cout << "Select Action: [A]Attack [1~3]Skill [I]Item [T]Change target\n> ";
            cin >> choice;

            if (choice == 'a' || choice == 'A') {
                currentPlayer.attack(currentTargetIdx, monsters, players);
                actionCompleted = true; // 普攻結束回合
            }
            else if (choice == '1' || choice == '2' || choice == '3') {
                int skillIdx = choice - '0';
                // 讓 useSkill 回傳 bool，如果是「紅色子彈」這種特殊技能就回傳 false
                actionCompleted = currentPlayer.useSkill(skillIdx, currentTargetIdx, players, monsters); 
            }
            else if (choice == 'i' || choice == 'I') {
                cout << "請輸入要使用的道具代碼: ";
                int itemCode;
                cin >> itemCode;
                currentPlayer.useItem(itemCode); // 道具庫是玩家私有的，直接呼叫
                actionCompleted = false; // 使用道具不消耗回合，繼續選！
            }
            else if (choice == 't' || choice == 'T') {
                do {
                    currentTargetIdx = (currentTargetIdx + 1) % monsters.size();
                } while (!monsters[currentTargetIdx]->isAlive()); // 確保鎖定活著的怪
                actionCompleted = false; // 切換鎖定不消耗回合
            }
            else {
                cout << "輸入錯誤！請重新輸入。\n";
                actionCompleted = false;
            }
        } while (!actionCompleted);
    }
}
