#include <SDL2/SDL.h>
#include <stdbool.h>
#include "../include/input.h"
#include "../include/common.h"
#include "../include/player.h"
#include "../include/enemy.h"
#include "../include/bullet.h"

// 全局事件
SDL_Event event;

bool handleInput(Player *player, Enemy enemies[])
{
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            // 退出事件
            return false;
        }
        else if (event.type == SDL_KEYDOWN)
        {
            // 按键按下
            switch (event.key.keysym.scancode)
            {
            case SDL_SCANCODE_W:
                if (player->direction == 6)
                {
                    player->state = STAND_UP_LEFT;
                }
                else
                {
                    if (player->direction == 2)
                    {
                        player->state = STAND_UP_RIGHT;
                    }
                }
                player->frameCount = 1; // 假设站立有1帧
                player->direction = 0;  // 面向上
                // player->dy = -player->speed;
                break;
            case SDL_SCANCODE_A:
                player->direction = 6;     // 面向左
                player->state = WALK_LEFT; // 设置为向左跑动状态
                player->frameCount = 5;    // 假设跑动有5帧
                if (player->rect.x > MAP_EDGE_WIDTH)
                {
                    player->dx = -player->speed;
                }
                else
                {
                    SourceRect.x -= player->speed;
                    if (SourceRect.x < 0)
                    {
                        SourceRect.x = 0;
                        // 如果靠近地图左边界，不移动地图，还是移动角色
                        player->dx = -player->speed;
                    }
                    else
                    {
                        // 不移动角色，移动地图和敌人
                        for (int i = 0; i < MAX_ENEMIES; i++)
                        {                                               
                            enemies[i].rect.x += player->speed;
                        }
                    }
                }
                break;
            case SDL_SCANCODE_S:
                // ! start
                if (player->direction == 2)
                {
                    player->state = LIE_DOWN_RIGHT;
                    player->frameCount = 1;
                }
                else if (player->direction == 6)
                {
                    player->state = LIE_DOWN_LEFT;
                    player->frameCount = 1;
                }
                player->direction = 4; // 面向下
                // ! End
                // ! 日志：增加了LIE_DOWN_RIGHT、SHOOTING_RUN_LEFT、DIE等状态表示趴下、开枪同时移动、死亡
                player -> dx = 0; // 停止移动
                // player->dy = player->speed;
                break;
            case SDL_SCANCODE_D:
                player->state = WALK_RIGHT; // 设置为向右跑动状态
                player->direction = 2;      // 面向右
                player->frameCount = 5;     // 假设跑动有5帧
                if (player->rect.x + player->rect.w < SCREEN_WIDTH - MAP_EDGE_WIDTH)
                {
                    player->dx = player->speed;
                }
                else
                {
                    SourceRect.x += player->speed;       
                    if (SourceRect.x + SCREEN_WIDTH > MAP_WIDTH)
                    {
                        // 如果靠近地图右边界，不移动地图，还是移动角色
                        SourceRect.x = MAP_WIDTH - SCREEN_WIDTH;
                        player->dx = player->speed;
                    }
                    else
                    {
                        // 不移动角色，移动地图和敌人
                        for (int i = 0; i < MAX_ENEMIES; i++)
                        {
                            enemies[i].rect.x -= player->speed;//死的和活的敌人都要移动，活的使它与地图同步，死的固定在复活的位置
                        }                        
                    }
                }
                break;
            case SDL_SCANCODE_SPACE:
                // 如果角色处于跳跃状态，则取消跳跃
                if (player->is_jumping == false){
                    player->jumping = true; //???
                    if(player->state == STAND_LEFT || player->state == WALK_LEFT)
                    {
                        player->direction = 6;
                        player->state = JUMP_LEFT;
                        player->frameCount = 4;
                    }
                    if(player->state == STAND_RIGHT || player->state == WALK_RIGHT)
                    {
                        player->direction = 2;
                        player->state = JUMP_RIGHT;
                        player->frameCount = 4;
                    }
                }                
                break;
            case SDL_SCANCODE_J:
                player->shooting = true;
                firePlayerBullet(player, playerBullets, MAX_BULLETS); //???
                break;
            default:
                break;
            }
        }
        else if (event.type == SDL_KEYUP)
        {
            // 按键释放
            switch (event.key.keysym.scancode)
            {
            case SDL_SCANCODE_W:
                if (player->state == STAND_UP_LEFT)
                {
                    player->direction = 6;      // 面向左
                    player->state = STAND_LEFT; // 面向左
                    player->frameCount = 1;     // 假设站立有1帧
                }

                if (player->state == STAND_UP_RIGHT)
                {
                    player->direction = 2;       // 面向右
                    player->state = STAND_RIGHT; // 面向右
                    player->frameCount = 1;      // 假设站立有1帧
                }
                player->dy = 0; //???
                break;
            case SDL_SCANCODE_S:                
                // ! start
                if (player->state == LIE_DOWN_LEFT)
                {
                    player->state = STAND_LEFT;
                    player->direction = 6;
                    player->frameCount = 1;
                }

                if (player->state == LIE_DOWN_RIGHT)
                {
                    player->state = STAND_RIGHT;
                    player->direction = 2;
                    player->frameCount = 1;
                }
                // ! End
                // !日志：抬起s时，角色状态更新为站立
                player->dy = 0; // ???
                break;
            case SDL_SCANCODE_A:
                player->state = STAND_LEFT; // 面向左
                player->frame = 0;          // 重置动画帧索引
                player->frameCount = 1;     // 假设站立有1帧
                player->dx = 0;
                break;
            case SDL_SCANCODE_D:
                player->state = STAND_RIGHT; // 面向右
                player->frame = 0;           // 重置动画帧索引
                player->frameCount = 1;      // 假设站立有1帧
                player->dx = 0;
                break;
            case SDL_SCANCODE_SPACE:                
                break;
            case SDL_SCANCODE_J:
                player->shooting = false; //???
                break;
            default:
                break;
            }
        }
    }
    return true;
}