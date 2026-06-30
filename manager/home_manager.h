// Created by LuoYu on 2026/6/30.
#pragma once

#include "manager.h"

class HomeManager : public Manager<HomeManager> {
    friend class Manager;

public:
    [[nodiscard]] double get_current_hp_num() const;

    void decrease_hp(double val);

protected:
    HomeManager();

    ~HomeManager() = default;

private:
    double num_hp = 0;  // 房屋生命值
};