// Created by LuoYu on 2026/7/6.
#include "bullet.h"

#include "config_manager.h"

void Bullet::set_velocity(const Vector2& velocity) {
    this->velocity = velocity;
    // 当子弹可以旋转时, 根据速度计算子弹旋转角度
    if (can_rotated) {
        double radian = std::atan2(velocity.y, velocity.x);
        angle_anim_rotated = radian * 180 / 3.14159265;
    }
}

void Bullet::set_position(const Vector2& position) {
    this->position = position;
}

void Bullet::set_damage(double damage) {
    this->damage = damage;
}

const Vector2& Bullet::get_size() const {
    return size;
}

const Vector2& Bullet::get_position() const {
    return position;
}

double Bullet::get_damage() const {
    return damage;
}

double Bullet::get_damage_range() const {
    return damage_range;
}

void Bullet::disable_collide() {
    is_collisional = false;
}

bool Bullet::can_collide() const {
    return is_collisional;
}

void Bullet::make_invalid() {
    is_valid = false;
    is_collisional = false;
}

void Bullet::on_update(double delta) {
    animation.on_update(delta);
    position += velocity * delta;

    // 当子弹的一半飞出屏幕时删除
    if (const auto& [x, y, w, h] = ConfigManager::instance()->rect_tile_map;
        position.x - size.x / 2 <= x || position.x + size.x / 2 >= x + w ||
        position.y - size.y / 2 <= y || position.y + size.y / 2 >= y + h)
        is_valid = false;
}

void Bullet::on_render(SDL_Renderer* renderer) {
    static SDL_Point point;
    point.x = static_cast<int>(position.x - size.x / 2);
    point.y = static_cast<int>(position.y - size.y / 2);

    animation.on_render(renderer, point, angle_anim_rotated);
}

void Bullet::on_collide(Enemy* enemy) {
    is_valid = false;
    is_collisional = false;
}