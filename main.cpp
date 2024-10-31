#include "Game.h"
#include <cstdlib>
#include <ctime>

int main() { //게임 실행 화면ㄴ
    std::srand(static_cast<unsigned int>(std::time(0)));
    Game game;
    game.run();
    return 0;
}
 