// Created by LuoYu on 2026/6/24.
#pragma once

#include "enemy.h"
#include "config_manager.h"
#include "resources_manager.h"

// 皇家史莱姆 - 敌人
class KingSlimEnemy : public Enemy {
public:
    KingSlimEnemy() {
        static const ResourcesManager::TexturePool& texture_pool = ResourcesManager::instance()->get_texture_pool();

        // 获取纹理
        static SDL_Texture* tex_king_slim = texture_pool.find(ResID::Tex_KingSlim)->second;
        static SDL_Texture* tex_king_slim_sketch = texture_pool.find(ResID::Tex_KingSlimSketch)->second;

        // 配置数据
        static auto& [
            hp, speed,damage, reward_ratio,
            recover_interval, recover_ranger, recover_intensity
        ] = ConfigManager::instance()->king_slim_template;

        // 向上动画索引
        static const std::vector idx_list_down = {0, 1, 2, 3, 4, 5};
        static const std::vector idx_list_left = {6, 7, 8, 9, 10, 11};
        static const std::vector idx_list_right = {12, 13, 14, 15, 16, 17};
        static const std::vector idx_list_up = {18, 19, 20, 21, 22, 23};

        // 动画设置
        anim_up.set_loop();
        anim_up.set_interval(0.1);
        anim_up.set_frame_data(tex_king_slim, 6, 4, idx_list_up);

        anim_down.set_loop();
        anim_down.set_interval(0.1);
        anim_down.set_frame_data(tex_king_slim, 6, 4, idx_list_down);

        anim_left.set_loop();
        anim_left.set_interval(0.1);
        anim_left.set_frame_data(tex_king_slim, 6, 4, idx_list_left);

        anim_right.set_loop();
        anim_right.set_interval(0.1);
        anim_right.set_frame_data(tex_king_slim, 6, 4, idx_list_right);

        // 剪影动画设置
        anim_up_sketch.set_loop();
        anim_up_sketch.set_interval(0.1);
        anim_up_sketch.set_frame_data(tex_king_slim_sketch, 6, 4, idx_list_up);

        anim_down_sketch.set_loop();
        anim_down_sketch.set_interval(0.1);
        anim_down_sketch.set_frame_data(tex_king_slim_sketch, 6, 4, idx_list_down);

        anim_left_sketch.set_loop();
        anim_left_sketch.set_interval(0.1);
        anim_left_sketch.set_frame_data(tex_king_slim_sketch, 6, 4, idx_list_left);

        anim_right_sketch.set_loop();
        anim_right_sketch.set_interval(0.1);
        anim_right_sketch.set_frame_data(tex_king_slim_sketch, 6, 4, idx_list_right);

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

    ~KingSlimEnemy() = default;
};