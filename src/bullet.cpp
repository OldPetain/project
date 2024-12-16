#include "../include/bullet.h"
#include "../include/player.h"
#include "../include/enemy.h"
#include "../include/common.h"

#include <stdio.h>

// 初始化子弹数组
void initBullets(Bullet bullets[]) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        bullets[i].active = false;  // 所有子弹初始未激活
    }
}

// 玩家发射子弹
void firePlayerBullet(Player* player, Bullet bullets[],int max_bullets) {
    for (int i = 0; i < max_bullets; i++) {
        if (!bullets[i].active) {
            bullets[i].active = true;
            bullets[i].rect.x = player->rect.x + (player->direction == 0 ? player->rect.w : -10);
            bullets[i].rect.y = player->rect.y + player->rect.h / 2;
            switch (player->direction) {
                case 0://向上
                    bullets[i].dy = -BULLET_SPEED;
                    bullets[i].dx = 0;
                    break;
                case 2://向右
                    bullets[i].dy = 0; 
                    bullets[i].dx = BULLET_SPEED;
                    break;
                case 4://向下
                    bullets[i].dy = BULLET_SPEED;
                    bullets[i].dx = 0;
                    break;
                case 6://向左
                    bullets[i].dy = 0; 
                    bullets[i].dx = -BULLET_SPEED;
                    break;
            }
            break;
        }
    }
}

// 敌人发射子弹
void fireEnemyBullet(Player* player, Bullet bullets[],Enemy enemies[],int i,int max_bullets) {
    for (int j = 0; j < max_bullets; j++) {
        if (!bullets[j].active) {
            bullets[j].active = true;
            bullets[j].rect.x = enemies[i].rect.x + (enemies[i].direction > 0 ? enemies[i].rect.w : -10);
            bullets[j].rect.y = enemies[i].rect.y + enemies[i].rect.h / 2;
            bullets[j].dx = (enemies[i].direction > 0 ? BULLET_SPEED : -BULLET_SPEED);
            bullets[j].dy = 0;
            break;
        }
    }
}

// 更新子弹位置
void updateBullets(Bullet bullets[], int max_bullets, Player* player, Enemy enemies[], int max_enemies) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            // 更新子弹位置
            bullets[i].rect.x += bullets[i].dx;
            bullets[i].rect.y += bullets[i].dy;

            // 检查子弹是否击中敌人
            for (int j = 0; j < max_enemies; j++) {
                if (enemies[j].active && checkBulletCollision(&bullets[i].rect, &enemies[j].rect)) {
                    bullets[i].active = false; // 子弹被击中后不再活动
                }
            }

            // 子弹超出屏幕范围后失效
            if (bullets[i].rect.y < 0 || bullets[i].rect.y > SCREEN_HEIGHT ||
                bullets[i].rect.x < 0 || bullets[i].rect.x > SCREEN_WIDTH) {
                bullets[i].active = false;
            }

            // 碰撞检测（对玩家或敌人）
            // 1. 玩家子弹击中敌人
            /*
            for (int j = 0; j < max_enemies; j++) {
                if (checkCollision(&bullets[i].rect, &enemies[j].rect)) {
                    bullets[i].active = false;
                    enemies[j].active = false; // 简单处理：敌人消失
                    break;
                }
            }
            */
            // 2. 敌人子弹击中玩家
            if (checkCollision(&bullets[i].rect, &player->rect)) {
                bullets[i].active = false;
                player->lives--;
            }
        }
    }
}

// 检测子弹是否碰撞目标
bool checkBulletCollision(SDL_Rect* bulletRect, SDL_Rect* targetRect) {
    if (bulletRect->x < targetRect->x + targetRect->w &&
        bulletRect->x + bulletRect->w > targetRect->x &&
        bulletRect->y < targetRect->y + targetRect->h &&
        bulletRect->y + bulletRect->h > targetRect->y) {
        return true;
    }
    return false;
}
