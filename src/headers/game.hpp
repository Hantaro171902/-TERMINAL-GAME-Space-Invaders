#pragma once 

#include <vector>
#include <memory>
#include "invader.hpp"
#include "bullet.hpp"
#include "bullet.hpp"
#include "cursor_input.hpp"
#include "utils.hpp"

enum class GameState { 
    INTRO, 
    PLAY, 
    WAIT, 
    EXPLORE, 
    GAMEOVER
};

struct Bomb {
    int x, y;
    int anim = 0;
    Bomb(int _x, int _y) : x(_x), y(_y) {}
};

class Game {
public:
    Game(int cols, int rows);
    ~Game();

    void init();
    void run();
    void update();
    void render();
    void handleInput(InputKey k);
    void resetAliens();
    void resetShields();
private:
    int cols, rows;
    int score = 0;
    int lives = 3;
    GameState state = GameState::INTRO;
    int timer = 0;
    int fps = 30;

    // player/gunner
    int gunX;
    Bullet playerBullet{0,0};

    // aliens grid
    int alienCols, alienRows;
    std::vector<int> alienTable;
    int alienX = 0, alienY = 0;
    int alienDirection = 1;
    int alienAnim = 0;
    int alienCount = 0;
    int paintWait = 5;
    int paintRow = 0;

    // shields
    std::string shields;

    // bombs
    std::vector<Bomb> bombs;

    //helper
    void moveAliensDown();
    void dropBombsRandom();
    void updateBombs();
    void updateMissile();
    void paintScore();
    void paintGunner();
    void paintShelters();
    void paintAliRow(int row);
}
