// Created by LuoYu on 2026/6/10.
#pragma once

#include "tile.h"

#include <SDL.h>
#include <vector>

// 路径
class Route {
public:
    using IdxList = std::vector<SDL_Point>; // 路径上的每个网格在地图上的索引

public:
    Route() = default;

    /**
     * @param map 使用的瓦片地图
     * @param idx_origin 从该索引出生点执行寻路操作
     */
    Route(const TileMap& map, const SDL_Point& idx_origin);

    ~Route() = default;

    [[nodiscard]] const IdxList& get_idx_list() const;


private:
    IdxList idx_list;   // 路径在地图上的索引

private:
    // 检查地图是否存在环路
    bool check_duplicate_idx(const SDL_Point& idx_target);
};