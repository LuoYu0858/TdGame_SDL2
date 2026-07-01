// Created by LuoYu on 2026/6/9.
#include "game_manager.h"

#include "wave_manager.h"
#include "enemy_manager.h"
#include "config_manager.h"
#include "resources_manager.h"

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

        if (delta * 1000 < 1000.0 / FPS) SDL_Delay(static_cast<Uint32>(1000.0 / FPS - delta * 1000));

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
    _init_assert(!SDL_Init(SDL_INIT_EVERYTHING), "SDL2初始化失败");
    _init_assert(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG), "SDL_image初始化失败");
    _init_assert(Mix_Init(MIX_INIT_MP3), "SDL_mixer初始化失败");
    _init_assert(!TTF_Init(), "SDL_ttf初始化失败");

    // 设置音频声道
    Mix_OpenAudio(AUDIO_FREQ, MIX_DEFAULT_FORMAT, AUDIO_TRACT, AUDIO_BUFF_SIZE);
    // 开启输入法候选词窗口
    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");

    // 加载配置文件
    ConfigManager* config = ConfigManager::instance();

    _init_assert(config->map.load("./config/map.csv"), "加载游戏地图失败");
    _init_assert(config->load_level_config("./config/level.json"), "加载关卡配置失败");
    _init_assert(config->load_game_config("./config/config.json"), "加载游戏配置失败");

    // 创建游戏窗口
    window = SDL_CreateWindow(config->basic_template.widow_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              config->basic_template.window_width, config->basic_template.window_height, SDL_WINDOW_SHOWN
    );
    _init_assert(window, "游戏窗口创建失败");

    // 创建渲染器, 开启GPU渲染, 垂直同步, 设置渲染目标为纹理
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
                                  SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE
    );
    _init_assert(renderer, "渲染器创建失败");

    // 加载静态资源
    _init_assert(ResourcesManager::instance()->load_from_file(renderer), "资源加载失败");

    // 加载瓦片地图
    _init_assert(_generate_tile_map_texture(), "生成瓦片地图纹理失败");
}

GameManager::~GameManager() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_Quit();
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}

void GameManager::_init_assert(const bool flag, const char* err_msg) {
    if (flag) return;
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "游戏启动失败", err_msg, nullptr);
    exit(-1);
}

void GameManager::on_input() {
}

void GameManager::on_update(double delta) {
    static ConfigManager* instance = ConfigManager::instance();

    if (!instance->is_game_over) {
        WaveManager::instance()->on_update(delta);
        EnemyManager::instance()->on_update(delta);
    }
}

void GameManager::on_render() {
    static ConfigManager* instance = ConfigManager::instance();
    static SDL_Rect& rect_dst = instance->rect_tile_map;
    SDL_RenderCopy(renderer, tex_tile_map, nullptr, &rect_dst);

    EnemyManager::instance()->on_render(renderer);
}

bool GameManager::_generate_tile_map_texture() {
    ConfigManager* config = ConfigManager::instance();
    const Map& map = config->map;
    const TileMap& tile_map = map.get_tile_map();
    // 渲染瓦片地图纹理的矩形坐标
    SDL_Rect& rect_tile_map = config->rect_tile_map;
    // 获取资源池中全部种类瓦片的自身纹理
    SDL_Texture* tex_tile_set = ResourcesManager::instance()->get_texture_pool().find(ResID::Tex_Tileset)->second;

    // 每个瓦片自身纹理尺寸
    int width_tex_tile_set, height_tex_tile_set;
    SDL_QueryTexture(tex_tile_set, nullptr, nullptr, &width_tex_tile_set, &height_tex_tile_set);

    // 静态文件中，每行瓦片的纹理个数
    const int num_tile_single_line = static_cast<int>(std::ceil(static_cast<double>(width_tex_tile_set) / SIZE_TILE));

    // 瓦片地图纹理尺寸
    const int width_tex_tile_map = static_cast<int>(map.get_width()) * SIZE_TILE;
    const int height_tex_tile_map = static_cast<int>(map.get_height()) * SIZE_TILE;

    // 创建纹理
    tex_tile_map = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                     SDL_TEXTUREACCESS_TARGET, width_tex_tile_map, height_tex_tile_map
    );

    if (!tex_tile_map) return false;

    rect_tile_map = {
        // 水平居中渲染瓦片地图
        (config->basic_template.window_width - width_tex_tile_map) / 2,
        // 垂直居中渲染瓦片地图
        (config->basic_template.window_height - height_tex_tile_map) / 2,
        width_tex_tile_map,
        height_tex_tile_map
    };

    SDL_SetTextureBlendMode(tex_tile_map, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(renderer, tex_tile_map);

    // 渲染到创建的纹理
    for (int y = 0; y < map.get_height(); ++y) {
        for (int x = 0; x < map.get_width(); ++x) {
            const Tile& tile = tile_map[y][x];

            // 目标渲染矩形
            const SDL_Rect& rect_dst = {
                x * SIZE_TILE,
                y * SIZE_TILE,
                SIZE_TILE,
                SIZE_TILE
            };
            // 源资源矩形
            SDL_Rect rect_src = {
                tile.terrain % num_tile_single_line * SIZE_TILE,
                tile.terrain / num_tile_single_line * SIZE_TILE,
                SIZE_TILE,
                SIZE_TILE
            };

            SDL_RenderCopy(renderer, tex_tile_set, &rect_src, &rect_dst);

            if (tile.decoration >= 0) {
                rect_src = {
                    tile.decoration % num_tile_single_line * SIZE_TILE,
                    tile.decoration / num_tile_single_line * SIZE_TILE,
                    SIZE_TILE,
                    SIZE_TILE
                };
                SDL_RenderCopy(renderer, tex_tile_set, &rect_src, &rect_dst);
            }
        }
    }

    // 房屋位置
    const auto& [x, y] = map.get_idx_home();
    const SDL_Rect rect_dst = {
        x * SIZE_TILE,
        y * SIZE_TILE,
        SIZE_TILE,
        SIZE_TILE
    };

    SDL_RenderCopy(renderer, ResourcesManager::instance()->get_texture_pool().find(ResID::Tex_Home)->second,
                   nullptr, &rect_dst
    );

    SDL_SetRenderTarget(renderer, nullptr);
    return true;
}