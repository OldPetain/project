// common.h
#ifndef COMMON_H
#define COMMON_H

#include <SDL2/SDL.h>

#define MAX_ENEMIES 5   // 最大敌人数
#define SCREEN_WIDTH 1200  // 屏幕宽度
#define SCREEN_HEIGHT 765  // 屏幕高度
#define MAP_WIDTH 11374   // 地图宽度
#define MAP_EDGE_WIDTH 200  // 地图边缘宽度

#define MAX_BULLETS 50  // 最大子弹数
#define BULLET_SPEED 10  // 子弹速度
#define PLAYER_JUMP_SPEED 60   // 玩家跳跃速度
#define PLAYER_GRAVITY 15   // 玩家重力加速度

typedef struct {
    SDL_Rect rect;   // 玩家矩形
    int dx, dy;      // 玩家移动方向
    int speed;       // 移动速度
    bool shooting;   // 是否射击
    int lives;       // 玩家生命数
    bool jumping;    // 是否跳跃
    int direction;   // 玩家方向
} Player;

typedef struct {
    SDL_Rect rect;  // 敌人矩形
    int dx, dy;     // 敌人速度
    bool active;    // 敌人是否活动
    int direction;  // 敌人方向
} Enemy;

typedef struct {
    SDL_Rect rect;  // 子弹矩形
    int dx, dy;     // 子弹速度
    bool active;    // 子弹是否激活
} Bullet;


// 声明全局变量
extern SDL_Rect SourceRect;
extern Bullet playerBullets[];//玩家子弹
extern Bullet enemyBullets[];//敌人子弹

#endif // COMMON_H
