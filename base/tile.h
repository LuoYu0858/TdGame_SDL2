// Created by LuoYu on 2026/6/9.
#pragma once

#include <vector>

#define SIZE_TILE 48    // 瓦片尺寸

// 瓦片
struct Tile {
    enum class Direction {
        None = 0,   // 无方向 (非敌人行进路线)
        Up,         // 向上
        Down,       // 向下
        Left,       // 向左
        Right       // 向右
    };

    int terrain = 0;                        // 地形瓦片索引
    int decoration = -1;                    // 装饰瓦片索引
    int special_flag = -1;                  // 特殊标志 (-1 -> 无标志, 0 -> 房屋, >0 -> 刷怪)
    Direction direction = Direction::None;  // 该瓦片上敌人的行进方向

    bool has_tower = false;                 // 当前瓦片是否放置防御塔
};

typedef std::vector<std::vector<Tile>> TileMap; // 瓦片地图 (动态二维Tile数组)