#include "BattleManager.h"
#include "Player.h"
#include "Skill.h"
#include "Item.h"
#include "Monster.h"
#include <iostream>
#include <cstdlib> // 用於 rand() 隨機索敵

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;

namespace RPG_Colaborate
{
    BattleManager::BattleManager(vector<Player*> AllPlayers, vector<Monster*> AllMonsters)
    : players(AllPlayers), monsters(AllMonsters) {}
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
            
            // 第二行：狀態顯示區
            cout << "Buff/Debuff: ";
            // 這裡可以呼叫你之後要實作的狀態顯示，先留空
            
            cout << "               "; // 中間排版空格
            
            if (i < monsters.size() && monsters[i]->isAlive()) {
                cout << "Buff:[" << monsters[i]->getBuffs() << "] Debuff:[" << monsters[i]->getDebuffs() << "]";
            }
            cout << "\n-----------------------------------------------------------------\n";
        }
    }

    // 核心主迴圈：控制整場戰鬥直到分出勝負
    void BattleManager::startBattle() {
        cout << "⚔️ 戰鬥開始！ ⚔️\n";
        
        while (isBattleOver() == 0) { // 假設 0:進行中, 1:玩家勝利, 2:怪物勝利
            cout << "\n=== ✨ 全新輪次開始 ===" << endl;

            // 1. 玩家行動階段 (Player Phase)
            for (int i = 0; i < players.size(); i++) {
                if (players[i]->isAlive()) {
                    // 控場檢查：如果被冰凍(FREEZE)，直接跳過該玩家
                    if (players[i]->getEffectTurns(FREEZE) > 0) {
                        cout << "❄️ [" << players[i]->getName() << "] 被冰凍了，無法行動！\n";
                        continue;
                    }
                    playerTurn(*players[i], i); // 傳入當前玩家與其索引
                }
                if (isBattleOver() != 0) break;
            }

            if (isBattleOver() != 0) break;

            // 2. 怪物行動階段 (Monster Phase)
            monsterPhase();

            // 3. 輪次結束結算階段 (End of Round)
            endOfRoundCleanup();
        }

        if (isBattleOver() == 1) cout << "🏆 玩家小隊獲得勝利！\n";
        else cout << "💀 玩家小隊全軍覆沒…… GAME OVER\n";
    }

    void BattleManager::playerTurn(Player& currentPlayer, int currentPlayerIdx) {
        bool actionCompleted = false;
    
        do 
        {
            // 在普攻或放技能前加入活體檢查
            while (!monsters[currentTargetIdx]->isAlive()) {
                currentTargetIdx = (currentTargetIdx + 1) % monsters.size();
            }
            
            renderUI();

            char choice;
            cout << "[" << currentPlayer.getName() << "] 的回合。選擇行動: [A]普攻 [1~3]技能 [I]道具 [T]切換目標\n> ";
            cin >> choice;

            if (choice == 'a' || choice == 'A') {
                // STRENGTH 狀態：建議在 Player::attack 內部去檢查 getEffectTurns(STRENGTH)
                // 若大於 0，則將基礎攻擊力乘以傷害加成係數，計算出最終傷害
                currentPlayer.attack(currentTargetIdx, monsters, players);
                actionCompleted = true; 
            }
            else if (choice == '1' || choice == '2' || choice == '3') {
                int skillIdx = choice - '0';
                // 這裡 useSkill 會回傳 bool，決定是否消耗非自由行動回合
                actionCompleted = currentPlayer.useSkill(skillIdx, currentTargetIdx, players, monsters); 
            }
            else if (choice == 'i' || choice == 'I') {
                cout << "請輸入要使用的道具代碼: ";
                int itemCode;
                cin >> itemCode;
                currentPlayer.useItem(itemCode);
                actionCompleted = false; // 使用道具不消耗回合
            }
            else if (choice == 't' || choice == 'T') {
                do {
                    currentTargetIdx = (currentTargetIdx + 1) % monsters.size();
                } while (!monsters[currentTargetIdx]->isAlive()); 
                actionCompleted = false; // 切換鎖定不消耗回合
            }
            else {
                cout << "輸入錯誤！請重新輸入。\n";
                actionCompleted = false;
            }

            // 🎯 【FREEACTION 攔截點】：作用在回合即將結束前
            if (actionCompleted) {
                if (currentPlayer.getEffectTurns(FREEACTION) > 0) {
                    cout << "⚡ [" << currentPlayer.getName() << "] 觸發了『自由行動』！獲得額外連擊回合！\n";
                    // 減少或移除 FREEACTION 狀態
                    currentPlayer.takeEffect(FREEACTION, currentPlayer.getEffectTurns(FREEACTION)-1);

                    actionCompleted = false; // 關鍵：強行重置為 false，利用 do-while 重新回到選單開頭！
                }
            }

        } while (!actionCompleted);
    }

    // 怪物行動階段與索敵邏輯
    void BattleManager::monsterPhase() {
        cout << "\n=== 👾 怪物行動階段 ===" << endl;
        
        for (int i = 0; i < monsters.size(); i++) {
            if (!monsters[i]->isAlive()) continue;

            // 控場檢查：如果怪物被冰凍(FREEZE)，跳過
            if (monsters[i]->getEffectTurns(FREEZE) > 0) {
                cout << "❄️ 怪物 [" << monsters[i]->getName() << "] 處於冰凍狀態，跳過回合。\n";
                continue;
            }

            // 🎯 【HIDE 索敵攔截點】：尋找合法的玩家目標
            int targetPlayerIdx = getValidMonsterTarget();
            if (targetPlayerIdx == -1) return; // 沒有活著的目標

            cout << "👹 [" << monsters[i]->getName() << "] 瞄準了 [" << players[targetPlayerIdx]->getName() << "]!\n";

            // 怪物發動攻擊，造成傷害
            monsters[i]->attack(targetPlayerIdx, players, monsters);
        }
    }

    // 索敵輔助函式：排除死者與擁有 HIDE 狀態的刺客
    int BattleManager::getValidMonsterTarget() {
        // 🎯 【TAUNT 優先權攔截】：如果有人開嘲諷，怪物眼睛裡就只有他！
        for (int i = 0; i < players.size(); i++) {
            if (players[i]->isAlive() && players[i]->getEffectTurns(TAUNT) > 0) {
                return i; // 直接回傳騎士的索引，後面的 HIDE 和隨機都不用看了
            }
        }

        vector<int> normalTargets;
        vector<int> hiddenTargets; // 備用：若大家都隱身，怪物還是得打人

        for (int i = 0; i < players.size(); i++) {
            if (!players[i]->isAlive()) continue;

            if (players[i]->getEffectTurns(HIDE) > 0) {
                hiddenTargets.push_back(i); // 隱身者先放進備用清單
            } else {
                normalTargets.push_back(i); // 正常可被鎖定的目標
            }
        }

        // 優先從沒隱身的活人裡隨機選一個
        if (!normalTargets.empty()) {
            return normalTargets[rand() % normalTargets.size()];
        }
        // 如果全部活著的人都處於 HIDE 隱身狀態或全隊陣亡，怪物失去目標，跳過回合
        cout << "" << endl;
        return -1;
    }

    // 3. 輪次結束結算階段：遞減狀態、結算持續傷害
    void BattleManager::endOfRoundCleanup() {
        cout << "\n--- 回合結束結算 ---" << endl;

        // 玩家狀態遞減與檢查（例如騎士的 PERSEVERANCE）
        for (auto& player : players) {
            if (!player->isAlive()) continue;
            
            // 🎯 【PERSEVERANCE 攔截點】：
            // 在每輪結束時，呼叫狀態更新更新剩餘回合。
            // 只要在狀態管理內設定：當計數歸零時，將該狀態從 map 中 erase 移除即可。
            player->updateStatusEffects(); 
            player->restoreMp(15);
        }

        // 🎯 【怪物的負面狀態在回合結束時減少一回合】 + 毒燒持續傷害結算
        for (auto& monster : monsters) {
            if (!monster->isAlive()) continue;

            // 結算持續傷害 (BURN / POISON)
            /*if (monster->getEffectTurns(BURN) > 0) {
                cout << "🔥 [" << monster->getName() << "] 受到燃燒傷害！\n";
                monster->takeDamage(20); // 舉例傷害
            }
            if (monster->getEffectTurns(POISON) > 0) {
                cout << "🤢 [" << monster->getName() << "] 受到中毒毒發傷害！\n";
                monster->takeDamage(15); 
            }*/

            // 令怪物身上的所有負面狀態計數器減 1 輪
            monster->updateStatusEffects();
        }
    }

    // 檢查戰鬥是否結束的輔助函式
    int BattleManager::isBattleOver() {
        bool allPlayersDead = true;
        for (auto& p : players) if (p->isAlive()) allPlayersDead = false;
        if (allPlayersDead) return 2; // 怪物勝

        bool allMonstersDead = true;
        for (auto& m : monsters) if (m->isAlive()) allMonstersDead = false;
        if (allMonstersDead) return 1; // 玩家勝

        return 0; // 戰鬥繼續
    }
}