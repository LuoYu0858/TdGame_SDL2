// Created by LuoYu on 2026/6/10.

#include "config_manager.h"

#include <fstream>
#include <sstream>
#include <iostream>

bool ConfigManager::load_level_config(const std::string& path) {
    // 打开文件
    std::ifstream file(path);
    if (!file.good()) return false;

    // 读取文件
    std::stringstream str_stream;
    str_stream << file.rdbuf();

    // 关闭文件
    file.close();

    cJSON* json_root = cJSON_Parse(str_stream.str().c_str());

    // 解析失败
    if (!json_root) return false;

    // 检查JSON格式
    if (json_root->type != cJSON_Array) {
        cJSON_Delete(json_root);
        return false;
    }

    // ------ 解析成功 ------

    cJSON* json_wave = nullptr;
    // 遍历JSON中的每一个元素
    cJSON_ArrayForEach(json_wave, json_root) {
        // 判断波次数据是否是object
        if (json_wave->type != cJSON_Object) continue;
        // 创造空元素并获取引用
        wave_list.emplace_back();
        auto& [rewards, interval, spawn_event_list] = wave_list.back();

        // 获取rewards字段
        if (const cJSON* json_wave_rewards = cJSON_GetObjectItem(json_wave, "rewards");
            json_wave_rewards && json_wave_rewards->type == cJSON_Number)
            rewards = json_wave_rewards->valuedouble;

        // 获取interval字段
        if (const cJSON* json_wave_interval = cJSON_GetObjectItem(json_wave, "interval");
            json_wave_interval && json_wave_interval->type == cJSON_Number)
            interval = json_wave_interval->valuedouble;

        // 获取spawn_list数组字段
        if (const cJSON* json_wave_spawn_list = cJSON_GetObjectItem(json_wave, "spawn_list");
            json_wave_spawn_list && json_wave_spawn_list->type == cJSON_Array) {
            cJSON* json_spawn_event = nullptr;
            // 遍历波次数组中的所有事件
            cJSON_ArrayForEach(json_spawn_event, json_wave_spawn_list) {
                if (json_spawn_event->type != cJSON_Object) continue;

                spawn_event_list.emplace_back();
                auto& [event_interval, spawn_point, enemy_type] = spawn_event_list.back();

                // ------ 从事件数组中获取字段 ------

                // 获取事件的interval字段
                if (const cJSON* json_spawn_event_interval = cJSON_GetObjectItem(json_spawn_event, "interval");
                    json_spawn_event_interval && json_spawn_event_interval->type == cJSON_Number)
                    event_interval = json_spawn_event_interval->valuedouble;

                // 获取事件的point字段
                if (const cJSON* json_spawn_event_spawn_point = cJSON_GetObjectItem(json_spawn_event, "point");
                    json_spawn_event_spawn_point && json_spawn_event_spawn_point->type == cJSON_Number)
                    spawn_point = json_spawn_event_spawn_point->valueint;

                // 获取事件的enemy字段
                if (const cJSON* json_spawn_event_enemy_type = cJSON_GetObjectItem(json_spawn_event, "enemy");
                    json_spawn_event_enemy_type && json_spawn_event_enemy_type->type == cJSON_String) {
                    // 将enemy字段的字符串转化为对应的枚举类，若字符串非法则使用默认值
                    if (const std::string string_enemy_type = json_spawn_event_enemy_type->valuestring;
                        string_enemy_type == "Slim")
                        enemy_type = EnemyType::Slim;
                    else if (string_enemy_type == "KingSlim") enemy_type = EnemyType::KingSlim;
                    else if (string_enemy_type == "Skeleton") enemy_type = EnemyType::Skeleton;
                    else if (string_enemy_type == "Goblin") enemy_type = EnemyType::Goblin;
                    else if (string_enemy_type == "GoblinPriest") enemy_type = EnemyType::GoblinPriest;
                }
            }
            // 若所有的事件数组均未解析成功
            if (spawn_event_list.empty()) wave_list.pop_back();
        }
    }
    cJSON_Delete(json_root);

    // 所有波次均无效 -> 配置文件出错 返回false | 返回true
    return !wave_list.empty();
}

bool ConfigManager::load_game_config(const std::string& path) {
    // 打开文件 读取 并关闭
    std::ifstream file(path);
    if (!file.good()) return false;

    std::stringstream str_stream;
    str_stream << file.rdbuf();

    file.close();

    cJSON* json_root = cJSON_Parse(str_stream.str().c_str());
    if (!json_root || json_root->type != cJSON_Object) return false;

    // 获取basic字段 -> 游戏基础设置
    const cJSON* json_basic = cJSON_GetObjectItem(json_root, "basic");

    // 获取player字段 -> 玩家设置
    const cJSON* json_player = cJSON_GetObjectItem(json_root, "player");

    // 获取tower字段 -> 防御塔配置
    const cJSON* json_tower = cJSON_GetObjectItem(json_root, "tower");

    // 获取enemy字段 -> 怪物设置
    const cJSON* json_enemy = cJSON_GetObjectItem(json_root, "enemy");

    if (!json_basic || !json_player || !json_tower || !json_enemy
        || json_basic->type != cJSON_Object
        || json_player->type != cJSON_Object
        || json_tower->type != cJSON_Object
        || json_enemy->type != cJSON_Object) {
        cJSON_Delete(json_root);
        return false;
    }

    // 解析具体字段
    _parse_basic_template(basic_template, json_basic);

    _parse_player_template(player_template, json_player);

    _parse_tower_template(archer_template, cJSON_GetObjectItem(json_tower, "archer"));
    _parse_tower_template(axeman_template, cJSON_GetObjectItem(json_tower, "axeman"));
    _parse_tower_template(gunner_template, cJSON_GetObjectItem(json_tower, "gunner"));

    _parse_enemy_template(slim_template, cJSON_GetObjectItem(json_enemy, "slim"));
    _parse_enemy_template(king_slim_template, cJSON_GetObjectItem(json_enemy, "king_slim"));
    _parse_enemy_template(skeleton_template, cJSON_GetObjectItem(json_enemy, "skeleton"));
    _parse_enemy_template(goblin_template, cJSON_GetObjectItem(json_enemy, "goblin"));
    _parse_enemy_template(goblin_priest_template, cJSON_GetObjectItem(json_enemy, "goblin_priest"));

    cJSON_Delete(json_root);
    return true;
}

void ConfigManager::_parse_basic_template(BasicTemplate& tpl, const cJSON* json_root) {
    if (!json_root || json_root->type != cJSON_Object) return;

    if (const cJSON* json_window_title = cJSON_GetObjectItem(json_root, "window_title");
        json_window_title && json_window_title->type == cJSON_String)
        tpl.widow_title = json_window_title->valuestring;

    if (const cJSON* json_window_width = cJSON_GetObjectItem(json_root, "window_width");
        json_window_width && json_window_width->type == cJSON_Number)
        tpl.window_width = json_window_width->valueint;

    if (const cJSON* json_window_height = cJSON_GetObjectItem(json_root, "window_height");
        json_window_height && json_window_height->type == cJSON_Number)
        tpl.window_height = json_window_height->valueint;
}

void ConfigManager::_parse_player_template(PlayerTemplate& tpl, const cJSON* json_root) {
    if (!json_root || json_root->type != cJSON_Object) return;

    if (const cJSON* json_speed = cJSON_GetObjectItem(json_root, "speed");
        json_speed && json_speed->type == cJSON_Number)
        tpl.speed = json_speed->valuedouble;

    if (const cJSON* json_normal_attack_interval = cJSON_GetObjectItem(json_root, "normal_attack_interval");
        json_normal_attack_interval && json_normal_attack_interval->type == cJSON_Number)
        tpl.normal_attack_interval = json_normal_attack_interval->valuedouble;

    if (const cJSON* json_normal_attack_damage = cJSON_GetObjectItem(json_root, "normal_attack_damage");
        json_normal_attack_damage && json_normal_attack_damage->type == cJSON_Number)
        tpl.normal_attack_damage = json_normal_attack_damage->valuedouble;

    if (const cJSON* json_skill_interval = cJSON_GetObjectItem(json_root, "skill_interval");
        json_skill_interval && json_skill_interval->type == cJSON_Number)
        tpl.skill_interval = json_skill_interval->valuedouble;

    if (const cJSON* json_skill_damage = cJSON_GetObjectItem(json_root, "skill_damage");
        json_skill_damage && json_skill_damage->type == cJSON_Number)
        tpl.skill_damage = json_skill_damage->valuedouble;
}

void ConfigManager::_parse_tower_template(TowerTemplate& tpl, const cJSON* json_root) {
    if (!json_root || json_root->type != cJSON_Object) return;

    const cJSON* json_interval = cJSON_GetObjectItem(json_root, "interval");
    const cJSON* json_damage = cJSON_GetObjectItem(json_root, "damage");
    const cJSON* json_view_range = cJSON_GetObjectItem(json_root, "view_range");
    const cJSON* json_cost = cJSON_GetObjectItem(json_root, "cost");
    const cJSON* json_upgrade_cost = cJSON_GetObjectItem(json_root, "upgrade_cost");

    _parse_number_array(tpl.interval, 10, json_interval);
    _parse_number_array(tpl.damage, 10, json_damage);
    _parse_number_array(tpl.view_range, 10, json_view_range);
    _parse_number_array(tpl.cost, 10, json_cost);
    _parse_number_array(tpl.upgrade_cost, 9, json_upgrade_cost);
}

void ConfigManager::_parse_enemy_template(EnemyTemplate& tpl, const cJSON* json_root) {
    if (!json_root || json_root->type != cJSON_Object) return;

    if (const cJSON* json_hp = cJSON_GetObjectItem(json_root, "hp");
        json_hp && json_hp->type == cJSON_Number)
        tpl.hp = json_hp->valuedouble;

    if (const cJSON* json_speed = cJSON_GetObjectItem(json_root, "speed");
        json_speed && json_speed->type == cJSON_Number)
        tpl.speed = json_speed->valuedouble;

    if (const cJSON* json_damage = cJSON_GetObjectItem(json_root, "damage");
        json_damage && json_damage->type == cJSON_Number)
        tpl.damage = json_damage->valuedouble;

    if (const cJSON* json_reward_ratio = cJSON_GetObjectItem(json_root, "reward_ratio");
        json_reward_ratio && json_reward_ratio->type == cJSON_Number)
        tpl.reward_ratio = json_reward_ratio->valuedouble;

    if (const cJSON* json_recover_interval = cJSON_GetObjectItem(json_root, "recover_interval");
        json_recover_interval && json_recover_interval->type == cJSON_Number)
        tpl.recover_interval = json_recover_interval->valuedouble;

    if (const cJSON* json_recover_range = cJSON_GetObjectItem(json_root, "recover_range");
        json_recover_range && json_recover_range->type == cJSON_Number)
        tpl.recover_ranger = json_recover_range->valuedouble;

    if (const cJSON* json_recover_intensity = cJSON_GetObjectItem(json_root, "recover_intensity");
        json_recover_intensity && json_recover_intensity->type == cJSON_Number)
        tpl.recover_intensity = json_recover_intensity->valuedouble;
}

void ConfigManager::_parse_number_array(double* arr, const int max_len, const cJSON* json_root) {
    if (!json_root || json_root->type != cJSON_Array) return;

    int idx = -1;
    cJSON* json_elem = nullptr;
    cJSON_ArrayForEach(json_elem, json_root) {
        ++idx;
        if (json_elem->type != cJSON_Number || idx >= max_len) continue;
        arr[idx] = json_elem->valuedouble;
    }
}