#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../include/enemy.h"
#include "../include/player.h"
#include "../include/bullet.h"
#include "../include/common.h"
#include <iostream>

// 初始化敌人
void initEnemies(Enemy enemies[])
{
    SDL_Texture *enemies_texture = IMG_LoadTexture(renderer, "../assets/enemy.png"); // 加载敌人图片
    if (!enemies_texture)
    {
        SDL_Log("Failed to load enemy texture");
        exit(1);
    }
    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        enemies[i].rect.x = 800 + i * 100;
        enemies[i].rect.y = 100 + i * 50;
        enemies[i].rect.w = 150;
        enemies[i].rect.h = 150;
        enemies[i].dx = -2;
        enemies[i].dy = 0;
        enemies[i].active = false;
        enemies[i].direction = 0;    // 敌人方向为0，向左
        enemies[i].dying = false;    // 敌人初始化，还没有死亡
        enemies[i].deathTimer = 300; // 死亡动画计时器

        enemies[i].texture = enemies_texture; // 敌人图片
        enemies[i].state = enemy_state::RUN_LEFT;
        enemies[i].current_frame = 0;
        enemies[i].frame_count = 4;
        enemies[i].frame_width = 150;
        enemies[i].frame_height = 150;
        enemies[i].animation_speed = 3;
        enemies[i].frame_timer = 0;

        std::cout << "init enemy " << i << "now || size:" << enemies[i].frame_height << "x" << enemies[i].frame_width << std::endl;
    }
}

// 更新敌人状态（位置等）
void updateEnemies(Enemy enemies[], Player *player, Bullet bullets[], int max_bullets)
{
    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        enemies[i].frame_timer++;
        if (enemies[i].frame_timer >= enemies[i].animation_speed)
        {
            std::cout << "update***enemy* before " << i << " current_frame:" << enemies[i].current_frame << std::endl; // test
            std::cout << "update***enemy* before " << i << " all frame:" << enemies[i].frame_count << std::endl;       // test

            // 这里没有问题！
            enemies[i].current_frame = (enemies[i].current_frame + 1) % enemies[i].frame_count;
            enemies[i].frame_timer = 0;

            std::cout << "update***enemy* after" << i << " current_frame:" << enemies[i].current_frame << std::endl; // test
        }

        if (enemies[i].rect.x <= SCREEN_WIDTH && enemies[i].rect.x >= 0)
        { // 如果敌人已经部分进入屏幕，则开始活动
            enemies[i].active = true;
        }
        if (enemies[i].active)
        {
            bool flag = true;
            if (enemies[i].dying)
            {
                if (flag) // 这里添加了个开关，让下面的只执行一次
                {
                    enemies[i].state = enemy_state::ENEMY_DIE;
                    enemies[i].current_frame = 0;
                    enemies[i].frame_count = 2;
                    flag = false;
                }
                enemies[i].deathTimer--;
                if (enemies[i].deathTimer <= 0)
                {
                    enemies[i].active = false; // 敌人彻底消失
                    enemies[i].dying = false;  // 敌人死亡状态复位
                }
            }
            else
            {
                if (rand() % 100 < 5)
                { // 随机发射子弹
                    fireEnemyBullet(player, bullets, enemies, i, max_bullets);
                }
                if (enemies[i].rect.x > player->rect.x)
                {
                    enemies[i].rect.x += enemies[i].dx;       // 敌人向左移动
                    enemies[i].direction = 0;                 // 敌人方向为0，向左
                    enemies[i].state = enemy_state::RUN_LEFT; // 敌人状态为向左跑
                    enemies[i].frame_count = 4;
                }
                else
                {
                    enemies[i].rect.x -= enemies[i].dx;        // 敌人向右移动
                    enemies[i].direction = 1;                  // 敌人方向为1，向右
                    enemies[i].state = enemy_state::RUN_RIGHT; // 敌人状态为向右跑
                    enemies[i].frame_count = 4;
                }
            }
        }
    }
}

// 碰撞检测
bool checkCollision(SDL_Rect *a, SDL_Rect *b)
{
    if (a->x < b->x + b->w && a->x + a->w > b->x &&
        a->y < b->y + b->h && a->y + a->h > b->y)
    {
        return true;
    }
    return false;
}