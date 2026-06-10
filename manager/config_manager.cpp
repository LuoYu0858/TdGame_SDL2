// Created by LuoYu on 2026/6/10.

#include "config_manager.h"

#include <cJSON.h>
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
}