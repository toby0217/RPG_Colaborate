#include <iostream>
#include <vector>
#include <string>
#include "Player.h"
#include "Skill.h"
#include "Item.h"
#include "Monster.h"
// 🎯 引入各個子職業，讓 main 能夠進行實體化
#include "Knight.h"
#include "Mage.h"
#include "Ranger.h"
#include "Assassin.h"
#include "Priest.h"
#include "Necromancer.h"
#include "BountyHunter.h"
#include "Berserker.h"
#include "BattleManager.h" // 確保戰場管理員有被引入

using std::vector;
using std::cin;
using std::cout;
using std::endl;
using namespace RPG_Colaborate;

int main()
{
    // 🎯 修正：改成指標向量，防止物件切片 (Object Slicing)，確保多型運作
    vector<Player*> players;
    
    int role;
    cout << "========================================\n";
    cout << "       歡迎來到 RPG 協同戰鬥測試系統       \n";
    cout << "========================================\n";
    
    // 戰鬥準備 1：選擇職業
    // 🎯 Combat Preparation 1: Choose Job Classes (0-7)
    for (int i = 0; i < 4; i++) {
        cout << "\n==================================================================\n";
        cout << "Please choose the class for Adventurer " << i + 1 << ":\n";
        cout << "==================================================================\n";
        cout << "[0] Knight        - The ultimate frontliner with high defense & injury-withstanding.\n";
        cout << "[1] Mage          - Master of elements, unleashing devastating AOE spells.\n";
        cout << "[2] Ranger        - Elite sharpshooter who masters the laws of the forest.\n";
        cout << "[3] Assassin      - Master of ninjutsu, executing high-value backline targets.\n";
        cout << "[4] Priest        - Holy healer with powerful recovery & revival skills.\n";
        cout << "[5] Necromancer   - Undead summoner who commands the dead to fight.\n";
        cout << "[6] Bounty Hunter - Gold-obsessed gunslinger sweeping fields with a shotgun.\n";
        cout << "[7] Berserker     - Blood-soaked beast thriving on slaughter & low-HP fury.\n";
        cout << "------------------------------------------------------------------\n";
        cout << "Enter choice (0-7): ";
        
        // Input validation for 8 classes (0 to 7)
        while (!(cin >> role) || role < 0 || role > 7) {
            cout << "❌ Invalid choice! Please enter a valid class number (0-7): ";
            cin.clear();
            while (cin.get() != '\n'); // Clear input buffer
        }

        string defaultName = "Adventurer_" + std::to_string(i + 1);
        
        switch (role)
        {
        case 0:
            // Knight: High HP & DEF, No MP, Balanced ATK
            players.push_back(new Knight(defaultName, 3210, 100, 896, 1528));
            cout << "🛡️ " << defaultName << " has joined the party as a [Knight]!\n";
            break;
        case 1:
            // Mage: Low HP & DEF, High MP & ATK
            players.push_back(new Mage(defaultName, 2118, 100, 1441, 889));
            cout << "🔮 " << defaultName << " has joined the party as a [Mage]!\n";
            break;
        case 2:
            // Ranger: Moderate HP & DEF, High ATK, Sustained MP
            players.push_back(new Ranger(defaultName, 2366, 100, 1229, 653));
            cout << "🏹 " << defaultName << " has joined the party as a [Ranger]!\n";
            break;
        case 3:
            // Assassin: Glass-cannon burst, High ATK, Low DEF
            players.push_back(new Assassin(defaultName, 2629, 100, 1868, 629));
            cout << "🥷 " << defaultName << " has joined the party as an [Assassin]!\n";
            break;
        case 4:
            // Priest: High MP, Strong recovery, Lower ATK
            players.push_back(new Priest(defaultName, 4048, 100, 1002, 762));
            cout << "✨ " << defaultName << " has joined the party as a [Priest]!\n";
            break;
        case 5:
            // Necromancer: High MP, Balanced HP/ATK for summoning overhead
            players.push_back(new Necromancer(defaultName, 1927, 100, 1126, 845));
            cout << "💀 " << defaultName << " has joined the party as a [Necromancer]!\n";
            break;
        case 6:
            // Bounty Hunter: Burst shotgun ATK, Balanced HP & DEF
            players.push_back(new BountyHunter(defaultName, 2778, 100, 1074, 1149));
            cout << "🪙 " << defaultName << " has joined the party as a [Bounty Hunter]!\n";
            break;
        case 7:
            // Berserker: High HP, High ATK, scales aggressively with missing health
            players.push_back(new Berserker(defaultName, 4946, 100, 1335, 447));
            cout << "🪓 " << defaultName << " has joined the party as a [Berserker]!\n";
            break;
        }
    }

        // 戰鬥準備 2：每人自帶一個回血與回魔道具
        // 💡 註：因為 Player::items 是 protected，最優雅的做法是在各職業的建構子內部
        // 直接執行 items[1] = Item("生命藥水", ...) 與 items[2] = Item("魔法藥水", ...)。
        // 這樣 main() 就不需要越權去塞道具，維持封裝性！
    
    cout << "\n========================================\n";
    cout << "⚔️ 冒險者小隊集結完畢，準備生成怪物資料庫...\n";
    cout << "========================================\n";

    // 創建怪物資料庫 (作為戰場生成怪物的範本)
    std::vector<Monster*> monsterTable;

    // 參數順序：名字, 血量, 攻擊力, 獎勵金幣, 閃避率, 防禦力, 等級(Rank)
    // 1. 小怪們 (Normal)
    monsterTable.push_back(new Monster("Slime", 50, 10, 10, 0, 0, NORMAL));
    monsterTable.push_back(new Monster("Goblin", 80, 10, 15, 5, 0, NORMAL));
    monsterTable.push_back(new Monster("Goblin Warrior", 80, 10, 20, 5, 5, NORMAL));
    monsterTable.push_back(new Monster("Skeleton", 60, 16, 25, 0, 2, NORMAL));

    // 2. 精英怪們 (Elite)
    monsterTable.push_back(new Monster("Elite Goblin Warrior", 150, 18, 50, 5, 12, ELITE));
    monsterTable.push_back(new Monster("Death Knight", 250, 22, 100, 0, 20, ELITE));

    cout << "👾 成功載入 " << monsterTable.size() << " 種怪物範本。\n";
    cout << "🏹 戰場大門開啟，冒險者進入戰場！\n\n";

    // 戰鬥流程啟動：
    // 戰場管理員接手，內部會處理關卡(層數)推進、增益選擇、玩家與怪物的回合交替
    BattleManager battleArena(players, monsterTable);
    battleArena.startBattle();

    // 🎯 記憶體管理安全防線：戰鬥結束後，手動釋放 new 出來的動態記憶體
    for (Player* p : players) {
        delete p;
    }
    players.clear();

    cout << "\n測試結束，感謝遊玩！\n";
    return 0;
}