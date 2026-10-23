# ⚔️ RPG_Colaborate: CLI 文字 RPG 協同戰鬥系統

> 國立臺南大學 資訊工程學系 《計算機程式設計實習(二)》期末專題作品

本專案為一款基於 C++11/14 開發的多人協同戰鬥 CLI RPG 系統。系統採用物件導向設計（OOP）、設計模式（Manager / Factory）與 C++ STL 容器，支援 8 大特色職業與多階級怪物陣營回合制對戰。

---

## 🛠️ 技術棧與核心架構演進

* **開發語言與工具**：`C++11 / 14 / 17` | `Git / GitHub`
* **核心系統重構 (System Refactoring)**：
  * **戰場調度管理器 (`BattleManager`)**：解耦個別類別邏輯，統一管控玩家與怪物陣營行動、回合階段及雙欄對齊 CLI UI 面板渲染。
  * **通用技能系統 (`Skill`)**：採用 **16 參數泛型建構子**，透過內部 flag 組合（`isDamage`, `isBuff`, `isDebuff`, `isHeal` 等）一次完成多目標傷害、團隊增益與冷卻計算。
  * **全域多態狀態機 (`EffectType`)**：動態追蹤並自動結算 `PERSEVERANCE` (致死鎖血)、`BURN` (燃燒)、`SPACEGOAT` (假人替身)、`GOLDBULLET` (金色子彈) 等多種戰術狀態。

---

## 🎮 8 大職業特色機制

* 🛡️ **騎士 (Knight)**：高防前排，HP $<50\%$ 觸發 $30\%$ 被動減傷，具備致死傷鎖血 1 點與過期爆發回血。
* 🔮 **法師 (Mage)**：範圍魔法，普攻左右 $50\%$ 濺射，支援燃燒與冰霜之「熱休克 (Thermal Shock)」元素引爆。
* 🏹 **遊俠 (Ranger)**：Boss 增傷 $50\%$，可召喚替身假人吸收傷害，具備 10 發彈射多重箭。
* 🥷 **刺客 (Assassin)**：每 2 回合自動突襲全場最低 HP 敵方，大招斬殺成功重置 CD 與魔力並獲得連擊。
* ✨ **牧師 (Priest)**：自動檢索全隊最小血量比例隊友進行治療，具備單場限次隊友復活。
* 💀 **死靈法師 (Necromancer)**：召喚亡靈大軍隨機追擊，具備分階血量門檻滿血斬殺。
* 🪙 **賞金獵人 (Bounty Hunter)**：持金動態加成攻擊，5 段霰彈槍彈藥分流與全耗魔金色子彈。
* 🪓 **狂戰士 (Berserker)**：受擊按比例自療，受傷觸發全體巨額反擊與低血量極刑爆發。

---

## 👹 特色怪物與首領 AI

* 🎭 **最終首領 (The Deceiver)**：進場 AOE 襲擊，採用 C++11 `<random>` 洗牌算法動態切換三種隱藏姿態（$80\%$ 減傷 / 蓄力反擊 / 每回合 $20\%$ 自療）。
* 💀 **骷髏軍曹 (Skeleton Master Sergeant)**：動態掃瞄場上 5 個槽位，自動補充無賞金骷髏兵。
* 🦇 **吸血鬼蝙蝠 (Vampire Bat)**：每次攻擊前依攻擊力自動恢復自身生命值。

---

## ⚙️ 編譯與執行教學 (How to Build & Run)

### 環境需求
* `g++` (支援 C++11 以上標準)

### 編譯與執行指令
```bash
# 1. 一次性編譯全專案模組
g++ -std=c++11 -O2 main.cpp Player.cpp Skill.cpp Item.cpp Monster.cpp \
BattleManager.cpp Knight.cpp Mage.cpp Ranger.cpp Assassin.cpp \
Priest.cpp Necromancer.cpp BountyHunter.cpp Berserker.cpp \
VampireBat.cpp BlazeShaman.cpp SkeletonMasterSergeant.cpp \
FrostSlime.cpp TheDeceiver.cpp -o RPG_Game

# 2. 執行遊戲
./RPG_Game
```

### 👥 團隊開發成員 (Contributors)
1. 孫逸明

2. 宋金日

3. 王懷賢

4. 吳浤銓