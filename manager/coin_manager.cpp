// Created by LuoYu on 2026/7/1.
#include "coin_manager.h"

void CoinManager::increase_coin(double val) {
    num_coin += val;
}

void CoinManager::decrease_coin(double val) {
    num_coin -= val;
    if (num_coin < 0) num_coin = 0;
}