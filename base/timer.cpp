// Created by LuoYu on 2026/6/12.

#include "timer.h"

void Timer::restart() {
    pass_time = 0;
    shotted = false;
}

void Timer::set_wait_time(const double wait_time) {
    this->wait_time = wait_time;
}

void Timer::set_one_shot(const bool one_shot) {
    this->one_shot = one_shot;
}

void Timer::set_on_timeout(const std::function<void()>& on_timeout) {
    this->on_timeout = on_timeout;
}

void Timer::pause() {
    this->paused = true;
}

void Timer::resume() {
    this->paused = false;
}

void Timer::on_update(const double delta) {
    if (paused) return;

    pass_time += delta;
    if (pass_time >= wait_time) {
        const auto can_shot = !one_shot || !shotted;
        shotted = true;
        if (can_shot && on_timeout) on_timeout();
        pass_time -= wait_time;
    }
}