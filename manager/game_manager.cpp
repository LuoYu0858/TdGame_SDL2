// Created by LuoYu on 2026/6/9.
#include "game_manager.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

int GameManager::run(const int argc, char** argv) {
    Uint64 last_counter = SDL_GetPerformanceCounter();
    const Uint64 counter_freq = SDL_GetPerformanceFrequency();

    while (is_run) {
        // 获取SDL事件
        while (SDL_PollEvent(&event)) on_input();

        // 动态延时
        const Uint64 current_counter = SDL_GetPerformanceCounter();
        const double delta = static_cast<double>(current_counter - last_counter) / static_cast<double>(counter_freq);

        last_counter = current_counter;

        if (delta * 1000 < 1000.0 / FPS) SDL_Delay(static_cast<Uint32>(1000.0 / 60 - delta * 1000));

        // 游戏帧更新
        on_update(delta);

        // 游戏帧渲染
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        on_render();

        SDL_RenderPresent(renderer);
    }
    return 0;
}

GameManager::GameManager() {
    // SDL库初始化
    init_assert(!SDL_Init(SDL_INIT_EVERYTHING), "SDL2初始化失败");
    init_assert(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG), "SDL_image初始化失败");
    init_assert(Mix_Init(MIX_INIT_MP3), "SDL_mixer初始化失败");
    init_assert(!TTF_Init(), "SDL_ttf初始化失败");

    // 设置音频声道
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    // 开启输入法候选词窗口
    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");

    // 创建游戏窗口
    window = SDL_CreateWindow("村庄保卫战", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              1280, 720, SDL_WINDOW_SHOWN
    );
    init_assert(window, "游戏窗口创建失败");

    // 创建渲染器, 开启GPU渲染, 垂直同步, 设置渲染目标为纹理
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
                                  SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE
    );
    init_assert(renderer, "渲染器创建失败");
}

GameManager::~GameManager() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_Quit();
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}

void GameManager::init_assert(const bool flag, const char* err_msg) {
    if (flag) return;
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "游戏启动失败", err_msg, nullptr);
    exit(-1);
}

void GameManager::on_input() {
}

void GameManager::on_update(double delta) {
}

void GameManager::on_render() {
}