#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>
#include "player.h"
#include "enemy.h"
#include "common.h"

// 处理输入事件
bool handleInput(Player* player,Enemy enemies[]);

#endif