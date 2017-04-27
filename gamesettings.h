#ifndef GAMESETTINGS_H
#define GAMESETTINGS_H

#include <QString>

// banana settings
#define BANANA_SIZE 20
#define BANANAS_SPEED 30 // in px per frame
#define BANANA_SIZE 20
#define INV_FPS 17      // 1/fps = 1/60 ~= 17ms

// display settings
#define VIEW_HEIGHT 800
#define VIEW_WIDTH 600
#define BOTTOM_LINE_HEIGHT 750
#define TOP_LINE_HEIGHT 50
#define PLAYER_POSX 275
#define PLAYER_POSY 700
#define PLAYER_SIZE 50
#define MAX_ROTATION 80 // of the axis while rotating
#define AXIS_OFFSET 90  // since its vertical at the initialization
#define MAX_BLOCKLINE 12
#define BLOCK_DESTR_POSX 425

// player settings
#define BANANAS_DEFAULT 1;

// timer settings
#define REFRESH_COLLISION 17 // match the FPS
#define MONITORING_INTERVAL 150
#define THROWING_INTERVAL 200

// block settings
#define BLOCK_SIZE 50
#define MORE_BANANA_BONUS 1
#define PADDLE_BONUS 2
#define MORE_LIFE_BANANA_BONUS 3

// pop-up and messageBoxs settings
#define GAME_OVER_WIDTH 200

typedef struct Qhighscore {
    QString playerName;
    int score;
} Qhighscore;

#endif // GAMESETTINGS_H
