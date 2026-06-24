// Created by LuoYu on 2026/6/24.
#pragma once

#include "enemy.h"
#include "config_manager.h"
#include "resources_manager.h"

// 哥布林 - 敌人
class GoblinEnemy : public Enemy {
public:
    GoblinEnemy() {
        static const ResourcesManager::TexturePool& texture_pool = ResourcesManager::instance()->get_texture_pool();

        // 获取纹理
        static SDL_Texture* tex_goblin = texture_pool.find(ResID::Tex_Goblin)->second;
        static SDL_Texture* tex_goblin_sketch = texture_pool.find(ResID::Tex_GoblinSketch)->second;

        // 配置数据
        static auto& [hp, speed, damage, reward_ratio,
            recover_interval, recover_ranger, recover_intensity
        ] = ConfigManager::instance()->goblin_template;

        // 向上动画索引
        static const std::vector idx_list_down = {0, 1, 2, 3, 4};
        static const std::vector idx_list_up = {5, 6, 7, 8, 9};
        static const std::vector idx_list_right = {10, 11, 12, 13, 14};
        static const std::vector idx_list_left = {15, 16, 17, 18, 19};

        // 动画设置
        anim_up.set_loop();
        anim_up.set_interval(0.15);
        anim_up.set_frame_data(tex_goblin, 5, 4, idx_list_up);

        anim_down.set_loop();
        anim_down.set_interval(0.15);
        anim_down.set_frame_data(tex_goblin, 5, 4, idx_list_down);

        anim_left.set_loop();
        anim_left.set_interval(0.15);
        anim_left.set_frame_data(tex_goblin, 5, 4, idx_list_left);

        anim_right.set_loop();
        anim_right.set_interval(0.15);
        anim_right.set_frame_data(tex_goblin, 5, 4, idx_list_right);

        // 剪影动画设置
        anim_up_sketch.set_loop();
        anim_up_sketch.set_interval(0.15);
        anim_up_sketch.set_frame_data(tex_goblin_sketch, 5, 4, idx_list_up);

        anim_down_sketch.set_loop();
        anim_down_sketch.set_interval(0.15);
        anim_down_sketch.set_frame_data(tex_goblin_sketch, 5, 4, idx_list_down);

        anim_left_sketch.set_loop();
        anim_left_sketch.set_interval(0.15);
        anim_left_sketch.set_frame_data(tex_goblin_sketch, 5, 4, idx_list_left);

        anim_right_sketch.set_loop();
        anim_right_sketch.set_interval(0.15);
        anim_right_sketch.set_frame_data(tex_goblin_sketch, 5, 4, idx_list_right);

        // 模板属性
        this->max_hp = hp;
        this->max_speed = speed;
        this->damage = damage;
        this->reward_ratio = reward_ratio;
        this->recover_interval = recover_interval;
        this->recover_range = recover_ranger;
        this->recover_intensity = recover_intensity;

        this->size.x = 48, this->size.y = 48;
        this->hp = max_hp;
        this->speed = max_speed;
    }

    ~GoblinEnemy() = default;
};