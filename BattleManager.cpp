#include "BattleManager.h"
#include "Player.h"
#include "Skill.h"
#include "Item.h"
#include "Monster.h"

// 🚀 Include all derived monsters freely!
#include "VampireBat.h"
#include "BlazeShaman.h"
#include "SkeletonMasterSergeant.h"
#include "FrostSlime.h"
#include "TheDeceiver.h"

#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <string>
#include <iomanip> // 用於排版對齊

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;

namespace RPG_Colaborate
{
    // 🎯 Factory Implementation
    vector<Monster*> MonsterFactory::createStageMonsters(int stageLevel) {
        vector<Monster*> stageMonsters(5, nullptr);
        if (stageLevel < 1 || stageLevel > 10) return stageMonsters;

        const auto& layout = STAGE_DATABASE[stageLevel - 1];
        for (int i = 0; i < 5; i++) {
            switch (layout[i]) {
                case NONEM:     stageMonsters[i] = nullptr; break;
                // Base Monsters
                case SLIME:     stageMonsters[i] = new Monster("Slime", 1893, 761, 10, 0, 243, NORMAL); break;
                case GOBLIN:    stageMonsters[i] = new Monster("Goblin", 1929, 828, 10, 0, 243, NORMAL); break;
                case GOBLIN_W:  stageMonsters[i] = new Monster("Goblin Warrior", 6602, 1647, 20, 0, 365, NORMAL); break;
                case ELITE_G:   stageMonsters[i] = new Monster("Elite Goblin Warrior", 12024, 2334, 50, 0, 730, ELITE); break;
                case SKELETON:  stageMonsters[i] = new Monster("Skeleton", 1665, 1419, 15, 0, 243, NORMAL); break;
                case DK:        stageMonsters[i] = new Monster("Death Knight", 39629, 3038, 70, 0, 1228, ELITE); break;
                // Derived Special Monsters
                case BAT:       stageMonsters[i] = new VampireBat("Vampire Bat", 2718, 1120, 25, 20, 243, NORMAL); break;
                case SHAMAN:    stageMonsters[i] = new BlazeShaman("Blaze Shaman", 5739, 1493, 30, 0, 243, NORMAL); break;
                case SERGEANT:  stageMonsters[i] = new SkeletonMasterSergeant("Skeleton Master Sergeant", 50058, 3627, 50, 0, 779, ELITE); break;
                case ICE_SLIME: stageMonsters[i] = new FrostSlime("Frost Slime", 7881, 1004, 30, 0, 365, NORMAL); break;
                case DECEIVER:  stageMonsters[i] = new TheDeceiver("The Deceiver", 96615, 4267, 100, 30, 930, BOSS); break;
            }
        }
        return stageMonsters;
    }

    BattleManager::BattleManager(vector<Player*> AllPlayers)
    : players(AllPlayers) {}

    // 🆕 Pre-Battle Phase
    void BattleManager::preBattlePhase() {
        cout << "\n=== 🏕️ Pre-Battle Phase ===\n"; 
        char choice;
        do {
            cout << "Do you want to use an Out-of-Combat item? (y/n)\n> "; 
            cin >> choice;
            if (choice == 'y' || choice == 'Y') {
                int playerIdx, slotCode;
                cout << "Select Player (0-" << players.size() - 1 << "):\n> "; 
                cin >> playerIdx;
                
                if (playerIdx >= 0 && playerIdx < players.size() && players[playerIdx]->isAlive()) {
                    
                    // 📢 動態顯示該玩家的背包現狀
                    players[playerIdx]->showInventory();
                    
                    cout << "Enter the [Slot ID] of the item you want to use:\n> "; 
                    cin >> slotCode;
                    
                    if (players[playerIdx]->useOutOfCombatItem(slotCode, players, monsters)) {
                        cout << "🎒 Item used successfully!\n"; 
                    } else {
                        cout << "❌ Failed to use the item.\n";
                    }
                } else {
                    cout << "❌ Invalid player selection.\n"; 
                }
            }
        } while (choice == 'y' || choice == 'Y');
        cout << "--------------------------------\n";
    }

    void BattleManager::renderUI(int activePlayerIdx)
    {
        cout << "=======================================================================================================================\n";
        // 1. 左上角金幣保持不變
        int currentPartyGold = (!players.empty()) ? players[0]->getOwnedGolds() : 0;
        cout << " 💰 Party Gold: " << currentPartyGold << " G\n";
        cout << "=======================================================================================================================\n";
        
        // 🚀 100% 依賴 getEffectTurns 的通用狀態解析機制（已完全移除 getBuffs/getDebuffs）
        auto unpackStatus = [](auto* unit, std::string& buffs, std::string& debuffs) {
            buffs = "";
            debuffs = "";
            
            auto getEffectShortName = [](int type) -> std::string {
                switch(type) {
                    case LOWERDEFENSE: return "DEF↓"; case BURN: return "BRN"; case COLD: return "COLD";
                    case FREEZE: return "FRZ"; case POISON: return "PSN"; case WEAKNESS: return "WEAK";
                    case ELETRICSHOCK: return "SHOCK"; case TAUNT: return "TAUNT"; case PERSEVERANCE: return "PRSV";
                    case CONTSHOOT: return "DBL"; case SPACEGOAT: return "GOAT"; case HIDE: return "HIDE";
                    case FREEACTION: return "FREE"; case STRENGTH: return "STR"; case FEAST: return "FST";
                    case GOLDBULLET: return "GOLD"; case COUNTERATTACK: return "CNTR"; case GOLDEN_BELL: return "BELL";
                    case FLOOR_STRENGTH: return "F_STR"; case FLOOR_CALAMITY: return "F_CLM"; case LAST_GASP: return "GASP";
                    default: return "";
                }
            };

            // 為了防範新塞入的列舉、非連續列舉、或超出原本範圍的效果，我們直接盲掃 0 ~ 64
            for (int e = 0; e <= 64; e++) {
                if (unit->getEffectTurns(static_cast<EffectType>(e)) > 0) {
                    std::string sName = getEffectShortName(e);
                    
                    // 【除錯大師機制】如果身上有該狀態，但上面的 switch 沒寫到名字，就直接顯示其整數 ID！
                    if (sName.empty()) {
                        sName = "UNK_" + std::to_string(e);
                    }
                    
                    // 劃分正面(Buff)與負面(Debuff) -> 假設小於等於雷擊的都是負面
                    if (e <= static_cast<int>(ELETRICSHOCK)) {
                        debuffs += sName + " ";
                    } else {
                        buffs += sName + " ";
                    }
                }
            }
            
            // 清除末尾多餘的空格
            while (!buffs.empty() && buffs.back() == ' ') buffs.pop_back();
            while (!debuffs.empty() && debuffs.back() == ' ') debuffs.pop_back();
            
            // 若最終仍無狀態，則顯示 "None"
            if (buffs.empty()) buffs = "None";
            if (debuffs.empty()) debuffs = "None";
        };

        // 5個怪物各佔3行，總共 15 行
        int totalUILines = static_cast<int>(monsters.size()) * 3;
        
        for (int line = 0; line < totalUILines; line++) {
            std::string leftRow = "";
            std::string rightRow = "";

            // ------------------ 👤 左側角色 UI 邏輯 ------------------
            if (line % 3 == 1) {
                // 角色主要資訊行
                int p = (line - 1) / 3;
                if (p >= 0 && p < players.size()) {
                    std::string pArrow = (p == activePlayerIdx) ? "==> " : "    ";
                    leftRow = pArrow + "👤 [" + players[p]->getJob() + "] " + players[p]->getName() +
                              " HP:" + std::to_string(players[p]->getHp()) + "/" + std::to_string(players[p]->getMaxHp()) +
                              " MP:" + std::to_string(players[p]->getMp()) + "/" + std::to_string(players[p]->getMaxMp()) +
                              " CD:[" + std::to_string(players[p]->getSkillCooldown(0)) + "/" +
                              std::to_string(players[p]->getSkillCooldown(1)) + "/" +
                              std::to_string(players[p]->getSkillCooldown(2)) + "]";
                }
            } 
            else if (line % 3 == 2) {
                // 角色狀態行
                int p = (line - 2) / 3;
                if (p >= 0 && p < players.size()) {
                    std::string buffs, debuffs;
                    unpackStatus(players[p], buffs, debuffs); // 呼叫全新純 getEffectTurns 解包
                    leftRow = "         State -> B:[" + buffs + "] D:[" + debuffs + "]";
                }
            } 
            else if (line % 3 == 0) {
                // 角色虛線分隔行
                int p = (line - 3) / 3;
                if (p >= 0 && p < players.size()) {
                    leftRow = "    ========================================";
                }
            }

            // ------------------ 👹 右側怪物 UI 邏輯 ------------------
            if (line % 3 == 0) {
                // 怪物名稱與血量行
                int m = line / 3;
                if (m >= 0 && m < monsters.size()) {
                    if (monsters[m] == nullptr) {
                        rightRow = "💀 [Pos " + std::to_string(m + 1) + ": -- Empty --]";
                    } else if (!monsters[m]->isAlive()) {
                        rightRow = "🪦 [" + monsters[m]->getName() + " Defeated]";
                    } else {
                        rightRow = "👹 " + monsters[m]->getName() + 
                                   " HP:" + std::to_string(monsters[m]->getHp()) + "/" + std::to_string(monsters[m]->getMaxHp());
                    }
                }
            } 
            else if (line % 3 == 1) {
                // 怪物鎖定箭頭與狀態行
                int m = (line - 1) / 3;
                if (m >= 0 && m < monsters.size() && monsters[m] != nullptr && monsters[m]->isAlive()) {
                    std::string mArrow = (m == currentTargetIdx) ? "==> " : "    ";
                    std::string buffs, debuffs;
                    unpackStatus(monsters[m], buffs, debuffs); // 怪物也同步呼叫此方法
                    rightRow = mArrow + "State -> B:[" + buffs + "] D:[" + debuffs + "]";
                }
            } 
            else if (line % 3 == 2) {
                // 怪物虛線分隔行
                int m = (line - 2) / 3;
                if (m >= 0 && m < monsters.size()) {
                    rightRow = "========================";
                }
            }

            cout << std::left << std::setw(68) << leftRow << " | " << rightRow << "\n";
        }
        cout << "=======================================================================================================================\n";
    }

    void BattleManager::startBattle() {
        cout << "⚔️ Campaign Started! ⚔️\n"; 
        
        for (size_t stage = 1; stage <= STAGE_DATABASE.size(); stage++) {
            cout << "\n===================================================\n";
            cout << "🏰 [STAGE " << stage << "] Entrance Detected!\n"; 
            cout << "===================================================\n";

            preBattlePhase();

            cout << "👾 Spawning monsters for Stage " << stage << "...\n"; 
            monsters = MonsterFactory::createStageMonsters(stage);
            
            autoLockFirstTarget();
            if (monsters.size() > 2 && monsters[2] != nullptr) {
                monsters[2]->entryAction(players, monsters);
            }

            vector<bool> looted(monsters.size(), false);
            
            // 🎯 1. 將掉落與賞金改為「暫存形式變數」
            int stageGoldPool = 0;
            vector<Item> stageItemDrops;

            // 🎯 修改後的 Lambda：此時只存放、不索取輸入，完全不干擾戰鬥迴圈
            auto processLoot = [&]() {
                for (size_t m = 0; m < monsters.size(); m++) {
                    if (monsters[m] != nullptr && !monsters[m]->isAlive() && !looted[m]) {
                        looted[m] = true; 
                        
                        // 暫存金幣
                        int bounty = monsters[m]->getRewardGold(); 
                        stageGoldPool += bounty;

                        // 暫存道具
                        int rank = monsters[m]->getRank(); 
                        if (rank == ELITE || (rank == NORMAL && rand() % 100 < 50)) {
                            int itemDropCode;
                            bool isElite = (rank == ELITE);
                            
                            if (isElite) {
                                int roll = rand() % 100;
                                const int rares[] = {5, 7, 9};
                                const int uncommons[] = {2, 6, 8};
                                itemDropCode = (roll < 20) ? rares[rand() % 3] : uncommons[rand() % 3];
                            } else {
                                int roll = rand() % 100;
                                const int uncommons[] = {2, 6, 8};
                                const int commons[] = {0, 1, 2, 3};
                                itemDropCode = (roll < 15) ? uncommons[rand() % 3] : commons[rand() % 4];
                            }
                            
                            Item theItemDrop = Item().getStandardItems(itemDropCode);
                            stageItemDrops.push_back(theItemDrop); // 塞入暫存陣列
                        }
                    }
                }
            };
            
            // 🎯 追蹤這一關最後是誰行動的標記變數
            int lastActivePlayerIdx = 0;

            while (isBattleOver() == 0) { 
                cout << "\n=== ✨ New Round Begins ===" << endl;

                playerTurnReached.assign(players.size(), false);
                monsterTurnReached.assign(monsters.size(), false);

                bool battleEndedEarly = false;

                for (int i = 0; i < players.size(); i++) {
                    if (players[i]->isAlive()) {
                        playerTurnReached[i] = true; 

                        if (players[i]->getEffectTurns(FREEZE) > 0) {
                            cout << "❄️ [" << players[i]->getName() << "] is frozen and skipped!\n"; 
                            continue; 
                        } else if (players[i]->getEffectTurns(ELETRICSHOCK) > 0) {
                            cout << "⚡ [" << players[i]->getName() << "] is paralyzed and skipped!\n"; 
                            continue; 
                        }
                        
                        // 記錄當前準備或正在行動的角色標記
                        lastActivePlayerIdx = i;
                        playerTurn(*players[i], i); 

                        processLoot();
                    }

                    if (isBattleOver() != 0) {
                        battleEndedEarly = true;
                        break; 
                    }
                }

                if (battleEndedEarly) {
                    endOfRoundCleanup();
                    processLoot(); 
                    break; 
                }

                monsterPhase();
                processLoot(); 

                endOfRoundCleanup();
                processLoot(); 
                
                // 🎯 依題意：在怪物全部動過之後（即回合正常結束時），重置角色行動標記
                lastActivePlayerIdx = 0;
            }

            // 🎯 2. 通關層數結算與獎勵發放 (當 isBattleOver == 1 時)
            if (isBattleOver() == 1) {
                
                // [前置結算]: 從第一個角色到最後行動標記角色為止，進行回合狀態結算
                for (int p = 0; p <= lastActivePlayerIdx; p++) {
                    if (players[p]->isAlive()) {
                        players[p]->updateStatusEffects(monsters);
                    }
                }
                
                // [前置結算]: 所有存活角色均進行 -cd 結算
                for (auto* p : players) {
                    if (p->isAlive()) {
                        p->reduceCooldowns();
                    }
                }

                // a. 將 FLOOR_STRENGTH, FLOOR_CALAMITY 這兩個層數狀態進行結算 (Turns - 1)
                for (auto* p : players) {
                    if (p->isAlive()) {
                        int currentStrTurns = p->getEffectTurns(FLOOR_STRENGTH);
                        if (currentStrTurns > 0) {
                            p->takeEffect(FLOOR_STRENGTH, currentStrTurns - 1);
                        }
                        int currentCalTurns = p->getEffectTurns(FLOOR_CALAMITY);
                        if (currentCalTurns > 0) {
                            p->takeEffect(FLOOR_CALAMITY, currentCalTurns - 1);
                        }
                    }
                }

                // b. 所有存活角色的魔力回滿
                for (auto* p : players) {
                    if (p->isAlive()) {
                        p->restoreMp(100); 
                    }
                }

                cout << "\n🎉 Stage " << stage << " Cleared successfully!\n"; 

                // 發放加總後的總賞金金幣
                if (stageGoldPool > 0) {
                    cout << "\n💰 [Stage Reward] Total bounty collected: " << stageGoldPool << " Gold!\n";
                    for (auto* p : players) {
                        p->addBountyGold(stageGoldPool);
                    }
                    cout << "✨ All players received " << stageGoldPool << " Gold!\n";
                }

                // 發放暫存陣列中的所有道具（安全脫戰互動）
                for (const auto& dropItem : stageItemDrops) {
                    bool getPlayer = false;
                    cout << "\n🎁 Stored loot ready for distribution: [" << dropItem.getName() << "]\n";
                    do
                    {
                        cout << "-------------------------------------------------\n";
                        cout << "Who should pick up this item? Enter Player ID:\n";
                        for (size_t pIdx = 0; pIdx < players.size(); pIdx++) {
                            if (players[pIdx]->isAlive()) {
                                cout << "[" << pIdx << "] " << players[pIdx]->getName() << " (" << players[pIdx]->getJob() << ")\n";
                            }
                        }
                        cout << "> ";
                        
                        int chosenPlayerIdx;
                        cin >> chosenPlayerIdx;

                        getPlayer = (chosenPlayerIdx >= 0 && chosenPlayerIdx < players.size() && players[chosenPlayerIdx]->isAlive());
                        
                        if (getPlayer) {
                            players[chosenPlayerIdx]->obtainItem(dropItem);
                        }
                        else {
                            cout << "❌ Invalid selection or Player is dead! Please re-enter!\n";
                        }
                    } while (!getPlayer);
                    cout << "-------------------------------------------------\n";
                }
                
                // 記憶體回收與清理
                for (auto* monsterPtr : monsters) {
                    if (monsterPtr != nullptr) {
                        delete monsterPtr; 
                    }
                }
                monsters.clear(); 
                
                cout << "Press Enter to proceed to the next stage...\n"; 
                cin.get(); cin.ignore(); 
            } 
            else if (isBattleOver() == 2) {
                cout << "💀 The Player Party has been wiped out on Stage " << stage << "... GAME OVER\n"; 
                
                for (auto* monsterPtr : monsters) {
                    if (monsterPtr != nullptr) delete monsterPtr;
                }
                return; 
            }
        }

        cout << "\n===================================================\n";
        cout << "🏆 Congratulations! You have conquered the entire Dungeon!\n"; 
        cout << "===================================================\n";
    }

    void BattleManager::playerTurn(Player& currentPlayer, int currentPlayerIdx) {
        bool actionCompleted = false;
    
        do 
        {
            while (monsters[currentTargetIdx] == nullptr || !monsters[currentTargetIdx]->isAlive()) {
                currentTargetIdx = (currentTargetIdx + 1) % monsters.size();
            }
            
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
                cout << "[1] Scorching Sun  [2] Golden Bell  [3] Thor's Fury  [4] The Last Gasp\n> ";
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

            bool allMonstersDead = true;
            for (auto& m : monsters) {
                if (m != nullptr && m->isAlive()) {
                    allMonstersDead = false;
                    break; 
                }
            }
            if (actionCompleted) {
                if (!allMonstersDead) {
                    if (currentPlayer.getEffectTurns(FREEACTION) > 0) {
                        cout << "⚡ [" << currentPlayer.getName() << "] triggered 'Free Action'! Gained an extra combo turn!\n";
                        currentPlayer.takeEffect(FREEACTION, currentPlayer.getEffectTurns(FREEACTION) - 1);
                        actionCompleted = false; 
                    }
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

            monsterTurnReached[i] = true; 

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

        for (int i = 0; i < players.size(); i++) {
            if (!players[i]->isAlive()) continue;
            
            if (playerTurnReached[i]) {
                players[i]->updateStatusEffects(monsters);
            } else {
                cout << "💤 [" << players[i]->getName() << "] Turn not reached. Status preserved.\n";
            }
            
            players[i]->reduceCooldowns(); 
            players[i]->restoreMp(15);
        }

        for (int i = 0; i < monsters.size(); i++) {
            if (monsters[i] == nullptr || !monsters[i]->isAlive()) continue;
            
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