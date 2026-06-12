// Created by LuoYu on 2026/6/12.
#pragma once

#include <functional>

// 计时器
class Timer {
public:
    Timer() = default;
    ~Timer() = default;

    void restart();

    void set_wait_time(double wait_time);

    void set_one_shot(bool one_shot = true);

    void set_on_timeout(const std::function<void()>& on_timeout);

    void pause();

    void resume();

    void on_update(double delta);

private:
    double pass_time = 0;   // 已过时间
    double wait_time = 0;   // 等待时间
    bool paused = false;    // 是否暂停
    bool shotted = false;   // 是否触发
    bool one_shot = false;  // 番茨触发
    std::function<void()> on_timeout;   // 回调函数
};