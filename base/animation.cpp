// Created by LuoYu on 2026/6/12.

#include "animation.h"

Animation::Animation() {
    timer.set_one_shot(false);
    timer.set_on_timeout([this] {
        ++idx_frame;
        if (idx_frame >= rect_src_list.size()) {
            idx_frame = is_loop ? 0 : rect_src_list.size() - 1;
            if (!is_loop && on_finished) on_finished();
        }
    });
}

void Animation::reset() {
    timer.restart();
    idx_frame = 0;
}

void Animation::set_frame_data(SDL_Texture* texture, const int num_h, const int num_v, const std::vector<int>& idx_list) {
    int width_tex, height_tex;

    this->texture = texture;
    SDL_QueryTexture(texture, nullptr, nullptr, &width_tex, &height_tex);

    width_frame = width_tex / num_h, height_frame = height_tex / num_v;

    rect_src_list.resize(idx_list.size());

    for (size_t i = 0; i < idx_list.size(); ++i) {
        const int idx = idx_list[i];
        auto& rect_src = rect_src_list[i];

        rect_src = {
            idx % num_h * width_frame,
            idx / num_h * height_frame,
            width_frame,
            height_frame
        };
    }
}

void Animation::set_loop(const bool is_loop) {
    this->is_loop = is_loop;
}

void Animation::set_interval(const double interval) {
    timer.set_wait_time(interval);
}

void Animation::set_on_finished(const PlayCallback& on_finished) {
    this->on_finished = on_finished;
}

void Animation::on_update(const double delta) {
    timer.on_update(delta);
}

void Animation::on_render(SDL_Renderer* renderer, const SDL_Point& pos_dst, const double angle) const {
    static SDL_Rect rect_dst;

    rect_dst = {
        pos_dst.x, pos_dst.y,
        width_frame, height_frame
    };

    SDL_RenderCopyEx(renderer, texture, &rect_src_list[idx_frame], &rect_dst, angle, nullptr, SDL_FLIP_NONE);
}