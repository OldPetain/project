#include <SDL2/SDL.h>
#include "../include/enemy.h"
#include "../include/player.h"
#include "../include/bullet.h"

//初始化敌人
void initEnemies(Enemy enemies[]) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        enemies[i].rect.x = 800 + i*100;
        enemies[i].rect.y = 100 + i*50;
        enemies[i].rect.w = 18;
        enemies[i].rect.h = 30;
        enemies[i].dx = -2;
        enemies[i].dy = 0;
        enemies[i].active = false;
        enemies[i].direction = 0;//敌人方向为0，向左
        enemies[i].dying = false;//敌人初始化，还没有死亡
        enemies[i].deathTimer = 300; // 死亡动画计时器
    }
}

// 更新敌人状态（位置等）
void updateEnemies(Enemy enemies[], Player* player, Bullet bullets[], int max_bullets) { 
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].rect.x <= SCREEN_WIDTH && enemies[i].rect.x >= 0) { // 如果敌人已经部分进入屏幕，则开始活动
            enemies[i].active = true;
        }
        if (enemies[i].active) {
            if (enemies[i].dying) {
                enemies[i].deathTimer--;
                if (enemies[i].deathTimer <= 0) {
                    enemies[i].active = false; // 敌人彻底消失
                }
            } 
            else {
                if (rand() % 100 < 5) { // 随机发射子弹
                    fireEnemyBullet(player, bullets, enemies, i, max_bullets);
                }
                if(enemies[i].rect.x > player->rect.x){
                    enemies[i].rect.x += enemies[i].dx;//敌人向左移动
                    enemies[i].direction = 0;//敌人方向为0，向左
                }
                else{
                    enemies[i].rect.x -= enemies[i].dx;//敌人向右移动
                    enemies[i].direction = 1;//敌人方向为1，向右
                }
            }      
        }
    } 
}

//碰撞检测
bool checkCollision(SDL_Rect* a, SDL_Rect* b) {
    if (a->x < b->x + b->w && a->x + a->w > b->x &&
        a->y < b->y + b->h && a->y + a->h > b->y) {
        return true;
    }
    return false;
}