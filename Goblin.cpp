#include "Goblin.h"

namespace RPG_Colaborate {
    // 定位小怪，血量略高(80)，給多一點點金幣
    Goblin::Goblin() 
        : Monster("Goblin", 80, 10, 15, 5, 0) {
    }
}