// Created by LuoYu on 2026/6/9.
#include "map.h"

#include <fstream>
#include <sstream>

size_t Map::get_width() const {
    return tile_map.empty() ? 0 : tile_map[0].size();
}

size_t Map::get_height() const {
    return tile_map.size();
}

bool Map::load(const std::string& path) {
    std::fstream file(path);
    if (!file.good()) return false;

    // ------ 成功打开 ------

    TileMap temp_tile_map;
    // 当前读取的索引
    int idx_x, idx_y = -1;
    std::string str_line;
    // 读取一行CSV
    while (std::getline(file, str_line)) {
        // 去除空格及制表符并忽略空行
        str_line = trim_string(str_line);
        if (str_line.empty()) continue;

        // 列索引重置，行索引+1
        idx_x = -1, ++idx_y;
        // 创建一行瓦片地图
        temp_tile_map.emplace_back();

        std::string str_tile;
        std::stringstream str_stream(str_line);
        // 以 ',' 分隔，新增一列
        while (std::getline(str_stream, str_tile, ',')) {
            ++idx_x;
            temp_tile_map[idx_y].emplace_back();
            Tile& tile = temp_tile_map[idx_y].back();
            load_tile_from_string(tile, str_tile);
        }
    }
    file.close();

    if (temp_tile_map.empty() || temp_tile_map[0].empty()) return false;
    tile_map = temp_tile_map;
    return true;
}

std::string Map::trim_string(const std::string& str) {
    const size_t idx_start = str.find_first_not_of(" \t");
    if (idx_start == std::string::npos) return "";
    const size_t idx_end = str.find_last_not_of(" \t");
    return str.substr(idx_start, idx_end - idx_start + 1);
}

void Map::load_tile_from_string(Tile& tile, const std::string& string) {
    const std::string str_tidy = trim_string(string);
    std::string str_value;
    std::vector<int> value_list;
    std::stringstream str_stream(str_tidy);

    // 使用\分割数据
    while (std::getline(str_stream, str_value, '\\')) {
        int value;
        try {
            value = std::stoi(str_value);
        } catch (const std::invalid_argument&) {
            value = -1;
        }
        value_list.emplace_back(value);
    }

    tile.terrain = value_list.size() < 1 || value_list[0] < 0 ? 0 : value_list[0];
    tile.decoration = value_list.size() < 2 ? -1 : value_list[1];
    tile.direction = static_cast<Tile::Direction>(value_list.size() < 3 || value_list[2] < 0 ? 0 : value_list[2]);
    tile.special_flag = value_list.size() <= 3 ? -1 : value_list[3];
}

void Map::generate_map_cache() {
    // 遍历整张地图
    for (int y = 0; y < get_height(); ++y) {
        for (int x = 0; x < get_width(); ++x) {
            const Tile& tile = tile_map[y][x];
            // 无特殊标志
            if (tile.special_flag < 0) continue;
            // 赋值房子的坐标
            if (tile.special_flag == 0) idx_home.x = x, idx_home.y = y;
        }
    }
}