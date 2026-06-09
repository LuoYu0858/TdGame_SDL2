// Created by LuoYu on 2026/6/9.
#pragma once

#include "manager.h"

#include <SDL.h>

// 游戏主体管理器
class GameManager : public Manager<GameManager> {
    friend class Manager<GameManager>;

public:
    int run(int argc, char** argv);

protected:
    GameManager();
    ~GameManager();

private:
    const int FPS = 60;     // 游戏帧率

private:
    SDL_Event event{};      // SDL事件
    bool is_run = true;     // 游戏运行标记

    SDL_Window* window = nullptr;       // SDL窗口
    SDL_Renderer* renderer = nullptr;   // SDL渲染器

private:
    // 自定义断言
    static void init_assert(bool flag, const char* err_msg);

    // 输入处理
    void on_input();

    // 帧更新
    void on_update(double delta);

    // 渲染
    void on_render();
};