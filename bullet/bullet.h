// Created by LuoYu on 2026/7/6.
#pragma once

#include "enemy.h"
#include "vector2.h"
#include "animation.h"

// 子弹基类
class Bullet {
public:
    Bullet() = default;

public:
    void set_velocity(const Vector2& velocity);

    void set_position(const Vector2& position);

    void set_damage(double damage);

    [[nodiscard]] const Vector2& get_size() const;

    [[nodiscard]] const Vector2& get_position() const;

    [[nodiscard]] double get_damage() const;

    [[nodiscard]] double get_damage_range() const;

    void disable_collide();

    [[nodiscard]] bool can_collide() const;

    void make_invalid();

    [[nodiscard]] bool can_remove() const {
        return !is_valid;
    }

    virtual void on_update(double delta);

    virtual void on_render(SDL_Renderer* renderer);

    virtual void on_collide(Enemy* enemy);

protected:
    ~Bullet() = default;

protected:
    Vector2 size;       // 子弹尺寸
    Vector2 velocity;   // 子弹运动双速度
    Vector2 position;   // 子弹当前位置

    Animation animation;    // 子弹动画
    bool can_rotated = false;   // 是否可以旋转

    double damage = 0;          // 子弹伤害
    double damage_range = 0;    // 伤害范围

private:
    bool is_valid = true;       // 子弹是否有效
    bool is_collisional = true; // 是否允许发生碰撞

    double angle_anim_rotated = 0;  // 动画旋转角度
};