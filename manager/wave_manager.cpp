// Created by LuoYu on 2026/7/1.
#include "wave_manager.h"

#include "coin_manager.h"
#include "enemy_manager.h"
#include "config_manager.h"

void WaveManager::on_update(double delta) {
    static ConfigManager* instance = ConfigManager::instance();

    if (instance->is_game_over) return;

    if (!is_wave_started) timer_start_wave.on_update(delta);
    else timer_spawn_enemy.on_update(delta);

    // 当前波次的敌人全部清空
    if (is_spawned_last_enemy && EnemyManager::instance()->check_cleared()) {
        // 金币增加
        CoinManager::instance()->increase_coin(instance->wave_list[idx_wave].rewards);
        ++idx_wave;
        if (idx_wave >= instance->wave_list.size()) {
            // 玩家击败最后一波敌人
            instance->is_game_win = true;
            instance->is_game_over = true;
        } else {
            // 还有后续波次
            // 事件索引以及波次状态重置
            idx_spawn_event = 0;
            is_wave_started = false;
            is_spawned_last_enemy = false;

            const Wave& wave = instance->wave_list[idx_wave];
            timer_start_wave.set_wait_time(wave.interval);
            timer_start_wave.restart();
        }
    }
}

WaveManager::WaveManager() {
    static const std::vector<Wave>& wave_list = ConfigManager::instance()->wave_list;

    // 首次进入波次执行的定时器
    timer_start_wave.set_one_shot();
    timer_start_wave.set_wait_time(wave_list[0].interval);
    timer_start_wave.set_on_timeout([&] {
        is_wave_started = true;
        // 等待间隔为每一波敌人的第一个生成时间的时间间隔
        timer_spawn_enemy.set_wait_time(wave_list[idx_wave].spawn_event_list[0].interval);
        timer_spawn_enemy.restart();
    });

    timer_spawn_enemy.set_one_shot();
    timer_spawn_enemy.set_on_timeout([&] {
        // 事件列表
        const std::vector<Wave::SpawnEvent>& spawn_event_list = wave_list[idx_wave].spawn_event_list;
        // 具体事件
        const Wave::SpawnEvent& spawn_event = spawn_event_list[idx_spawn_event];

        EnemyManager::instance()->spawn_enemy(spawn_event.enemy_type, spawn_event.spawn_point);

        ++idx_spawn_event;

        if (idx_spawn_event >= spawn_event_list.size()) {
            is_spawned_last_enemy = true;
            return;
        }

        // 再次启动循环生成敌人
        timer_spawn_enemy.set_wait_time(spawn_event_list[idx_spawn_event].interval);
        timer_spawn_enemy.restart();
    });
}