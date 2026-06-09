// Created by LuoYu on 2026/6/9.
#pragma once

#include "tile.h"

#include <SDL.h>
#include <string>

class Map {
public:
    Map() = default;
    ~Map() = default;

    [[nodiscard]] size_t get_width() const;

    [[nodiscard]] size_t get_height() const;

    /**
     * @brief 加载地图文件数据
     * @param path .csv格式的地图文件路径
     * @return true -> 加载成功 | false -> 加载失败
     */
    [[nodiscard]] bool load(const std::string& path);

private:
    TileMap tile_map;           // 瓦片地图
    SDL_Point idx_home = {};    // 房屋索引缓存

private:
    // 去除字符串收尾空格
    [[nodiscard]] static std::string trim_string(const std::string& str);

    // 从字符串读取瓦片信息，并解析到tile
    static void load_tile_from_string(Tile& tile, const std::string& string);

    // 生成地图缓存
    void generate_map_cache();
};