// Created by LuoYu on 2026/6/10.
#include "route.h"

Route::Route(const TileMap& map, const SDL_Point& idx_origin) {
    // 扫描的下一个网格索引
    SDL_Point idx_next = idx_origin;

    // 遍历整张地图
    for (const size_t map_width = map[0].size(), map_height = map.size();;) {
        // 超出边界退出路径生成
        if (idx_next.x >= map_width || idx_next.y >= map_height) break;
        // 地图中存在环路同样需要退出
        if (check_duplicate_idx(idx_next)) break;

        // 将该网格添加到路径点
        idx_list.push_back(idx_next);

        // 基于方向标将idx_next沿对应方向前进
        bool is_next_dir_exist = true;  // 是否存在下一个方向标
        const Tile& tile = map[idx_next.y][idx_next.x];

        // 遇到房屋 -> 路径生成结束
        if (tile.special_flag == 0) break;

        switch (tile.direction) {
            case Tile::Direction::Up:
                --idx_next.y;
                break;
            case Tile::Direction::Down:
                ++idx_next.y;
                break;
            case Tile::Direction::Left:
                --idx_next.x;
                break;
            case Tile::Direction::Right:
                ++idx_next.x;
                break;
            case Tile::Direction::None:
                is_next_dir_exist = false;
                break;
        }
        // 不存在后续路标 -> 地图存在断头路，退出生成
        if (!is_next_dir_exist) break;
    }
}

const Route::IdxList& Route::get_idx_list() const {
    return this->idx_list;
}

bool Route::check_duplicate_idx(const SDL_Point& idx_target) {
    for (const auto& [x, y] : idx_list) {
        // 目标索引和已扫描索引相同，存在环路
        if (x == idx_target.x && y == idx_target.y) return true;
    }
    return false;
}