#include "Slime.h"

namespace RPG_Colaborate {
    // 參數順序: 名字, 血量, 攻擊力, 獎勵金幣, 閃避率, 防禦力
    Slime::Slime() 
        : Monster("Slime", 50, 10, 10, 0, 0) {
    }
}