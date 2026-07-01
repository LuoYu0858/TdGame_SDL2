// Created by LuoYu on 2026/6/30.
#include "enemy_manager.h"

#include "home_manager.h"

#include "slim_enemy.h"
#include "goblin_enemy.h"
#include "skeleton_enemy.h"
#include "king_slim_enemy.h"
#include "goblin_priest_enemy.h"

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

void EnemyManager::spawn_enemy(EnemyType type, int idx_spawn_point) {
    static Vector2 position;
    static const SDL_Rect& rect_tile_map = ConfigManager::instance()->rect_tile_map;
    static const Map::SpawnerRoutePool& spawner_route_pool = ConfigManager::instance()->map.get_idx_spawner_pool();

    const auto& itor = spawner_route_pool.find(idx_spawn_point);

    // 不存在该出生点
    if (itor == spawner_route_pool.end()) return;

    Enemy* enemy = nullptr;

    switch (type) {
        case EnemyType::Slim:
            enemy = new SlimEnemy;
            break;
        case EnemyType::KingSlim:
            enemy = new KingSlimEnemy;
            break;
        case EnemyType::Skeleton:
            enemy = new SkeletonEnemy;
            break;
        case EnemyType::Goblin:
            enemy = new GoblinEnemy;
            break;
        case EnemyType::GoblinPriest:
            enemy = new GoblinPriestEnemy;
            break;
    }

    // 设置敌人技能(恢复)
    enemy->set_on_skill_release([&](const Enemy* enemy_src) {
        // 敌人恢复范围
        double recover_radius = enemy_src->get_recover_radius();
        // 判定是否存在恢复技能, 范围小于0视为无恢复技能
        if (recover_radius < 0) return;
        // 敌人自身坐标
        const Vector2 pos_src = enemy_src->get_position();
        // 查找所有敌人
        for (Enemy* enemy_dst : enemy_list) {
            const Vector2& pos_dst = enemy->get_position();
            if (double distance = (pos_dst - pos_src).length();
                distance <= recover_radius)
                enemy_dst->increase_hp(enemy_src->get_recover_intensity());
        }
    });

    // 设置敌人
    const Route::IdxList& idx_list = itor->second.get_idx_list();
    position.x = rect_tile_map.x + idx_list[0].x * SIZE_TILE + SIZE_TILE / 2.0;
    position.y = rect_tile_map.y + idx_list[0].y * SIZE_TILE + SIZE_TILE / 2.0;

    // 初始位置
    enemy->set_position(position);
    // 寻路路径
    enemy->set_route(&itor->second);

    enemy_list.emplace_back(enemy);
}

bool EnemyManager::check_cleared() const {
    return enemy_list.empty();
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