#include "../include/player.h"
#include "../include/bullet.h"

void initPlayer(Player* player) {
    player->rect.x = SCREEN_WIDTH / 2 - 100;
    player->rect.y = SCREEN_HEIGHT / 2 - 60;
    player->rect.w = 18;
    player->rect.h = 30;
    player->dx = 0;
    player->dy = 0;
    player->speed = 5;
    player->shooting = false;
    player->lives = 30;
    player->jumping = false;
    player->direction = 2; // 0:向上，1：向右上 2:向右 3：向右下 4：向下 5：向左下 6：向左 7：向左上
}

void updatePlayer(Player* player, Bullet bullets[], int max_bullets) {
    // 跳跃逻辑
    if (player->jumping) {
        player->rect.y -= PLAYER_JUMP_SPEED;
        player->jumping = false;
    } else {
        player->rect.y += PLAYER_GRAVITY;
        if (player->rect.y + player->rect.h > SCREEN_HEIGHT/2) {//？？？
            player->rect.y = SCREEN_HEIGHT/2 - player->rect.h; // 落地
        }
    }
    // 更新玩家位置
    player->rect.x += player->dx;


    // 边界检测
    if (player->rect.x < 0) player->rect.x = 0;
    if (player->rect.x > SCREEN_WIDTH - player->rect.w) player->rect.x = SCREEN_WIDTH - player->rect.w;
    if (player->rect.y < 0) player->rect.y = 0;
    if (player->rect.y > SCREEN_HEIGHT - player->rect.h) player->rect.y = SCREEN_HEIGHT - player->rect.h;

    // 发射子弹
    if (player->shooting) {
        firePlayerBullet(player, bullets, max_bullets);
    }
}