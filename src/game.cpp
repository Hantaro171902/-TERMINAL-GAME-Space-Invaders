#include "game.hpp"
#include <chrono>
#include <thread>
#include <cstdlib>
#include <iostream>

using namespace std;

static const int ALIEN_WIDTH    = 6;
static const int ALIEN_HEIGHT   = 2;
static const int GUNNER_WIDTH   = 7;
static const int GUNNER_HEIGHT  = 2;
static const int SHELTER_HEIGHT = 3;
static const int MA_ENTRANCE    = 800;  // keep simple

Game::Game(int _cols, int _rows) : cols(_cols), rows(_rows) {
    fps = 30;
    gunX = cols / 2;
}

Game::~Game() {}

void Game::init() {
    // simple alien layout
    alienCols = (cols / ALIEN_WIDTH) - 4;
    alienRows = (rows / ALIEN_HEIGHT) - 4;
    if (alienCols < 4) alienCols = 4;
    if (alienRows < 4) alienRows = 4;
    alienTable.assign(alienCols * alienRows, 0);
    shields.assign(cols * SHELTER_HEIGHT, ' ');
    resetAliens();
    resetShields();
}

void Game::resetAliens() {
    // fill with 3 types
    for (int x = 0; x < alienCols) alienTable[x] = 30;
    for (int y = 1; y < alienRows / 2; y++) {
        for (int x = 0; x < alienCols; x++) {
            alienTable[y * alienCols + x] = 20;
        }
    }
    for (int y = alienRows / 2; y < alienRows; y++) {
        for (int x = 0; x < alienCols; x++) {
            alienTable[y*alienCols + x] = 10;
        }
    }
    alienCount = alienCols * alienRows;
    alienX = 0;
    alienY = 1;
    paintWait = 5;
    paintRow = alienRows;
    bombs.clear();
    playerBullet.y = 0; // none active
}

void Game::resetShields() {
    for (int i = 0; i < (int)shields.size(); i++) {
        shields[i] = ' ';
    }

    for (int x = 0; x < cols - 10; x += 10) {
        // place a small shelter design vertically
        const char* s0 = "/MMMMM\\";
        const char* s1 = "MMMMMMM";
        const char* s2 = "MMM MMM";
        int base = 0;
        for (int i=0; s0[i]; ++i) {
            shields[(0*cols) + x + i] = s0[i];
        }
        for (int i=0; s1[i]; ++i) {
            shields[(1*cols) + x + i] = s1[i];
        }
        for (int i=0; s2[i]; ++i) {
            shields[(2*cols) + x + i] = s2[i];
        }
    }
}

void Game::run() {
    setTerminalNonBlocking();
    hideCursor();
    init();

    using clock = chrono::steady_clock;
    auto next_frame = clock::now();
    const chrono::milliseconds frame_time(100 / fps);

    while (state != GameState::GAMEOVER) {
        // input
        InputKey k = getInputKey();
        handleInput(k);

        // update
        update();

        // render
        render();

        // simple fixed FPS
        next_frame += frame_time;
        this_thread::sleep_until(next_frame);
    }

    showCursor();
    restoreTerminalBlocking();
    clearTerminal();
    cout << "GAME OVER. SCORE: " << score << endl;
}


void Game::handleInput(InputKey k) {
    if (k == InputKey::LEFT) gunX = max(GUNNER_WIDTH/2, gunX - 2);
    if (k == InputKey::RIGHT) gunX = min(cols - GUNNER_WIDTH/2, gunX + 2);
    if (k == InputKey::SPACE) {
        if (playerBullet.y == 0) {
            playerBullet.x = gunX;
            playerBullet.y = rows - GUNNER_HEIGHT - 1;
        }
    }
    if (k == InputKey::Q) {
        state = GameState::GAMEOVER;
    }
}

void Game::update() {
    timer++;
    // alien animation & movement
    if (--paintWait <= 0) {
        paintWait = max(1, (int)(5 * ((double)alienCount / (alienCols * alienRows))));
        paintRow--;
        if (paintRow <= 0) {
            paintRow = alienRows;
            alienAnim ^= 1;
            // move horizontally
            alienX += alienDirection;
            // check edges
            if (alienX + (alienCols * ALIEN_WIDTH) >= cols || alienX <= 0) {
                alienDirection *= -1;
                moveAliensDown();
            }
        }
    }

    // bombs
    dropBombsRandom();
    updateBombs();

    // missile
    updateMissile();
}


void Game::dropBombsRandom() {
    // occasionally drop bombs from bottom-most alien columns
    if (timer % 8 != 0) return;
    for (int x = 0; x < alienCols; ++x) {
        // find bottom-most alien for column x
        for (int y = alienRows - 1; y >= 0; --y) {
            int idx = y * alienCols + x;
            if (alienTable[idx] > 0) {
                if (rand() % 200 == 0) {
                    int bx = alienX + (x*ALIEN_WIDTH) + (ALIEN_WIDTH/2);
                    int by = alienY + (y*ALIEN_HEIGHT) + ALIEN_HEIGHT;
                    addBomb(bx, by);
                }
                break;
            }
        }
    }
}

void Game::addBomb(int x, int y) {
    bombs.emplace_back(x,y);
}

void Game::updateBombs() {
    for (int i = 0; i < (int)bombs.size();) {
        bombs[i].y += 1;
        // collision with player
        if (bombs[i].y >= rows - GUNNER_HEIGHT
            && abs(bombs[i].x - gunX) < (GUNNER_WIDTH/2)) {
            // explode player
            lives--;
            if (lives <= 0) { state = GameState::GAMEOVER; return; }
            bombs.erase(bombs.begin()+i);
            continue;
        }
        // collision with shields
        if (bombs[i].y >= rows - GUNNER_HEIGHT - SHELTER_HEIGHT) {
            int shieldRow = bombs[i].y - (rows - GUNNER_HEIGHT - SHELTER_HEIGHT);
            if (shieldRow >=0 && shieldRow < SHELTER_HEIGHT) {
                int idx = shieldRow * cols + bombs[i].x;
                if (idx >=0 && idx < (int)shields.size() && shields[idx] != ' ') {
                    shields[idx] = ' ';
                    bombs.erase(bombs.begin()+i);
                    continue;
                }
            }
        }
        // out of bounds
        if (bombs[i].y >= rows-1) {
            bombs.erase(bombs.begin()+i);
            continue;
        }
        ++i;
    }
}

void Game::updateMissile() {
    if (playerBullet.y != 0) {
        // move up fast
        playerBullet.clear();
        playerBullet.y -= 2;
        if (playerBullet.y < 1) {
            playerBullet.y = 0;
            return;
        }

        // check collision with aliens
        if (playerBullet.x >= alienX && playerBullet.x < alienX + (alienCols * ALIEN_WIDTH)
            && playerBullet.y >= alienY && playerBullet.y < alienY + (alienRows * ALIEN_HEIGHT)) {
            int rx = (playerBullet.x - alienX) / ALIEN_WIDTH;
            int ry = (playerBullet.y - alienY) / ALIEN_HEIGHT;
            int idx = ry * alienCols + rx;
            if (idx >= 0 && idx < (int)alienTable.size() && alienTable[idx] > 0) {
                score += alienTable[idx];
                // remove alien
                alienTable[idx] = 0;
                alienCount--;
                playerBullet.y = 0;
                // if all dead, reset
                if (alienCount == 0) {
                    resetAliens();
                }
                return;
            }
        }
    }
}

void Game::moveAliensDown() {
    alienY += ALIEN_HEIGHT;
    // if they reach shields or player, set game over
    int lastLine = alienY + (ALIEN_HEIGHT * alienRows) - 1;
    int shieldTop = rows - GUNNER_HEIGHT - SHELTER_HEIGHT - 1;
    if (lastLine >= shieldTop) {
        // push shields to be cleared a row
        for (int x=0;x<cols;x++){
            for (int r=0;r<SHELTER_HEIGHT-1;r++){
                shields[r*cols + x] = shields[(r+1)*cols + x];
            }
            shields[(SHELTER_HEIGHT-1)*cols + x] = ' ';
        }
    }
    if (lastLine >= rows - GUNNER_HEIGHT - 1) {
        state = GameState::GAMEOVER;
    }
}

void Game::render() {
    // minimal: clear and redraw important bits
    clearScreen();

    paintScore();
    paintShelters();
    // draw aliens (simple single-char per alien)
    for (int r = 0; r < alienRows; ++r) {
        for (int c = 0; c < alienCols; ++c) {
            int val = alienTable[r*alienCols + c];
            if (val > 0) {
                int drawx = alienX + c*ALIEN_WIDTH;
                int drawy = alienY + r*ALIEN_HEIGHT;
                move_cursor(drawx, drawy);
                if (val == 30) setTextColor(TextColor::MAGENTA);
                else if (val == 20) setTextColor(TextColor::GREEN);
                else setTextColor(TextColor::CYAN);
                cout << "V";
                resetTextColor();
            }
        }
    }

    paintGunner();

    // bullets
    if (playerBullet.y != 0) playerBullet.draw();

    // bombs
    for (auto &b : bombs) {
        move_cursor(b.x, b.y);
        cout << ( (b.anim++ & 1) ? '|' : '\\' );
    }

    // flush cursor to bottom
    move_cursor(0, rows);
    cout.flush();
}

void Game::paintScore() {
    move_cursor(0,0);
    setTextColor(TextColor::YELLOW);
    cout << "Score: " << score << "   Lives: " << lives;
    resetTextColor();
}

void Game::paintGunner() {
    move_cursor(gunX - (GUNNER_WIDTH/2), rows - GUNNER_HEIGHT);
    setTextColor(TextColor::WHITE);
    cout << "  mAm  ";
    move_cursor(gunX - (GUNNER_WIDTH/2), rows - GUNNER_HEIGHT + 1);
    cout << " MAZAM ";
    resetTextColor();
}

void Game::paintShelters() {
    for (int r = 0; r < SHELTER_HEIGHT; ++r) {
        move_cursor(0, rows - GUNNER_HEIGHT - SHELTER_HEIGHT + r);
        for (int x = 0; x < cols; ++x) {
            char ch = shields[r*cols + x];
            cout << ch;
        }
    }
}