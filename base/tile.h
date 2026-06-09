// Created by LuoYu on 2026/6/9.
#pragma once

#include <vector>

#define SIZE_TILE 48    // 瓦片尺寸

// 瓦片
struct Tile {
    enum class Direction {
        None = 0,
        Up,
        Down,
        Left,
        Right
    };

    int terrain = 0;                        // 地形瓦片
    int decoration = -1;                    // 装饰瓦片
    int special_flag = -1;                  // 特殊标志(-1 -> 非标志, 0 -> 房屋, 1.. -> 刷怪)
    Direction direction = Direction::None;  // 行进方向

    bool has_tower = false;                 // 当前瓦片是否放置防御塔
};

typedef std::vector<std::vector<Tile>> TileMap; // 瓦片地图