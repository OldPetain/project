#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include "../include/graphics.h"
#include "../include/input.h"
#include "../include/player.h"
#include "../include/enemy.h"
#include "../include/bullet.h"

#define FR 30        // 帧率
#define FT 1000 / FR // 帧间隔

int main(int argc, char *argv[])
{
    // 初始化SDL
    if (!initSDL())
    {
        IMG_Quit(); // 释放图形模块
        SDL_Quit(); // 释放SDL
        return -1;
    }

    // 初始化玩家
    Player player;
    initPlayer(renderer, &player);

    // 初始化敌人
    Enemy enemies[MAX_ENEMIES];
    initEnemies(enemies);

    // 游戏主循环
    bool running = true;
    Uint64 start, end;
    int delay;
    while (running)
    {
        // 获取当前时间
        start = SDL_GetTicks64();

        // 处理输入
        running = handleInput(&player, enemies);

        // 更新玩家和敌人
        updatePlayer(&player, playerBullets, MAX_BULLETS);
        updateEnemies(enemies, &player, enemyBullets, MAX_BULLETS);

        // 更新子弹
        // Bullet bullets[], int max_bullets, Player* player, Enemy enemies[], int max_enemies
        updateBullets(playerBullets, MAX_BULLETS, &player, enemies, MAX_ENEMIES);
        updateBullets(enemyBullets, MAX_BULLETS, &player, enemies, MAX_ENEMIES);

        // 检测人物碰撞
        for (int i = 0; i < MAX_ENEMIES; i++)
        {
            if (checkCollision(&player.rect, &enemies[i].rect))
            {
                player.lives--;
                if (player.lives <= 0)
                {
                    running = false;
                }
            }
        }

        // 检测子弹碰撞
        // 玩家子弹打中敌人
        for (int i = 0; i < MAX_BULLETS; i++)
        {
            if (playerBullets[i].active)
            {
                for (int j = 0; j < MAX_ENEMIES; j++)
                {
                    if (enemies[j].active && checkBulletCollision(&playerBullets[i].rect, &enemies[j].rect))
                    {
                        playerBullets[i].active = false;
                        // enemies[j].active = false; // 击中敌人后消失
                        enemies[j].dying = true; // ! 开始倒计时,马上就死咯
                    }
                }
            }
        }
        // 敌人子弹打中玩家
        for (int i = 0; i < MAX_BULLETS; i++)
        {
            if (enemyBullets[i].active && checkBulletCollision(&enemyBullets[i].rect, &player.rect))
            {
                enemyBullets[i].active = false;
                player.lives--; // 玩家生命减少
                if (player.lives <= 0)
                {
                    running = false; // 游戏结束
                }
            }
        }

        // 渲染内容
        render(&player, enemies, playerBullets, enemyBullets);

        // 延迟
        end = SDL_GetTicks64();
        delay = FT - (end - start);
        if (delay > 0)
        {
            SDL_Delay(delay);
        }
    }

    // 清理资源
    closeSDL(&player);
    IMG_Quit();
    SDL_Quit();
    return 0;
}
