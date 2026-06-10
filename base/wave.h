// Created by LuoYu on 2026/6/10.
#pragma once

#include "enemy_type.h"

#include <vector>

// 波次
struct Wave {
    // 生成事件
    struct SpawnEvent {
        double interval = 0;                    // 距离上一次事件间隔
        int spawn_point = 1;                    // 生成事件索引点位
        EnemyType enemy_type = EnemyType::Slim; // 生成的敌人类型
    };

    double rewards = 0;     // 波次奖励
    double interval = 0;    // 波次间隔
    std::vector<SpawnEvent> spawn_event_list;   // 事件列表
};