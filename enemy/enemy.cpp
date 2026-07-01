// Created by LuoYu on 2026/6/15.
#include "enemy.h"

Enemy::Enemy() {
    // 技能计时器
    timer_skill.set_one_shot(false);
    timer_skill.set_on_timeout([this] {
        on_skill_released(this);
    });

    // 剪影特效计时器
    timer_sketch.set_one_shot();
    timer_sketch.set_wait_time(0.05);
    timer_sketch.set_on_timeout([this] {
        is_show_sketch = false;
    });

    // 速度恢复计时器
    timer_restore_speed.set_one_shot();
    timer_restore_speed.set_on_timeout([this] {
        speed = max_speed;
    });
}

void Enemy::on_update(const double delta) {
    // ------ 计时器更新 ------

    timer_skill.on_update(delta);
    timer_sketch.on_update(delta);
    timer_restore_speed.on_update(delta);

    // ------ 计算移动方向 ------

    // 计算这一帧最远可以移动的距离
    const Vector2 move_distance = velocity * delta;
    // 前一帧距离目标位置距离
    const Vector2 target_distance = position_target - position;

    // 需要移动的距离 -> 不能超过最远可移动距离
    position += move_distance < target_distance ? move_distance : target_distance;

    // 若上一帧已到达目标点，则更新目标点
    if (target_distance.approx_zero()) {
        ++idx_target;
        _refresh_position_target();

        // 更新移动方向
        direction = (position_target - position).normalize();
    }

    velocity = direction * speed * SIZE_TILE;

    // ------ 选择对应动画 ------

    // 是否显示水平方向动画
    if (const bool is_show_x_anim = abs(velocity.x) >= abs(velocity.y);
        is_show_sketch) {
        if (is_show_x_anim) anim_current = velocity.x > 0 ? &anim_right_sketch : &anim_left_sketch;
        else anim_current = velocity.y > 0 ? &anim_down_sketch : &anim_up_sketch;
        } else {
            if (is_show_x_anim) anim_current = velocity.x > 0 ? &anim_right : &anim_left;
            else anim_current = velocity.y > 0 ? &anim_down : &anim_up;
        }

    // ------ 更新动画 ------

    anim_current->on_update(delta);
}

void Enemy::on_render(SDL_Renderer* renderer) const {
    static SDL_Rect rect;
    static SDL_Point point;
    // 血条尺寸
    static const Vector2 size_hp_bar = {40, 8};
    // 血条纵向偏移值
    static constexpr int offset_y = 2;

    static constexpr SDL_Color color_border = {116, 185, 124, 255};
    static constexpr SDL_Color color_content = {226, 255, 194, 255};

    point.x = static_cast<int>(position.x - size.x / 2);
    point.y = static_cast<int>(position.y - size.y / 2);

    anim_current->on_render(renderer, point);

    // 绘制血条
    if (hp < max_hp) {
        rect.x = static_cast<int>(position.x - size_hp_bar.x / 2);
        rect.y = static_cast<int>(position.y - size.y / 2 - size_hp_bar.y - offset_y);
        rect.w = static_cast<int>(size_hp_bar.x * (hp / max_hp));
        rect.h = static_cast<int>(size_hp_bar.y);

        SDL_SetRenderDrawColor(renderer, color_content.r, color_content.g, color_content.b, color_content.a);
        SDL_RenderFillRect(renderer, &rect);

        rect.w = static_cast<int>(size_hp_bar.x);

        SDL_SetRenderDrawColor(renderer, color_border.r, color_border.g, color_border.b, color_border.a);
        SDL_RenderDrawRect(renderer, &rect);
    }
}

void Enemy::set_on_skill_release(const SkillCallback& on_skill_release) {
    this->on_skill_released = on_skill_release;
}

void Enemy::increase_hp(const double v) {
    hp += v;
    hp = hp > max_hp ? max_hp : hp;
}

void Enemy::decease_hp(const double v) {
    hp -= v;
    if (hp <= 0) {
        hp = 0;
        is_valid = false;
    }
    is_show_sketch = true;
    timer_sketch.restart();
}

void Enemy::slow_down() {
    speed = max_speed - 0.5;
    timer_restore_speed.set_wait_time(1);
    timer_restore_speed.restart();
}

void Enemy::set_position(const Vector2& position) {
    this->position = position;
}

void Enemy::set_route(const Route* route) {
    this->route = route;
    _refresh_position_target();
}

void Enemy::make_invalid() {
    is_valid = false;
}

double Enemy::get_hp() const {
    return this->hp;
}

const Vector2& Enemy::get_size() const {
    return this->size;
}

const Vector2& Enemy::get_position() const {
    return this->position;
}

const Vector2& Enemy::get_velocity() const {
    return this->velocity;
}

double Enemy::get_damage() const {
    return this->damage;
}

double Enemy::get_reward_ratio() const {
    return this->reward_ratio;
}

double Enemy::get_recover_radius() const {
    return SIZE_TILE * this->recover_range;
}

double Enemy::get_recover_intensity() const {
    return this->recover_intensity;
}

bool Enemy::can_remove() const {
    return !is_valid;
}

double Enemy::get_route_process() const {
    if (route->get_idx_list().size() == 1) return 1;
    return static_cast<double>(idx_target) / static_cast<double>(route->get_idx_list().size() - 1);
}

void Enemy::_refresh_position_target() {
    if (const Route::IdxList& idx_list = route->get_idx_list();
    idx_target < idx_list.size()) { // 还未走到最后一个目标点
        // 获取对应索引
        const auto& [x, y] = idx_list[idx_target];

        // 转化为窗口坐标
        static const SDL_Rect& rect_tile_map = ConfigManager::instance()->rect_tile_map;
        position_target = {
            rect_tile_map.x + x * SIZE_TILE + SIZE_TILE / 2.0,   // 取地图左边缘，水平增加地图网格点水平坐标，乘以瓦片尺寸，再取中心点
            rect_tile_map.y + y * SIZE_TILE + SIZE_TILE / 2.0    // 取地图上边缘，垂直增加地图网格点垂直坐标，乘以瓦片尺寸，再取中心点
        };
    }
}