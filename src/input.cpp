#include <SDL2/SDL.h>
#include <stdbool.h>
#include "../include/input.h"
#include "../include/common.h"
#include "../include/player.h"
#include "../include/enemy.h"
#include "../include/bullet.h"

// 全局事件
SDL_Event event;

bool handleInput(Player* player, Enemy enemies[]) {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            // 退出事件
            return false;
        } else if (event.type == SDL_KEYDOWN) {
            // 按键按下
            switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_W:
                    if(player -> direction == 6){
                        player->state = STAND_UP_LEFT;
                    }
                    else {
                        if(player -> direction == 2){
                            player->state = STAND_UP_RIGHT;
                        }
                    }
                    player->frameCount = 1;  // 假设站立有1帧
                    player->direction = 0;//面向上
                    //player->dy = -player->speed;
                    break;
                case SDL_SCANCODE_A:
                    player->direction = 6;//面向左
                    player->state = WALK_LEFT; // 设置为向左跑动状态
                    player->frameCount = 5;  // 假设跑动有5帧
                    if(player->rect.x > MAP_EDGE_WIDTH){
                        player->dx = -player->speed;
                    }
                    else{
                        SourceRect.x -= player->speed;
                        if(SourceRect.x < 0){
                            SourceRect.x = 0;
                            //如果靠近地图左边界，不移动地图，还是移动角色
                            player->dx = -player->speed;
                        } 
                        else {
                            //不移动角色，移动地图和敌人
                            for (int i = 0; i < MAX_ENEMIES; i++){
                                if(enemies[i].active){
                                    enemies[i].rect.x += player->speed;
                                }
                            }                       
                        }
                    }
                    break;
                case SDL_SCANCODE_S:
                    player->direction = 4;//面向下
                    //player->dy = player->speed;
                    break;
                case SDL_SCANCODE_D:
                    player->state = WALK_RIGHT; // 设置为向右跑动状态
                    player->direction = 2;//面向右
                    player->frameCount = 5;  // 假设跑动有5帧
                    if(player->rect.x +player->rect.w < SCREEN_WIDTH - MAP_EDGE_WIDTH){
                        player->dx = player->speed;
                    }
                    else{
                        SourceRect.x += player->speed;                       
                        if(SourceRect.x + SCREEN_WIDTH > MAP_WIDTH){
                            //如果靠近地图右边界，不移动地图，还是移动角色
                            SourceRect.x = MAP_WIDTH - SCREEN_WIDTH;
                            player->dx = player->speed;
                        } 
                        else {
                            //不移动角色，移动地图和敌人
                            for (int i = 0; i < MAX_ENEMIES; i++){
                                if(enemies[i].active){
                                    enemies[i].rect.x -= player->speed;
                                }
                            }
                        }                        
                    }
                    break;
                case SDL_SCANCODE_SPACE:
                    player->jumping = true;//???                   
                    break;
                case SDL_SCANCODE_J:
                    player->shooting = true;
                    firePlayerBullet(player, playerBullets, MAX_BULLETS);//???
                    break;
                default:    
                    break;
            }
        } else if (event.type == SDL_KEYUP) {
            // 按键释放
            switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_W:
                    if(player->state == STAND_UP_LEFT){
                        player->direction = 6;//面向左
                    }
                    else {
                        if(player->state == STAND_UP_RIGHT){
                            player->direction = 2;//面向右
                        }
                    }
                    player->dy = 0;//???
                    break;
                case SDL_SCANCODE_S:
                    player->dy = 0;//???
                    break;
                case SDL_SCANCODE_A:
                    player->state = STAND_LEFT;//面向左
                    player->frame = 0; // 重置动画帧索引
                    player->frameCount = 1;  // 假设站立有1帧
                    player->dx = 0;
                    break;
                case SDL_SCANCODE_D:
                    player->state = STAND_RIGHT; // 面向右
                    player->frame = 0; // 重置动画帧索引
                    player->frameCount = 1;  // 假设站立有1帧
                    player->dx = 0;
                    break;
                case SDL_SCANCODE_SPACE:
                    break;
                case SDL_SCANCODE_J:
                    player->shooting = false;//???
                    break;
                default:
                    break;
            }
        }
    }
    return true;
}