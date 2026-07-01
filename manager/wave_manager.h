// Created by LuoYu on 2026/7/1.
#pragma once

#include "timer.h"
#include "manager.h"

// 波次管理器
class WaveManager : public Manager<WaveManager> {
    friend class Manager;

public:
    void on_update(double delta);

protected:
    WaveManager();
    ~WaveManager() = default;

private:
    int idx_wave = 0;           // 当前波次索引
    int idx_spawn_event = 0;    // 当前波次的事件索引

    Timer timer_start_wave;     // 波次开始时间间隔
    Timer timer_spawn_enemy;    // 敌人生成时间间隔

    bool is_wave_started = false;       // 波次是否开始
    bool is_spawned_last_enemy = false; // 是否生成了最后一个敌人
};