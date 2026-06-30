// Created by LuoYu on 2026/6/12.
#pragma once

#include "timer.h"
#include "route.h"
#include "vector2.h"
#include "animation.h"
#include "config_manager.h"

#include <functional>

class Enemy {
public:
    /**
     * 释放技能的回调函数
     * @param enemy 释放技能的敌人
     */
    using SkillCallback = std::function<void(Enemy* enemy)>;

public:
    Enemy();

    ~Enemy() = default;

    void on_update(double delta);

    void on_render(SDL_Renderer* renderer) const;

    void set_on_skill_release(const SkillCallback& on_skill_release);

    void increase_hp(double v);

    void decease_hp(double v);

    void slow_down();

    void set_position(const Vector2& position);

    void set_route(const Route* route);

    void make_invalid();

    [[nodiscard]] double get_hp() const;

    [[nodiscard]] const Vector2& get_size() const;

    [[nodiscard]] const Vector2& get_position() const;

    [[nodiscard]] const Vector2& get_velocity() const;

    [[nodiscard]] double get_damage() const;

    [[nodiscard]] double get_reward_ratio() const;

    [[nodiscard]] double get_recover_radius() const;

    [[nodiscard]] double get_recover_intensity() const;

    [[nodiscard]] bool can_remove() const;

    // 敌人在路径行进进度
    [[nodiscard]] double get_route_process() const;

protected:
    Vector2 size;           // 敌人尺寸

    Timer timer_skill;      // 技能释放计时器

    Animation anim_up;      // 向上移动动画
    Animation anim_down;    // 向下移动动画
    Animation anim_left;    // 向左移动动画
    Animation anim_right;   // 向右移动动画

    Animation anim_up_sketch;      // 向上移动剪影动画
    Animation anim_down_sketch;    // 向下移动剪影动画
    Animation anim_left_sketch;    // 向左移动剪影动画
    Animation anim_right_sketch;   // 向右移动剪影动画

    double hp = 0;                  // 当前生命值
    double max_hp = 0;              // 最大生命值
    double speed = 0;               // 当前移动速度
    double max_speed = 0;           // 正常状态移动速度
    double damage = 0;              // 对房屋造成的伤害
    double reward_ratio = 0;        // 爆金币概率
    double recover_interval = 0;    // 恢复技能时间间隔
    double recover_range = 0;       // 恢复技能生效范围
    double recover_intensity = 0;   // 恢复技能强度

private:
    Vector2 position;   // 当前位置
    Vector2 velocity;   // 移动速度
    Vector2 direction;  // 移动方向

    bool is_valid = true;   // 是否生效

    Timer timer_sketch;             // 剪影特效时间
    bool is_show_sketch = false;    // 是否处于显示剪影状态

    Animation* anim_current = nullptr;  // 当前正在显示的动画

    SkillCallback on_skill_released;    // 释放技能的回调函数
    Timer timer_restore_speed;          // 恢复速度的时间

    const Route* route = nullptr;   // 怪物需要行走的路径
    int idx_target = 0;             // 移动目标单元格索引
    Vector2 position_target;        // 目标单元格位置(以中心点为锚点)

private:
    // 更新目标点位置
    void _refresh_position_target();
};