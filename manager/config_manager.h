// Created by LuoYu on 2026/6/10.
#pragma once

#include "map.h"
#include "wave.h"
#include "manager.h"

#include <SDL.h>
#include <string>

// 游戏配置管理器
class ConfigManager : public Manager<ConfigManager> {
    friend class Manager;

public:
    // 游戏基础信息
    struct BasicTemplate {
        std::string widow_title = "村庄保卫战";  // 窗口标题
        int window_width = 1280;                // 窗口宽度
        int window_height = 720;                // 窗口高度
    };

    // 玩家基础信息
    struct PlayerTemplate {
        double speed = 3;                       // 移动速度
        double normal_attack_interval = 0.5;    // 普攻间隔
        double normal_attack_damage = 0.0;      // 普攻伤害
        double skill_interval = 10;             // 技能冷却
        double skill_damage = 0.0;              // 技能伤害
    };

    // 防御塔基础信息
    struct TowerTemplate {
        double interval[10]{};          // 攻击间隔
        double damage[10]{};            // 攻击伤害
        double view_range[10]{};        // 攻击范围
        double cost[10]{};              // 建造花费
        double upgrade_cost[9]{};       // 升级花费
    };

    // 敌人基础信息
    struct EnemyTemplate {
        double hp = 10;                 // 最大血量
        double speed = 1;               // 移动速度
        double damage = 1;              // 攻击伤害
        double reward_ratio = 0.5;      // 奖励概率
        double recover_interval = 10;   // 恢复间隔
        double recover_ranger = -1;     // 恢复范围
        double recover_intensity = 5;   // 恢复强度
    };

public:
    /**
     * @brief 从关卡配置文件加载关卡
     * @param path 关卡配置文件路径
     */
    bool load_level_config(const std::string& path);

    /**
     * @brief 从游戏配置文件加载配置数据
     * @param path 游戏配置文件路径
     */
    bool load_game_config(const std::string& path);

public:
    const double num_initial_hp = 10;       // 玩家房子初始生命值
    const double num_initial_roin = 100;    // 玩家初始金币数
    const double num_coin_per_prop = 10;    // 每次获得金币后，金币数量增加值

public:
    Map map;                        // 游戏地图
    std::vector<Wave> wave_list;    // 波次列表

    int level_archer = 0;   // 防御塔 -> 弓箭手等级
    int level_axeman = 0;   // 防御塔 -> 斧头兵等级
    int level_gunner = 0;   // 防御塔 -> 炮手等级

    bool is_game_win = true;    // 游戏是否胜利
    bool is_game_over = false;  // 游戏是否结束

    SDL_Rect rect_tile_map{};   // 瓦片地图在窗口相对位置

    BasicTemplate basic_template;           // 基础配置

    PlayerTemplate player_template;         // 玩家配置

    TowerTemplate archer_template;          // 防御塔 -> 弓箭手配置
    TowerTemplate axeman_template;          // 防御塔 -> 斧头兵配置
    TowerTemplate gunner_template;          // 防御塔 -> 炮手配置

    EnemyTemplate slim_template;            // 敌人 -> 史莱姆配置
    EnemyTemplate king_slim_template;       // 敌人 -> 史莱姆配置
    EnemyTemplate skeleton_template;        // 敌人 -> 史莱姆配置
    EnemyTemplate goblin_template;          // 敌人 -> 史莱姆配置
    EnemyTemplate goblin_priest_template;   // 敌人 -> 史莱姆配置

protected:
    ConfigManager() = default;
    ~ConfigManager() = default;
};