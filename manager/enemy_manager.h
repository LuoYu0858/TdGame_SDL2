// Created by LuoYu on 2026/6/30.
#pragma once

#include "enemy.h"
#include "manager.h"
#include "config_manager.h"

#include <SDL.h>
#include <vector>

// 敌人管理器
class EnemyManager : public Manager<EnemyManager> {
    friend class Manager;

public:
    using EnemyList = std::vector<Enemy*>;  // 敌人列表类型

public:
    void on_update(double delta);

    void on_render(SDL_Renderer* renderer) const;

protected:
    EnemyManager() = default;
    ~EnemyManager();

private:
    EnemyList enemy_list;   // 敌人列表

private:
    // 敌人和房屋碰撞处理
    void _process_home_collision() const;

    // TODO 敌人和防御塔攻击碰撞处理
    void _process_bullet_collision();

    // 移除无效状态的敌人
    void _remove_invalid_enemy();
};