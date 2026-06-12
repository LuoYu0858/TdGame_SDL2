// Created by LuoYu on 2026/6/12.
#pragma once

#include "timer.h"

#include <SDL.h>
#include <vector>
#include <functional>

// 动画类
class Animation {
public:
    using PlayCallback = std::function<void()>;

public:
    Animation();
    ~Animation() = default;

    void reset();

    /**
     * @brief 为动画添加一帧
     * @param texture 使用的纹理
     * @param num_h 该纹理水平格子数量
     * @param num_v 该纹理竖直格子数量
     * @param idx_list 使用的索引的列表
     */
    void set_frame_data(SDL_Texture* texture, int num_h, int num_v, const std::vector<int>& idx_list);

    void set_loop(bool is_loop = true);

    void set_interval(double interval);

    void set_on_finished(const PlayCallback& on_finished);

    void on_update(double delta);

    /**
     * @brief 动画渲染
     * @param renderer 渲染器
     * @param pos_dst 目标位置
     * @param angle 旋转角度
     */
    void on_render(SDL_Renderer* renderer, const SDL_Point& pos_dst, double angle = 0) const;

private:
    Timer timer;                            // 计时器
    bool is_loop = true;                    // 是否循环
    size_t idx_frame = 0;                   // 当前帧索引
    PlayCallback on_finished;               // 回调函数
    SDL_Texture* texture = nullptr;         // 动画纹理
    std::vector<SDL_Rect> rect_src_list;    // 源矩形列
    int width_frame = 0;                    // 单帧宽度
    int height_frame = 0;                   // 单帧高度
};