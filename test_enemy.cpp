#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../include/enemy.h"
#include "../include/player.h"
#include "../include/bullet.h"
#include "../include/common.h"
#include <stdlib.h>
#include <time.h>

// 初始化敌人
void initEnemies(Enemy enemies[], SDL_Renderer *renderer, int enemyCount)
{
    srand(time(0));
    SDL_Texture *enemyTexture = IMG_LoadTexture(renderer, "assets/enemy.png"); // 加载敌人图片
    if (!enemyTexture)
    {
        SDL_Log("Failed to load enemy texture");
        return;
    }

    for (int i = 0; i < enemyCount; i++)
    {
        enemies[i].rect.x = SCREEN_WIDTH + rand() % 300;   // 随机X位置
        enemies[i].rect.y = rand() % (SCREEN_HEIGHT - 50); // 随机Y位置
        enemies[i].rect.w = 18;
        enemies[i].rect.h = 30;
        enemies[i].dx = -2;
        enemies[i].dy = 0;
        enemies[i].active = false;
        enemies[i].direction = 0; // 初始方向
        enemies[i].dying = false;
        enemies[i].deathTimer = 30;                                               // 死亡动画时长
        enemies[i].texture = enemyTexture;                                        // 共享纹理
        enemies[i].type = rand() % 2 == 0 ? ENEMY_TYPE_NORMAL : ENEMY_TYPE_ELITE; // 随机类型
        enemies[i].health = enemies[i].type == ENEMY_TYPE_NORMAL ? 1 : 3;         // 设置血量
    }
}

// 更新敌人状态
void updateEnemies(Enemy enemies[], int enemyCount, Player *player, Bullet bullets[], int maxBullets)
{
    for (int i = 0; i < enemyCount; i++)
    {
        Enemy *enemy = &enemies[i];

        if (!enemy->active && enemy->rect.x < SCREEN_WIDTH)
        {
            enemy->active = true; // 激活敌人
        }

        if (enemy->active)
        {
            // 如果敌人处于死亡状态，处理死亡逻辑
            if (enemy->dying)
            {
                enemy->deathTimer--;
                if (enemy->deathTimer <= 0)
                {
                    enemy->active = false; // 敌人消失
                }
                continue;
            }

            // 追踪玩家逻辑
            if (enemy->type == ENEMY_TYPE_ELITE)
            {
                if (enemy->rect.x > player->rect.x)
                {
                    enemy->rect.x -= abs(enemy->dx); // 向玩家靠近
                }
                if (enemy->rect.y > player->rect.y)
                {
                    enemy->rect.y -= 1; // 向玩家靠近
                }
                else if (enemy->rect.y < player->rect.y)
                {
                    enemy->rect.y += 1;
                }
            }
            else
            {
                // 普通敌人随机移动
                if (rand() % 100 < 5)
                {
                    enemy->rect.y += (rand() % 2 == 0 ? -5 : 5); // 随机上下移动
                }
                enemy->rect.x += enemy->dx; // 左右移动
            }

            // 边界检测
            if (enemy->rect.x + enemy->rect.w < 0)
            {
                enemy->active = false; // 超出屏幕范围
            }

            // 发射子弹逻辑
            if (rand() % 100 < (enemy->type == ENEMY_TYPE_ELITE ? 10 : 5))
            { // 精英敌人更高概率
                fireEnemyBullet(player, bullets, enemies, i, maxBullets);
            }
        }
    }
}

// 渲染敌人
void renderEnemies(Enemy enemies[], int enemyCount, SDL_Renderer *renderer)
{
    SDL_Rect srcRect = {0, 0, 18, 30}; // 定义单帧矩形大小

    for (int i = 0; i < enemyCount; i++)
    {
        if (enemies[i].active)
        {
            SDL_RenderCopy(renderer, enemies[i].texture, &srcRect, &enemies[i].rect);

            // 渲染死亡动画（红色闪烁）
            if (enemies[i].dying)
            {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                SDL_RenderFillRect(renderer, &enemies[i].rect);
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // 恢复颜色
            }
        }
    }
}

// 释放敌人资源
void cleanupEnemies(Enemy enemies[], int enemyCount)
{
    if (enemyCount > 0 && enemies[0].texture)
    {
        SDL_DestroyTexture(enemies[0].texture); // 释放纹理
    }
}
