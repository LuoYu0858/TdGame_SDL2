// Created by LuoYu on 2026/6/9.
#pragma once

#include "manager.h"

#include <SDL.h>

// 游戏主体管理器
class GameManager : public Manager<GameManager> {
    friend class Manager;

public:
    int run(int argc, char** argv);

protected:
    GameManager();
    ~GameManager();

private:
    const int FPS = 60;                 // 游戏帧率
    const int AUDIO_FREQ = 44100;       // 音频采样率
    const int AUDIO_TRACT = 2;          // 音频声道数
    const int AUDIO_BUFF_SIZE = 2048;   // SDL音频缓冲区大小

private:
    SDL_Event event{};      // SDL事件
    bool is_run = true;     // 游戏运行标记

    SDL_Window* window = nullptr;       // SDL窗口
    SDL_Renderer* renderer = nullptr;   // SDL渲染器

    SDL_Texture* tex_tile_map = nullptr;    // 瓦片地图纹理

private:
    // 自定义断言
    static void _init_assert(bool flag, const char* err_msg);

    // 输入处理
    void on_input();

    // 帧更新
    void on_update(double delta);

    // 渲染
    void on_render();

    // 预生成瓦片地图纹理
    bool _generate_tile_map_texture();
};