#ifndef MONSTER_H
#define MONSTER_H

namespace RPG_Colaborate
{
    class Monster
    {
    private:
        /* data */
    public:
        Monster(/* args */);
        ~Monster();

        void takeDamage(int damage) {
            std::cout << "[測試用] 怪物受到了 " << damage << " 點傷害！\n";
        }
    };
}

#endif