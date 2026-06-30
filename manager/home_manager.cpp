// Created by LuoYu on 2026/6/30.
#include "home_manager.h"

#include "config_manager.h"
#include "resources_manager.h"

double HomeManager::get_current_hp_num() const {
    return num_hp;
}

void HomeManager::decrease_hp(double val) {
    num_hp -= val;

    if (num_hp < 0) num_hp = 0;

    // 播放受伤音效
    static const ResourcesManager::SoundPool& sound_pool = ResourcesManager::instance()->get_sound_pool();
    // 混音
    Mix_PlayChannel(-1, sound_pool.find(ResID::Sound_HomeHurt)->second, 0);
}

HomeManager::HomeManager() {
    num_hp = ConfigManager::instance()->num_initial_hp;
}