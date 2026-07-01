// Created by LuoYu on 2026/7/1.
#pragma once

#include "manager.h"

// 金币管理器
class CoinManager : public Manager<CoinManager> {
    friend class Manager;

public:
    void increase_coin(double val);

    void decrease_coin(double val);

protected:
    CoinManager() = default;
    ~CoinManager() = default;

private:
    double num_coin = 0;    // 当前金币数量
};