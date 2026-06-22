#include "BattleManager.h"
#include "Player.h"
#include "Skill.h"
#include "Item.h"
#include "Monster.h"

// 🚀 在 .cpp 自由引入所有衍生的特殊怪物標頭檔，絕對不會引發循環引用！
#include "VampireBat.h"
#include "BlazeShaman.h"
#include "SkeletonMasterSergeant.h"
#include "FrostSlime.h"
#include "TheDeceiver.h"

#include <iostream>
#include <cstdlib>
#include <algorithm>

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;

namespace RPG_Colaborate
{
    // ... BattleManager 的其他函式實作 (startBattle, renderUI 等) 維持不變 ...

    // 🎯 關鍵改動：將工廠的實作移到這裡！此時編譯器已經完整認識所有怪物類別了
    vector<Monster*> MonsterFactory::createStageMonsters(int stageLevel) {
        vector<Monster*> stageMonsters(5, nullptr);
            if (stageLevel < 1 || stageLevel > 10) return stageMonsters;

            const auto& layout = STAGE_DATABASE[stageLevel - 1];
            for (int i = 0; i < 5; i++) {
                switch (layout[i]) {
                    case NONEM:     stageMonsters[i] = nullptr; break;
                    // 無技能小怪 (Base Monster) - 名字，血，攻擊，賞金，閃避，防禦，等級
                    case SLIME:     stageMonsters[i] = new Monster("Slime", 1893, 761, 10, 0, 243, NORMAL); break;
                    case GOBLIN:    stageMonsters[i] = new Monster("Goblin", 1929, 828, 10, 0, 243, NORMAL); break;
                    case GOBLIN_W:  stageMonsters[i] = new Monster("Goblin Warrior", 6602, 1647, 20, 0, 365, NORMAL); break;
                    case ELITE_G:   stageMonsters[i] = new Monster("Elite Goblin Warrior", 12024, 2334, 50, 0, 730, ELITE); break;
                    case SKELETON:  stageMonsters[i] = new Monster("Skeleton", 1665, 1419, 10, 15, 243, NORMAL); break;
                    case DK:        stageMonsters[i] = new Monster("Death Knight", 39629, 3038, 50, 0, 1228, ELITE); break;
                    // 衍生特殊怪物 (Derived Monster)
                    case BAT:       stageMonsters[i] = new VampireBat("Vampire Bat", 2718, 1120, 25, 20, 243, NORMAL); break;
                    case SHAMAN:    stageMonsters[i] = new BlazeShaman("Blaze Shaman", 5739, 1493, 30, 0, 243, NORMAL); break;
                    case SERGEANT:  stageMonsters[i] = new SkeletonMasterSergeant("Skeleton Master Sergeant", 50058, 3627, 70, 0, 779, ELITE); break;
                    case ICE_SLIME: stageMonsters[i] = new FrostSlime("Frost Slime", 7881, 1004, 30, 0, 365, NORMAL); break;
                    case DECEIVER:  stageMonsters[i] = new TheDeceiver("The Deceiver", 96615, 4267, 100, 30, 930, BOSS); break;
                }
            }
            return stageMonsters;
    }

    BattleManager::BattleManager(vector<Player*> AllPlayers)
    : players(AllPlayers) {}

    // 🆕 脫戰道具使用階段
    void BattleManager::preBattlePhase() {
        cout << "\n=== 🏕️ Pre-Battle Phase ===\n"; // === 戰前準備階段 ===
        char choice;
        do {
            cout << "Do you want to use an Out-of-Combat item? (y/n)\n> "; // 是否使用脫戰道具？(y/n)
            cin >> choice;
            if (choice == 'y' || choice == 'Y') {
                int playerIdx, itemCode;
                cout << "Select Player (0-" << players.size() - 1 << "):\n> "; // 選擇玩家
                cin >> playerIdx;
                
                if (playerIdx >= 0 && playerIdx < players.size() && players[playerIdx]->isAlive()) {
                    cout << "Select Item: [1] Strength Potion [2] Calamity Potion (etc...)\n> "; // 選擇道具：[1]力量藥水 [2]災厄藥水 等...
                    cin >> itemCode;
                    // TODO: 呼叫玩家的脫戰道具使用邏輯，例如：
                    // players[playerIdx]->useOutOfCombatItem(itemCode);
                    cout << "🎒 Item used successfully!\n"; // 道具使用成功！
                } else {
                    cout << "❌ Invalid player selection.\n"; // 無效的玩家選擇
                }
            }
        } while (choice == 'y' || choice == 'Y');
        cout << "--------------------------------\n";
    }

    // UI：交錯排版 (新增 activePlayerIdx 標記當前行動玩家)
    void BattleManager::renderUI(int activePlayerIdx)
    {
        cout << "=================================================================\n";
        
        int totalRows = std::max(static_cast<int>(players.size()), static_cast<int>(monsters.size()));
        
        for (int i = 0; i < totalRows; i++) {
            // 1. 輸出玩家（靠左）- 根據 activePlayerIdx 加上行動標記
            if (i < players.size()) {
                // 如果是當前回合玩家，加上 "==> " 標記；其餘補 4 個空白維持排版對齊
                string pArrow = (i == activePlayerIdx) ? "==> " : "    ";
                
                cout << pArrow << "👤 [" << players[i]->getJob() << "] " << players[i]->getName() 
                     << " HP:" << players[i]->getHp() << "/" << players[i]->getMaxHp() 
                     << " MP:" << players[i]->getMp() << "/" << players[i]->getMaxMp()
                     << " | Buff/Debuff: [...] \n";
            }
            
            // 2. 輸出怪物（加上 Tab 縮排，製造交錯感）
            if (i < monsters.size()) {
                cout << "\t\t\t\t"; // 關鍵縮排，將怪物往右推
                string arrow = (i == currentTargetIdx) ? "==> " : "    ";
                cout << arrow;
                
                if (monsters[i] == nullptr) {
                    cout << "💀 [Pos " << i + 1 << ": -- Empty --]\n";
                } else if (!monsters[i]->isAlive()) {
                    cout << "🪦 [" << monsters[i]->getName() << " Defeated]\n";
                } else {
                    cout << "👹 " << monsters[i]->getName() 
                         << " HP:" << monsters[i]->getHp() << "/" << monsters[i]->getMaxHp()
                         << " | Buff:[" << monsters[i]->getBuffs() << "] Debuff:[" << monsters[i]->getDebuffs() << "]\n";
                }
            }
            cout << "-----------------------------------------------------------------\n";
        }
    }

    void BattleManager::startBattle() {
        cout << "⚔️ Campaign Started! ⚔️\n"; // 遠征正式開始！
        
        // 🚀 核心改動：將所有戰鬥內容包進層數迴圈中 (從第 1 層到最大層數)
        for (size_t stage = 1; stage <= STAGE_DATABASE.size(); stage++) {
            cout << "\n===================================================\n";
            cout << "🏰 [STAGE " << stage << "] Entrance Detected!\n"; // 偵測到第 X 層入口！
            cout << "===================================================\n";

            // 1. 每一層先做脫戰行為 (Pre-Battle Phase for Out-of-Combat items)
            preBattlePhase();

            // 2. 用怪物工廠設定目前層數的怪物陣列 (Initialize monster array via Factory)
            cout << "👾 Spawning monsters for Stage " << stage << "...\n"; // 正在生成第 X 層的怪物……
            monsters = MonsterFactory::createStageMonsters(stage);
            
            // 戰鬥開始時，自動鎖定當前層數第一個活著且非空的怪物
            autoLockFirstTarget();
            
            // 3. 進行戰鬥行為迴圈 (Battle Loop for the current stage)
            while (isBattleOver() == 0) { 
                cout << "\n=== ✨ New Round Begins ===" << endl;

                // 🎯 初始化：預設所有人這輪都還沒被「點名」
                playerTurnReached.assign(players.size(), false);
                monsterTurnReached.assign(monsters.size(), false);

                bool battleEndedEarly = false;

                // 玩家行動階段
                for (int i = 0; i < players.size(); i++) {
                    if (players[i]->isAlive()) {
                        playerTurnReached[i] = true; // 🎯 輪到他了！(不管有沒有被控)

                        // 檢查控場：被控就跳過輸入，但「輪次抵達」已成事實
                        if (players[i]->getEffectTurns(FREEZE) > 0) {
                            cout << "❄️ [" << players[i]->getName() << "] is frozen and skipped!\n"; 
                            continue; 
                        } else if (players[i]->getEffectTurns(ELETRICSHOCK) > 0) {
                            cout << "⚡ [" << players[i]->getName() << "] is paralyzed and skipped!\n"; 
                            continue; 
                        }
                        
                        playerTurn(*players[i], i); // 正常執行轉輪、出招
                    }

                    // 🎯 核心情境：如果前幾個角色把怪滅了
                    if (isBattleOver() != 0) {
                        battleEndedEarly = true;
                        break; // 跳出玩家循環
                    }
                }

                // 🎯 如果怪被滅了/玩家全滅，直接進結算（確保沒點名的人扣到 CD），然後退出戰鬥
                if (battleEndedEarly) {
                    endOfRoundCleanup();
                    break; 
                }

                // 怪物行動階段
                monsterPhase();

                // 正常走完一整輪的結算
                endOfRoundCleanup();
            }

            // 4. 層數勝負結算與記憶體清理 (Stage Result Evaluation & Memory Cleanup)
            if (isBattleOver() == 1) {
                cout << "\n🎉 Stage " << stage << " Cleared successfully!\n"; // 恭喜通關第 X 層！
                
                // 🚨 預防記憶體洩漏：通關後必須釋放工廠 new 出來的怪物物件
                for (auto* monsterPtr : monsters) {
                    if (monsterPtr != nullptr) {
                        delete monsterPtr; // 釋放記憶體
                    }
                }
                monsters.clear(); // 清空容器，準備迎接下一層的新怪
                
                cout << "Press Enter to proceed to the next stage...\n"; // 按下 Enter 鍵前進到下一層……
                cin.get(); cin.ignore(); // 暫停一下讓玩家看清通關訊息
            } 
            else if (isBattleOver() == 2) {
                cout << "💀 The Player Party has been wiped out on Stage " << stage << "... GAME OVER\n"; // 玩家小隊在第 X 層全軍覆沒…… GAME OVER
                
                // 🚨 失敗時同樣做好記憶體清理
                for (auto* monsterPtr : monsters) {
                    if (monsterPtr != nullptr) delete monsterPtr;
                }
                return; // 直接結束整個 startBattle()，終止遊戲
            }
        }

        // 🏆 成功爬完所有層數的終極勝利宣告
        cout << "\n===================================================\n";
        cout << "🏆 Congratulations! You have conquered the entire Dungeon!\n"; // 恭喜！你征服了整座地下城！
        cout << "===================================================\n";
    }

    void BattleManager::playerTurn(Player& currentPlayer, int currentPlayerIdx) {
        bool actionCompleted = false;
    
        do 
        {
            while (monsters[currentTargetIdx] == nullptr || !monsters[currentTargetIdx]->isAlive()) {
                currentTargetIdx = (currentTargetIdx + 1) % monsters.size();
            }
            
            // 🎯 關鍵改動：將目前輪到的玩家索引傳入 renderUI
            renderUI(currentPlayerIdx);

            char choice;
            cout << "[" << currentPlayer.getName() << "]'s Turn. Action: [A]Attack [1~3]Skill [I]Item [T]Switch Target\n> ";
            cin >> choice;

            if (choice == 'a' || choice == 'A') {
                currentPlayer.attack(currentTargetIdx, monsters, players);
                actionCompleted = true; 
            }
            else if (choice == '1' || choice == '2' || choice == '3') {
                int skillIdx = choice - '0';
                actionCompleted = currentPlayer.useSkill(skillIdx, currentTargetIdx, players, monsters); 
            }
            else if (choice == 'i' || choice == 'I') {
                cout << "Enter the item code to use: \n";
                cout << "[1] Scorching Sun  [2] Golden Bell  [3] Wrath of Thor  [4] The Last Gasp\n> ";
                int itemCode;
                cin >> itemCode;
                
                currentPlayer.useItem(itemCode, players, monsters);
                actionCompleted = false; 
            }
            else if (choice == 't' || choice == 'T') {
                switchTarget(); 
                actionCompleted = false; 
            }
            else {
                cout << "❌ Invalid input! Please try again.\n";
                actionCompleted = false;
            }

            if (actionCompleted) {
                if (currentPlayer.getEffectTurns(FREEACTION) > 0) {
                    cout << "⚡ [" << currentPlayer.getName() << "] triggered 'Free Action'! Gained an extra combo turn!\n";
                    currentPlayer.takeEffect(FREEACTION, currentPlayer.getEffectTurns(FREEACTION) - 1);
                    actionCompleted = false; 
                }
            }

        } while (!actionCompleted);
    }

    void BattleManager::autoLockFirstTarget() {
        for (int i = 0; i < monsters.size(); i++) {
            if (monsters[i] != nullptr && monsters[i]->isAlive()) {
                currentTargetIdx = i;
                break; 
            }
        }
    }

    void BattleManager::switchTarget() {
        int startIdx = currentTargetIdx;
        do {
            currentTargetIdx = (currentTargetIdx + 1) % monsters.size(); 
            if (monsters[currentTargetIdx] != nullptr && monsters[currentTargetIdx]->isAlive()) {
                return; 
            }
        } while (currentTargetIdx != startIdx); 
    }

    void BattleManager::monsterPhase() {
        cout << "\n=== 👾 Monster Phase ===\n"; 
        
        for (int i = 0; i < monsters.size(); i++) {
            if (monsters[i] == nullptr || !monsters[i]->isAlive()) continue;

            monsterTurnReached[i] = true; // 🎯 輪到這隻怪物了！

            if (monsters[i]->getEffectTurns(FREEZE) > 0 || monsters[i]->getEffectTurns(ELETRICSHOCK) > 0) {
                cout << "❄️⚡ [" << monsters[i]->getName() << "] is crowd-controlled and skipped!\n"; 
                continue; 
            }

            int targetPlayerIdx = getValidMonsterTarget();
            if (targetPlayerIdx == -1) return; 

            monsters[i]->attack(targetPlayerIdx, players, monsters);
        }
    }

    int BattleManager::getValidMonsterTarget() {
        for (int i = 0; i < players.size(); i++) {
            if (players[i]->isAlive() && players[i]->getEffectTurns(TAUNT) > 0) {
                return i; 
            }
        }

        vector<int> normalTargets;
        vector<int> hiddenTargets; 

        for (int i = 0; i < players.size(); i++) {
            if (!players[i]->isAlive()) continue;

            if (players[i]->getEffectTurns(HIDE) > 0) {
                hiddenTargets.push_back(i); 
            } else {
                normalTargets.push_back(i); 
            }
        }

        if (!normalTargets.empty()) {
            return normalTargets[rand() % normalTargets.size()];
        }
        
        if (!hiddenTargets.empty()) {
            return hiddenTargets[rand() % hiddenTargets.size()];
        }

        return -1;
    }

    void BattleManager::endOfRoundCleanup() {
        cout << "\n--- End of Round Cleanup ---\n"; 

        // 1. 玩家結算
        for (int i = 0; i < players.size(); i++) {
            if (!players[i]->isAlive()) continue;
            
            // ⏳ 【狀態結算】只有「確實輪到他」的人，才進行狀態流逝
            if (playerTurnReached[i]) {
                players[i]->updateStatusEffects(); 
            } else {
                cout << "💤 [" << players[i]->getName() << "] Turn not reached. Status preserved.\n";
            }
            
            // 🎯 【技能 CD 減免】檢查點！只要活著，全體無條件享受 -cd
            players[i]->reduceCooldowns(); 
            
            players[i]->restoreMp(15);
        }

        // 2. 怪物結算
        for (int i = 0; i < monsters.size(); i++) {
            if (monsters[i] == nullptr || !monsters[i]->isAlive()) continue;
            
            // ⏳ 怪物同樣只有輪到的才結算狀態
            if (monsterTurnReached[i]) {
                monsters[i]->updateStatusEffects();
            }
        }
    }

    int BattleManager::isBattleOver() {
        bool allPlayersDead = true;
        for (auto& p : players) if (p->isAlive()) allPlayersDead = false;
        if (allPlayersDead) return 2; 

        bool allMonstersDead = true;
        for (auto& m : monsters) {
            if (m != nullptr && m->isAlive()) {
                allMonstersDead = false;
                break; 
            }
        }
        if (allMonstersDead) return 1; 

        return 0; 
    }
}