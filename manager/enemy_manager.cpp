// Created by LuoYu on 2026/6/30.
#include "enemy_manager.h"

#include "home_manager.h"

void EnemyManager::on_update(double delta) {
    // 更新所有敌人
    for (auto enemy : enemy_list) enemy->on_update(delta);

    // 处理所有敌人的碰撞关系
    _process_home_collision();
    _process_bullet_collision();

    // 移除掉无效敌人
    _remove_invalid_enemy();
}

void EnemyManager::on_render(SDL_Renderer* renderer) const {
    for (auto enemy : enemy_list) enemy->on_render(renderer);
}

EnemyManager::~EnemyManager() {
    for (auto enemy : enemy_list) delete enemy;
}

void EnemyManager::_process_home_collision() const {
    // 获取房屋坐标
    const auto& [home_x, home_y] = ConfigManager::instance()->map.get_idx_home();
    // 获取瓦片地图矩形
    static const SDL_Rect& rect_tile_map = ConfigManager::instance()->rect_tile_map;
    // 计算房屋位置
    static const Vector2 position_home_tile = {
        static_cast<double>(rect_tile_map.x + home_x * SIZE_TILE),
        static_cast<double>(rect_tile_map.y + home_y * SIZE_TILE)
    };

    for (auto enemy : enemy_list) {
        // 跳过无效敌人
        if (enemy->can_remove()) continue;
        // 判断敌人是否碰到房屋(敌人的中心点是否进入房屋内部坐标)
        if (const Vector2& position = enemy->get_position();
            position.x >= position_home_tile.x &&
            position.y >= position_home_tile.y &&
            position.x <= position_home_tile.x + SIZE_TILE &&
            position.y <= position_home_tile.y + SIZE_TILE) {
            // 发生碰撞
            // 1. 使敌人无效
            enemy->make_invalid();
            // 2. 减少房屋生命
            HomeManager::instance()->decrease_hp(enemy->get_damage());
        }
    }
}

void EnemyManager::_process_bullet_collision() {
}

void EnemyManager::_remove_invalid_enemy() {
    std::erase_if(
        enemy_list,
        [](const Enemy* enemy) {
            bool deletable = enemy->can_remove();
            if (deletable) delete enemy;
            return deletable;
        }
    );
}