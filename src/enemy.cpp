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
    }
}

// 更新敌人状态（位置等）
void updateEnemies(Enemy enemies[], Player* player, Bullet bullets[], int max_bullets) { 
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].active && !enemies[i].hit) {
            if (rand() % 100 < 5) { // 随机发射子弹
                fireEnemyBullet(player, bullets, enemies, i, max_bullets);
            }
            if (enemies[i].rect.x > player->rect.x) {
                enemies[i].rect.x += enemies[i].dx; // 敌人向左移动
                enemies[i].direction = 0; // 敌人方向为0，向左
            } else {
                enemies[i].rect.x -= enemies[i].dx; // 敌人向右移动
                enemies[i].direction = 1; // 敌人方向为1，向右
            }
            if (enemies[i].rect.x < 0 || enemies[i].rect.x > SCREEN_WIDTH) {
                enemies[i].original_x = enemies[i].rect.x;//??? // 记录敌人在地图上的位置
                enemies[i].active = false; // 敌人脱离屏幕后不再活动
            }
        } else if (!enemies[i].active) {
            if (enemies[i].original_x >= 0 && enemies[i].original_x <= SCREEN_WIDTH) {//???
                enemies[i].rect.x = enemies[i].original_x; // 恢复敌人位置
                enemies[i].active = true; // 恢复敌人活动
                enemies[i].hit = false; //??? // 重置敌人被击中状态
            }
        }
      
        
        /*     
        if (enemies[i].rect.x <= SCREEN_WIDTH && enemies[i].rect.x >= 0) { // 如果敌人已经部分进入屏幕，则开始活动
            enemies[i].active = true;
        }
        if (enemies[i].active) {
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
            if (enemies[i].rect.x < 0) {
                enemies[i].active = false;
            }       
        }
        */
    } 
}

void handleEnemyHit(Enemy enemies[], int enemy_index) {
    if (enemy_index >= 0 && enemy_index < MAX_ENEMIES) {
        enemies[enemy_index].hit = true;
        enemies[enemy_index].active = false; // 敌人被击中后不再活动
    }
}

bool checkBulletCollision(SDL_Rect* bullet_rect, SDL_Rect* enemy_rect,Enemy enemies[],int i) {
    if (SDL_HasIntersection(bullet_rect, enemy_rect)) {
        // 处理敌人受击
        handleEnemyHit(enemies, i); // 假设你有一个 enemies 数组和当前敌人的索引 i
        return true;
    }
    return false;
}


//碰撞检测
bool checkCollision(SDL_Rect* a, SDL_Rect* b) {
    if (a->x < b->x + b->w && a->x + a->w > b->x &&
        a->y < b->y + b->h && a->y + a->h > b->y) {
        return true;
    }
    return false;
}